#!/bin/bash

set -eu

if [ "$#" -lt 2 ]; then
  echo "Usage: $0 BENCHMARK LAUNCHER [LAUNCHER_ARGS...]" >&2
  echo "The launcher must start exactly two PEs and bound execution time." >&2
  exit 1
fi

benchmark=$1
shift
output="./measurement-controls-$$.log"
trap 'rm -f "$output"' EXIT

run() {
  if ! "$@" > "$output" 2>&1; then
    cat "$output" >&2
    exit 1
  fi
}

for warmup in default 0 3; do
  args=()
  expected=20
  if [ "$warmup" != default ]; then
    args=(--warmup "$warmup")
    expected=$warmup
  fi
  run "$@" "$benchmark" --bench shmem_getmem --min 8 --max 8 \
    --ntimes 7 "${args[@]}"
  grep -Eq "Warmups: +$expected$" "$output"
  grep -Eq 'Ntimes: +7$' "$output"
  grep -q 'Avg MiB/s' "$output"
  grep -Eq '^8 +[0-9]+\.[0-9]+ +[0-9]+\.[0-9]+ *$' "$output"
done

run "$@" "$benchmark" --bench shmem_barrier_all --ntimes 7 --warmups 2
grep -Eq 'Warmups: +2$' "$output"
grep -q 'Total Time for 7 Barriers' "$output"

for option in warmup warmups ntimes; do
  values=(-1 invalid 1x 2147483648 999999999999999999999999999 '')
  if [ "$option" = ntimes ]; then
    values+=(0)
  fi
  for value in "${values[@]}"; do
    if "$@" "$benchmark" --bench shmem_barrier_all \
      "--$option" "$value" > "$output" 2>&1; then
      echo "Unexpected success for --$option '$value'" >&2
      cat "$output" >&2
      exit 1
    fi
    grep -q "Invalid --$option count:" "$output"
    if grep -q 'Avg Time per Barrier' "$output"; then
      echo "Invalid count produced a benchmark result" >&2
      exit 1
    fi
  done
done

echo "Measurement controls passed."
