#!/usr/bin/env python3

from typing import TypeVar, cast


runner = ["mpiexec.hydra", "-n", "2"]
cbin = "shmembench"
rsbin = "shmembench-rs"

latency_benches = [
    ("shmem_barrier_all", "barrier"),
    ("shmem_atomic_fetch", "atomic-fetch"),
    ("shmem_atomic_add", "atomic-cmp-swp"),
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
        result = subprocess.run(argv, capture_output=True, text=True, check=True)
        return result.stdout
    except subprocess.CalledProcessError as e:
        raise RuntimeError(f"failed running {args}:\n{e.stderr}")

shmrunrs = lambda *a: shmrun(rsbin, *a)
shmrunc = lambda *a: shmrun(cbin, *a)
T = TypeVar('T')
def unwrap(x: T, msg = "unwrapped falsy value") -> T:
    if x:
        return x
    else:
        raise Exception(msg)

US_PER_S = 1_000_000
B_PER_MIB = pow(1024, 2)
def extract_latency(out: str) -> float:
    import re
    avg = unwrap(re.findall(r'\d+\.\d{2,}', out), "didn't find any times in latency output")
    return float(avg[0]) if "(s)" in out else float(avg[0]) / US_PER_S

def run_latency_bench(cname: str, rsname: str) -> tuple[float, float]:
    cout = shmrunc("--bench", cname, "--benchtype", "latency", "--ntimes", "100")
    rsout = shmrunrs("--bench", rsname, "--ntimes", "100")
    return cast(tuple[float, float], tuple(map(extract_latency, (cout, rsout))))

def extract_bws(out: str) -> list[tuple[int, float, float]]:
    raise Exception("unimplemented")

def run_bw_bench(cname: str, rsname: str) -> list[tuple[tuple[float, float], tuple[float, float]]]:
    raise Exception("unimplemented")

if __name__ == '__main__':
    import csv
    with open(f"latency.csv", "w+", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["Routine", "C (baseline)", "RS (normalized)", "C (raw, us)", "RS (raw, us)"])
        for cname, rsname in latency_benches:
            latency = run_latency_bench(cname, rsname)
            writer.writerow([cname, 1.0, latency[1] / latency[0], latency[0], latency[1]])
