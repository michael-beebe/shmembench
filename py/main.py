from shmem4py import shmem
import numpy as np
import argparse
import time
from enum import Enum

class Routine(Enum):
    GET = "get"
    PUT = "put"
    ATOMICADD = "atomic-add"
    ATOMICCMPSWP = "atomic-cmp-swp"
    ATOMICFETCH = "atomic-fetch"
    ATOMICINC = "atomic-inc"
    BARRIER = "barrier"

    def uses_msg_size(self):
        return self in [Routine.GET, Routine.PUT]

    # Allow parsing from string command-line arg
    @classmethod
    def from_string(cls, s):
        try:
            return cls(s.lower())
        except ValueError:
             # Provide a helpful error message listing valid choices
             valid_choices = ", ".join([item.value for item in cls])
             raise ValueError(f"Invalid routine '{s}'. Choose from: {valid_choices}")


def parse_msg_sizes(value):
    """Helper to parse comma-separated msg sizes."""
    try:
        return [int(x) for x in value.split(',')]
    except ValueError:
        raise argparse.ArgumentTypeError(f"Invalid format for msg_sizes: '{value}'. Must be comma-separated integers.")

def generate_powers_of_2(max_val):
    """Generates powers of 2 up to max_val."""
    if max_val < 1:
        return []
    powers = []
    x = 1
    while x <= max_val:
        powers.append(x)
        x *= 2
    return powers

def main():
    parser = argparse.ArgumentParser(description="shmembench4py")
    parser.add_argument(
        "-b", "--bench",
        required=True,
        type=Routine.from_string, # Use the custom parser
        help=f"Benchmark routine to run. Choices: {[r.value for r in Routine]}"
    )
    parser.add_argument(
        "-n", "--ntimes",
        type=int,
        default=1000,
        help="Number of iterations for the benchmark loop (default: 1000)"
    )

    msg_size_group = parser.add_mutually_exclusive_group()
    msg_size_group.add_argument(
        "-s", "--msg-size-max",
        type=int,
        help="Generate powers-of-2 message sizes up to this maximum (bytes)"
    )
    msg_size_group.add_argument(
        "-M", "--msg-sizes",
        type=parse_msg_sizes, # Use the custom parser
        help="Comma-separated list of specific message sizes (bytes)"
    )

    args = parser.parse_args()

    # Determine message sizes
    if args.msg_sizes:
        msg_sizes = args.msg_sizes
    elif args.msg_size_max:
        msg_sizes = generate_powers_of_2(args.msg_size_max)
    else:
        # Default: powers of 2 from 1 byte up to 1 MiB (2^20)
        msg_sizes = generate_powers_of_2(1 << 20)

    # Ensure msg_sizes is not empty if needed
    if args.bench.uses_msg_size() and not msg_sizes:
         if args.msg_size_max is not None and args.msg_size_max < 1:
             print("Error: --msg-size-max must be at least 1.")
         else:
             # This case should ideally not happen with defaults, but good to check
             print("Error: No valid message sizes specified or generated for a benchmark that requires them.")
         return 1 # Indicate error

    shmemname = shmem.info_get_name()
    major, minor = shmem.info_get_version()
    n_pes = shmem.n_pes()
    n_times = args.ntimes
    benchname = args.bench
    my_pe_obj = shmem.my_pe()
    is_root = my_pe_obj == 0

    if is_root:
        print("==============================================")
        print("===         Test Information             ===")
        print("==============================================")
        print(f"  OpenSHMEM Name:         {shmemname}")
        print(f"  OpenSHMEM Version:      {major}.{minor}")
        print(f"  Bindings Version:       Python Placeholder") # Update if needed
        print(f"  Number of PEs:          {n_pes}")
        print(f"  Benchmark:              {benchname.name}") # Use enum member name
        if args.bench.uses_msg_size():
            min_s = min(msg_sizes)
            max_s = max(msg_sizes)
            print(f"  Min Msg Size (bytes):   {min_s}")
            print(f"  Max Msg Size (bytes):   {max_s}")
        print(f"  Ntimes:                 {n_times}")

    # Dispatch to the correct benchmark function
    if benchname == Routine.GET:
        bench_get(n_times, msg_sizes)
    elif benchname == Routine.PUT:
        bench_put(n_times, msg_sizes)
    elif benchname == Routine.ATOMICADD:
        bench_atomic_add(n_times)
    elif benchname == Routine.ATOMICCMPSWP:
        bench_atomic_cmp_swp(n_times)
    elif benchname == Routine.ATOMICFETCH:
        bench_atomic_fetch(n_times)
    elif benchname == Routine.ATOMICINC:
        bench_atomic_inc(n_times)
    elif benchname == Routine.BARRIER:
        bench_barrier(n_times)
    else:
        # Should be caught by argparse, but good practice
        if is_root:
            print(f"Error: Unknown benchmark routine '{benchname}'")
        return 1

    return 0 # Indicate success

