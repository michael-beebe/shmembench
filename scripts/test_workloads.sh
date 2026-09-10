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
  test "$(echo "$output" | grep -c '^RESULT .* check=pass$')" = 2
  for pe in 0 1; do
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
  if echo "$output" | grep -q '^RESULT '; then
    echo "Rejected input printed successful rows: $output" >&2
    exit 1
  fi
  cases=$((cases + 1))
}

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
    run --bench "$bench" --benchtype window
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
