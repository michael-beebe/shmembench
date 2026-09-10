#!/bin/bash
set -eu

if [ "$#" -lt 2 ]; then
  echo "Usage: SHMEM_VERSION=14|15|16 $0 BENCHMARK LAUNCHER [ARGS...]" >&2
  echo "Launcher must start exactly two PEs with a timeout." >&2
  exit 1
fi
benchmark=$1
shift
launcher=("$@")
version=${SHMEM_VERSION:-15}
cases=0

run() {
  echo "CHECK $*"
  if ! output=$("${launcher[@]}" "$benchmark" --ntimes 2 --warmup 1 \
      --window 3 --min 32 --max 32 "$@" 2>&1); then
    echo "$output" >&2
    exit 1
  fi
  echo "$output" | grep '^RESULT '
  expected=2
  if [[ " $* " == *" --bench shmem_putmem_signal_nbi_overlap "* ]]; then
    expected=1
    echo "$output" | grep '^NOTIFY_RESULT '
    test "$(echo "$output" | grep -c '^NOTIFY_RESULT .* pe=1 .* check=pass$')" = 1
  fi
  test "$(echo "$output" | grep -c '^RESULT .* check=pass$')" = "$expected"
  for ((pe=0; pe<expected; pe++)); do
    test "$(echo "$output" | grep -c "^RESULT .* pe=$pe .* check=pass$")" = 1
  done
  echo "$output" | awk '/^RESULT / {
    for (i=1; i<=NF; i++) {
      split($i, a, "=")
      if ((a[1]=="us" || a[1]=="ops_per_s") && !(a[2]+0 > 0)) exit 1
    }
  }'
  cases=$((cases + 1))
}

reject() {
  echo "REJECT $*"
  status=0
  output=$("${launcher[@]}" "$benchmark" --ntimes 2 --warmup 0 "$@" 2>&1) || status=$?
  if [ "$status" != 1 ]; then
    echo "Expected failure status 1, got $status: $output" >&2
    exit 1
  fi
  if echo "$output" | grep -Eq '^(RESULT|NOTIFY_RESULT) '; then
    echo "Rejected input printed successful rows: $output" >&2
    exit 1
  fi
  cases=$((cases + 1))
}

if [ "${WORKLOAD_CONTEXTS_ONLY:-0}" != 1 ]; then
  for mode in issue quiet window; do
    run --bench shmem_atomic_add --benchtype "$mode" --target contended
    if [ "$version" -ge 15 ]; then
      run --bench shmem_putmem_signal_nbi_overlap --benchtype "$mode" --compute-us 0
    fi
  done
  run --bench shmem_atomic_add --benchtype window --target contended --contexts 8 --ntimes 9
  if [ "$version" -ge 15 ]; then
    run --bench shmem_putmem_signal_nbi_overlap --benchtype window --compute-us 1000
    run --bench shmem_putmem_signal_nbi_overlap --benchtype window --compute-us 1000 --contexts 8 --ntimes 9
  else
    reject --bench shmem_putmem_signal_nbi_overlap --benchtype window
  fi
  reject --bench shmem_atomic_fetch_add --benchtype window --target contended
  reject --bench shmem_atomic_add --compute-us 1
  reject --bench shmem_putmem_signal_nbi_overlap --target self
  reject --bench shmem_putmem_signal_nbi_overlap --compute-us -1
  if [ "${WORKLOAD_W8_ONLY:-0}" = 1 ]; then
    echo "Contention/overlap controls passed: $cases cases (OpenSHMEM $version)."
    exit 0
  fi
fi

for contexts in 0 1 8 32; do
  run --bench shmem_atomic_add --benchtype window --contexts "$contexts" --ntimes 33
  if [ "$contexts" != 0 ]; then
    echo "$output" | grep '^CONTEXTS '
    for pe in 0 1; do
      test "$(echo "$output" | grep -c "^CONTEXTS pe=$pe count=$contexts ")" = 1
    done
  fi
done
run --bench shmem_atomic_fetch_add --benchtype issue --contexts 8 --ntimes 9
run --bench shmem_putmem_nbi --benchtype quiet --contexts 8 --ntimes 9
run --bench shmem_getmem_nbi --benchtype window --contexts 8 --ntimes 9
if [ "$version" -ge 15 ]; then
  run --bench shmem_putmem_signal_nbi --benchtype window --contexts 8 --ntimes 9
fi
reject --bench shmem_test --benchtype issue --contexts 1
reject --bench shmem_getmem --contexts 1
reject --bench shmem_atomic_add --contexts -1
reject --bench shmem_atomic_add --contexts 3x
if [ "${WORKLOAD_CONTEXTS_ONLY:-0}" = 1 ]; then
  echo "Context controls passed: $cases cases (OpenSHMEM $version)."
  exit 0
fi

for mode in issue quiet window; do
  for bench in shmem_atomic_add shmem_atomic_fetch_add shmem_putmem_nbi shmem_getmem_nbi; do
    run --bench "$bench" --benchtype "$mode"
  done
done
for target in self random; do
  run --bench shmem_atomic_add --benchtype window --target "$target"
done
for op in inc set fetch swap compare_swap and or xor; do
  run --bench "shmem_atomic_$op" --benchtype latency
done
for bench in shmem_p shmem_g shmem_test shmem_wait_until shmem_iput shmem_iget; do
  run --bench "$bench" --benchtype quiet --stride 3
done
run --bench shmem_test --benchtype issue --ready 0

if [ "$version" -ge 15 ]; then
  for bench in shmem_atomic_fetch_nbi shmem_putmem_signal shmem_putmem_signal_nbi \
      shmem_signal_fetch shmem_signal_wait_until; do
    if [ "$bench" = shmem_signal_wait_until ] &&
       [ "${DISABLE_SIGNAL_WAIT_UNTIL:-0}" = 1 ]; then
      reject --bench "$bench" --benchtype window
    else
      run --bench "$bench" --benchtype window
    fi
  done
  for family in test wait_until; do
    for kind in all any some all_vector any_vector some_vector; do
      run --bench "shmem_${family}_$kind" --benchtype issue --mask 50
    done
  done
  run --bench shmem_test_any --benchtype issue --ready 0
  run --bench shmem_test_all --benchtype issue --ready 0 --mask 100
  run --bench shmem_test_some --benchtype issue --mask 100
else
  reject --bench shmem_atomic_fetch_nbi --benchtype window
  reject --bench shmem_alltoallmem
  reject --bench shmem_test_all --benchtype issue
fi
if [ "$version" -ge 16 ]; then
  run --bench shmem_ibput --benchtype window --stride 3 --block-size 2
  run --bench shmem_ibget --benchtype window --stride 3 --block-size 2
else
  reject --bench shmem_ibput --benchtype window
fi
reject --bench missing
reject --bench shmem_atomic_add --benchtype bw
reject --bench shmem_barrier_all --benchtype window
reject --bench shmem_wait_until --ready 0
reject --bench shmem_test_all --benchtype issue --min 1 --max 1
reject --bench shmem_atomic_add --window 0
reject --bench shmem_atomic_add --window 3x
reject --bench shmem_atomic_add --target elsewhere
reject --bench shmem_getmem --min 32 --max 8
reject --bench shmem_getmem --max 2147483647
echo "Workload controls passed: $cases cases (OpenSHMEM $version)."