def header():
    print("==============================================")
    print("===         Benchmark Results            ===")
    print("==============================================")

BYTES_PER_MIB = 1024.0 * 1024.0
US_PER_S = 1_000_000.0

def bench_atomic_inc(ntimes):
    dest = shmem.array([0], dtype='i')
    #target_pe = (shmem.my_pe() + 1) % 2
    target_pe = 0

    shmem.barrier_all()
    start_time = time.perf_counter_ns()

    for _ in range(ntimes):
        shmem.atomic_inc(dest, target_pe)

    shmem.quiet()
    shmem.barrier_all()

    end_time = time.perf_counter_ns()
    elapsed_ns = end_time - start_time
    elapsed_us = elapsed_ns / 1000.0
    avg_us = elapsed_us / ntimes

    if shmem.my_pe() == 0:
        header()
        print(f"Avg Time per Increment (us): {avg_us:.8f} ({elapsed_us:.2f} total us)")

    shmem.barrier_all()

def bench_atomic_cmp_swp(ntimes):
    dest = shmem.array([0], dtype='i')
    #target_pe = (shmem.my_pe() + 1) % 2
    target_pe = 0
    my_rank_int = shmem.my_pe()
    # Each PE tries to swap its rank into the value if it's 0
    swp_with = my_rank_int
    if_eqs = 0

    shmem.barrier_all()
    start_time = time.perf_counter_ns()

    for _ in range(ntimes):
        shmem.atomic_compare_swap(dest, if_eqs, swp_with, target_pe)

    shmem.quiet()
    shmem.barrier_all()

    end_time = time.perf_counter_ns()
    elapsed_ns = end_time - start_time
    elapsed_us = elapsed_ns / 1000.0
    avg_us = elapsed_us / ntimes

    if shmem.my_pe() == 0:
        header()
        print(f"Avg Time per Compare+Swap (us): {avg_us:.8f} ({elapsed_us:.2f} total us)")

    shmem.barrier_all()

def bench_atomic_fetch(ntimes):
    dest = shmem.array([0], dtype='i')
    #target_pe = (shmem.my_pe() + 1) % 2
    target_pe = 0

    shmem.barrier_all()
    start_time = time.perf_counter_ns()

    for _ in range(ntimes):
        _val = shmem.atomic_fetch(dest, target_pe)

    shmem.quiet()
    shmem.barrier_all()

    end_time = time.perf_counter_ns()
    elapsed_ns = end_time - start_time
    elapsed_us = elapsed_ns / 1000.0
    avg_us = elapsed_us / ntimes

    if shmem.my_pe() == 0:
        header()
        print(f"Avg Time per Fetch (us): {avg_us:.8f} ({elapsed_us:.2f} total us)")

    shmem.barrier_all()

