#!/bin/bash

set -e

./scripts/build.sh

# Get a list of unique hostnames from the srun command, sorted and concatenated with commas
hosts=$(srun hostname | sort | uniq | paste -sd, -)
# Count the number of unique hosts
num_hosts=$(echo "$hosts" | tr ',' '\n' | wc -l)

# Path to the oshrun executable
oshrun=$OSSS_TESTING_BIN/oshrun
# Check if oshrun is executable, exit if not
if [ ! -x $oshrun ]; then
  echo "osss-ucx has not been built!"
  exit 1
fi

# Path to the shmembench executable
shmembench=build/bin/shmembench

# Check if shmembench is executable, exit if not
if [ ! -x $shmembench ]; then
  echo "shmembench has not been built!"
  exit 1
fi

# Function to create a hostfile with specified slots per host
create_hostfile() {
  local ppn=$1
  local hostfile="hostfile.txt"
  # Clear the hostfile if it exists
  > "$hostfile"
  # Write each host and its slots to the hostfile
  for host in ${hosts//,/ }; do
    echo "$host slots=$ppn" >> "$hostfile"
  done
  # Return the hostfile name
  echo "$hostfile"
}

# Default values for processes per node and list of benchmarks
ppn=1
benchmarks=(shmem_put shmem_get shmem_alltoall shmem_alltoalls shmem_collect shmem_fcollect shmem_broadcast)
selected_bench=""
benchtype="bw"
min=8
max=1024

# Function to parse command-line arguments
parse_args() {
  while [[ $# -gt 0 ]]; do
    case $1 in
      --help)
        echo "Usage: $0 [--ppn PPN] [--bench BENCHMARK] [--benchtype BENCHTYPE] [--min MIN] [--max MAX]"
        echo "  --ppn PPN             Set processes per node (default: 1)"
        echo "  --bench BENCHMARK     Run only the specified benchmark. Valid options: ${benchmarks[*]}"
        echo "  --benchtype BENCHTYPE Set benchmark type: 'bw' or 'latency' (default: bw)"
        echo "  --min MIN             Set minimum size (default: 8)"
        echo "  --max MAX             Set maximum size (default: 1024)"
        echo "  --help                Show this help message and exit"
        exit 0
        ;;
      --bench)
        selected_bench=$2
        shift 2
        ;;
      --ppn)
        ppn=$2
        shift 2
        ;;
      --benchtype)
        if [[ $2 != "bw" && $2 != "latency" ]]; then
          echo "Error: --benchtype must be 'bw' or 'latency'"
          exit 1
        fi
        benchtype=$2
        shift 2
        ;;
      --min)
        min=$2
        shift 2
        ;;
      --max)
        max=$2
        shift 2
        ;;
      *)
        echo "Unknown option: $1"
        echo "Try '$0 --help' for more information."
        exit 1
        ;;
    esac
  done
}

# Parse the command-line arguments
parse_args "$@"

# Main function to run the benchmark
main() {
  local ppn=$1
  local num_hosts=$2
  local np=$((ppn * num_hosts))
  local bench=$3
  local benchtype=$4
  local min=$5
  local max=$6
  local hostfile=$(create_hostfile $ppn $num_hosts)

  case $bench in
    shmem_put|shmem_get|shmem_alltoall|shmem_alltoalls|shmem_collect|shmem_fcollect|shmem_broadcast)
      cmd="$oshrun -np $np -hostfile $hostfile $shmembench --bench $bench --benchtype $benchtype --min $min --max $max"
      echo "Running benchmark: $bench ($benchtype)"
      cat $hostfile ; echo
      echo "Running $cmd" ; echo
      $cmd
      ;;
    *)
      echo "Unknown or unspecified benchmark: $bench"
      ;;
  esac

  rm -f $hostfile
}

# If a specific benchmark is selected, validate and run it
if [[ -n $selected_bench ]]; then
  if [[ ! " ${benchmarks[@]} " =~ " $selected_bench " ]]; then
    echo "Error: Unknown benchmark '$selected_bench'."
    echo "Valid options are: ${benchmarks[*]}"
    exit 1
  fi
  main $ppn $num_hosts $selected_bench $benchtype $min $max
else
  if [[ ${#benchmarks[@]} -eq 0 ]]; then
    echo "Error: No benchmarks specified."
    exit 1
  fi
  for bench in "${benchmarks[@]}"; do
    main $ppn $num_hosts $bench $benchtype $min $max
  done
fi
