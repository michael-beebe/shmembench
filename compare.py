#!/usr/bin/env python3

from os import environ, mkdir
import os
from typing import TypeVar, cast, List, Tuple
import re


n_times = 1000
runner = ["mpiexec.hydra", "-n", "2", "-bind-to", "hwthread"]
cbin = "shmembench"
rsbin = "shmembench-rs"
pybin = "python ./py/main.py"

latency_benches = [
    ("shmem_barrier_all", "barrier"),
    ("shmem_atomic_fetch", "atomic-fetch"),
    ("shmem_atomic_add", "atomic-add"),
    ("shmem_atomic_compare_swap", "atomic-cmp-swp"),
    ("shmem_atomic_inc", "atomic-inc")
]

bw_benches = [
    ("shmem_get", "get"),
    ("shmem_put", "put")
]

def shmrun(*args: str) -> str:
    import subprocess
    try:
        argv = [*runner, *args]
        print(f"running:  {' '.join(argv)}")
        result = subprocess.run(argv, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True, check=True)
        return result.stdout
    except subprocess.CalledProcessError as e:
        raise RuntimeError(f"failed running {args}:\n{e.stderr}")

shmrunrs = lambda *a: shmrun(rsbin, *a)
shmrunc = lambda *a: shmrun(cbin, *a)
shmrunpy = lambda *a: shmrun(pybin, *a)

T = TypeVar('T')
def unwrap(x: T, msg = "unwrapped falsy value") -> T:
    if x:
        return x
    else:
        raise Exception(msg)

US_PER_S = 1_000_000
B_PER_MIB = pow(1024, 2)

def extract_latency(out: str) -> float:
    avg = unwrap(re.findall(r'\d+\.\d{2,}', out), "didn't find any times in latency output")
    return (float(avg[0]) / US_PER_S) if "(s)" in out else float(avg[0])

def run_latency_bench(cname: str, rsname: str, ntimes: int) -> Tuple[float, float, float]:
    cout = shmrunc("--bench", cname, "--benchtype", "latency", "--ntimes", str(ntimes))
    rsout = shmrunrs("--bench", rsname, "--ntimes", str(ntimes))
    pyout = shmrunpy("--bench", rsname, "--ntimes", str(ntimes))
    return cast(Tuple[float, float, float], tuple(map(extract_latency, (cout, rsout, pyout))))

last = lambda xs: xs[len(xs) - 1]

bw_data_point_r = re.compile(r"(\d+)\s+(\d+\.\d+)\s+(\d+\.\d+)?", re.MULTILINE)
def extract_bws(out: str) -> List[Tuple[int, float, float]]:
    return [(int(sz), float(us), float(bw or 0))
            for sz, us, bw in
            (found.group(1, 2, 3)
             for found in bw_data_point_r.finditer(out))]

def run_bw_bench(
    cname: str,
    rsname: str,
    # args: Iterable[str],
    # rsargs: Iterable[str] = [],
    # cargs: Iterable[str] = []
) -> List[Tuple[int, Tuple[float, float], Tuple[float, float], Tuple[float, float]]]:
    rsout = shmrunrs("--bench", rsname, "--ntimes", "100", "--msg-size-max", "1048577")
    pyout = shmrunpy("--bench", rsname, "--ntimes", "100", "--msg-size-max", "1048577")
    cout = shmrunc("--bench", cname, "--benchtype", "latency", "--ntimes", "100", "--min", "1", "--max", "1048576")
    cout = extract_bws(cout)
    rsout = extract_bws(rsout)
    pyout = extract_bws(pyout)
    return [(csize, (cus, cbw), (rsus, rsbw), (pyus, pybw))
            for ((csize, cus, cbw), (_rssize, rsus, rsbw), (_pysize, pyus, pybw))
            in zip(cout, rsout, pyout)]

median = lambda xs: list(sorted(xs))[int(len(list(xs)) / 2)]

str_of_float = lambda x: "%.10f" % x

median_n = 7
assert median_n % 2, "median_n must be odd"

if __name__ == '__main__':
    import csv
    from shutil import rmtree
    rmtree("/tmp/results", True)
    mkdir("/tmp/results")
    with open(f"/tmp/results/latency.csv", "w+", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["Routine", "C (baseline)", "RS (normalized)", "Py (normalized)", "C (raw, us)", "RS (raw, us)", "Py (raw, us)"])
        for cname, rsname in latency_benches:
            latencies = [run_latency_bench(cname, rsname, n_times) for _ in range(median_n)]
            latency = median(x[0] for x in latencies), median(x[1] for x in latencies), median(x[2] for x in latencies)
            writer.writerow([cname, 1.0, *map(str_of_float, [latency[1] / latency[0], latency[0], latency[1]])])
    for cname, rsname in bw_benches:
        with open(f"/tmp/results/bw_{cname}.csv", "w+", newline="") as f:
            writer = csv.writer(f)
            writer.writerow(["Msg Size (b)", "C (baseline)", "RS (normalized)", "Py (normalized)", "C (raw, us)", "RS (raw, us)", "Py (raw, us)"])
            datapoints = run_bw_bench(cname, rsname)
            for size, (ctime, cbw), (rstime, rsbw), (pytime, pybw) in datapoints:
                writer.writerow([size, *map(str_of_float, [1.0, rstime / ctime, pytime / ctime, ctime, rstime, pytime])])