def bench_atomic_add(ntimes):
    dest = shmem.array([0], dtype='i')
    #target_pe = (shmem.my_pe() + 1) % 2
    target_pe = 0
    value_to_add = 1

    shmem.barrier_all()
    start_time = time.perf_counter_ns()

    for _ in range(ntimes):
        shmem.atomic_add(dest, value_to_add, target_pe)

    shmem.quiet()
    shmem.barrier_all()

    end_time = time.perf_counter_ns()
    elapsed_ns = end_time - start_time
    elapsed_us = elapsed_ns / 1000.0
    avg_us = elapsed_us / ntimes

    if shmem.my_pe() == 0:
        header()
        print(f"Avg Time per Add (us): {avg_us:.8f} ({elapsed_us:.2f} total us)")

    shmem.barrier_all()

def bench_put(ntimes, sizes):
    if not sizes: return
    max_msg = max(sizes)
    dest = shmem.zeros(max_msg, dtype='u1')
    #target_pe = (shmem.my_pe() + 1) % 2
    target_pe = 0

    shmem.barrier_all()
    times = []
    src = shmem.zeros(max_msg, dtype='u1')

    for size in sizes:
        data_to_put = src[:size]
        shmem.barrier_all()
        start_time = time.perf_counter()

        for _ in range(ntimes):
            shmem.put(dest, data_to_put, target_pe)

        shmem.quiet()
        shmem.barrier_all()
        end_time = time.perf_counter()
        times.append((size, end_time - start_time))

    if shmem.my_pe() == 0:
        header()
        print("size (b)\t\tlatency (us)\t\t\tbandwidth (MiB/s)")
        for size, total_time in times:
             if total_time > 0:
                 latency_s = total_time / ntimes
                 latency_us = latency_s * US_PER_S
                 bandwidth_mib_s = (size * ntimes) / total_time / BYTES_PER_MIB
                 print(f"{size}\t\t\t{latency_us:>10.2f}\t\t\t{bandwidth_mib_s:>15.2f}")
             else:
                 print(f"{size}\t\t\t{'inf':>10}\t\t\t{'inf':>15}")
    shmem.barrier_all()


def bench_get(ntimes, sizes):
    if not sizes: return
    max_msg = max(sizes)
    src = shmem.ones(max_msg, dtype='u1') # Fill with 1 (byte)
    #source_pe = (shmem.my_pe() + 1) % 2
    source_pe = 0

    shmem.barrier_all()
    times = []
    # Local destination buffer
    dest_local = shmem.zeros(max_msg, dtype='u1') # Mutable local buffer

    for size in sizes:
        shmem.barrier_all() # Sync before timing this size
        start_time = time.perf_counter()

        for _ in range(ntimes):
            shmem.get(dest_local, src, source_pe, size=size)

        shmem.quiet()
        shmem.barrier_all()
        end_time = time.perf_counter()
        times.append((size, end_time - start_time))

    if shmem.my_pe() == 0:
        header()
        print("size (b)\t\tlatency (us)\t\t\tbandwidth (MiB/s)")
        for size, total_time in times:
             if total_time > 0:
                 latency_s = total_time / ntimes
                 latency_us = latency_s * US_PER_S
                 bandwidth_mib_s = (size * ntimes) / total_time / BYTES_PER_MIB
                 print(f"{size}\t\t\t{latency_us:>10.2f}\t\t\t{bandwidth_mib_s:>15.2f}")
             else:
                 print(f"{size}\t\t\t{'inf':>10}\t\t\t{'inf':>15}")
    shmem.barrier_all()


def bench_barrier(ntimes):
    shmem.barrier_all()
    start_time = time.perf_counter_ns()

    for _ in range(ntimes):
        shmem.barrier_all()

    end_time = time.perf_counter_ns()
    elapsed_ns = end_time - start_time
    elapsed_us = elapsed_ns / 1000.0
    avg_us = elapsed_us / ntimes

    if shmem.my_pe() == 0:
        header()
        print(f"Avg Time per Barrier (us): {avg_us:.8f} ({elapsed_us:.2f} total us)")

    shmem.barrier_all()


if __name__ == "__main__":
    import sys
    sys.exit(main())
