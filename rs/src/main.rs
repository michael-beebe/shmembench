use std::{error::Error, hint::black_box, time::Instant};

use clap::{Parser, ValueEnum};
use openshmem_rs::{atomics::Atomic, ShmemCtx, PE};

#[derive(Debug, ValueEnum, Copy, Clone, PartialEq, Eq, PartialOrd, Ord)]
enum Routine {
    Get,
    Put,
    AtomicAdd,
    AtomicCmpSwp,
    AtomicFetch,
    AtomicInc,
    Barrier,
}
impl Routine {
    fn uses_msg_size(&self) -> bool {
        use Routine::*;
        match self {
            Get | Put => true,
            _ => false,
        }
    }
}

#[derive(Parser, Debug)]
struct CliArgs {
    #[arg(short, long = "bench")]
    routine: Routine,
    #[arg(short, long, default_value_t = 1000)]
    ntimes: usize,
    #[arg(short, long, group = "msg_size")]
    msg_size_max: Option<usize>,
    #[arg(short = 'M', long, group = "msg_size", value_delimiter = ',')]
    msg_sizes: Option<Vec<usize>>,
}

fn main() -> Result<(), Box<dyn Error>> {
    let args = CliArgs::parse();
    let msg_sizes = args
        .msg_sizes
        .or(args.msg_size_max.map(|max| {
            let mut powers = Vec::with_capacity((max.ilog2() + 1) as usize);
            let mut x = 1;
            while x <= max {
                powers.push(x);
                x *= 2;
            }
            powers
        }))
        .unwrap_or((0..=20).map(|x| 1 << x).collect());

    let ctx = ShmemCtx::init()?;
    let shmemname = ctx.info_get_name_str();
    let (major, minor) = ctx.info_get_version();
    let n_pes = ctx.n_pes();
    let n_times = args.ntimes;
    let benchname = &args.routine;
    let is_root = ctx.my_pe() == 0;

    if is_root {
        println!("==============================================");
        println!("===          Test Information              ===");
        println!("==============================================");
        println!("  OpenSHMEM Name:         {shmemname}");
        println!("  OpenSHMEM Version:      {major}.{minor}");
        println!("  Bindings Version:       1.5.3@cab22ea"); // update me!
        println!("  Number of PEs:          {n_pes}");
        println!("  Benchmark:              {benchname:?}");
        if args.routine.uses_msg_size() {
            let min = msg_sizes
                .iter()
                .min()
                .expect("at least one generated msg size");
            let max = msg_sizes
                .iter()
                .max()
                .expect("at least one generated msg size");
            println!("  Min Msg Size (bytes):   {min}");
            println!("  Max Msg Size (bytes):   {max}");
        }
        println!("  Ntimes:                 {n_times}");
    }

    match args.routine {
        Routine::Get => bench_get(n_times, msg_sizes, &ctx),
        Routine::Put => bench_put(n_times, msg_sizes, &ctx),
        Routine::AtomicAdd => bench_atomic_add(n_times, &ctx),
        Routine::AtomicCmpSwp => bench_atomic_cmp_swp(n_times, &ctx),
        Routine::AtomicFetch => bench_atomic_fetch(n_times, &ctx),
        Routine::AtomicInc => bench_atomic_inc(n_times, &ctx),
        Routine::Barrier => bench_barrier(n_times, &ctx),
    }

    Ok(())
}

fn header() {
    println!("==============================================");
    println!("===          Benchmark Results             ===");
    println!("==============================================");
}

// TODO: macro

const BYTES_PER_MIB: f32 = 1024.0 * 1024.0;
const US_PER_S: f32 = 1_000_000.0;

fn bench_atomic_inc(ntimes: usize, ctx: &ShmemCtx) {
    let shmalloc = ctx.shmallocator();
    let mut dest = shmalloc.shbox(Atomic::new(0u32));
    let mut target_pe = PE(0);
    ctx.barrier_all();
    let start = Instant::now();

    for _ in 0..ntimes {
        black_box(dest.atomic_inc(target_pe, ctx));
    }
    unsafe { openshmem_rs::ffi::shmem_quiet(); }
    ctx.barrier_all();

    let elapsed = start.elapsed().as_micros() as f32;
    let avg = elapsed / ntimes as f32;

    dbg!(dest.atomic_fetch(target_pe, ctx));
    if ctx.my_pe() == 0 {
        header();
        println!("Avg Time per Increment (us): {avg:.08} ({elapsed:.08} total)");
    }
    ctx.barrier_all();
}

fn bench_atomic_cmp_swp(ntimes: usize, ctx: &ShmemCtx) {
    let shmalloc = ctx.shmallocator();
    let mut dest = shmalloc.shbox(Atomic::new(0u32));
    let mut target_pe = PE(0);
    let swp_with = ctx.my_pe().raw() as u32;
    let if_eqs = (swp_with + 1) % ctx.n_pes() as u32;
    ctx.barrier_all();
    let start = Instant::now();

    for _ in 0..ntimes {
        black_box(dest.atomic_compare_swap(0, swp_with, target_pe, ctx));
    }
    unsafe { openshmem_rs::ffi::shmem_quiet(); }
    ctx.barrier_all();

    let elapsed = start.elapsed().as_micros() as f32;
    let avg = elapsed / ntimes as f32;

    if ctx.my_pe() == 0 {
        header();
        println!("Avg Time per Compare+Swap (us): {avg:.08} ({elapsed:.08} total)");
    }
    ctx.barrier_all();
}

fn bench_atomic_fetch(ntimes: usize, ctx: &ShmemCtx) {
    let shmalloc = ctx.shmallocator();
    let mut dest = shmalloc.shbox(Atomic::new(0u32));
    let mut target_pe = PE(0);
    ctx.barrier_all();
    let start = Instant::now();

    for _ in 0..ntimes {
        black_box(dest.atomic_fetch(target_pe, ctx));
    }
    unsafe { openshmem_rs::ffi::shmem_quiet(); }
    ctx.barrier_all();

    let elapsed = start.elapsed().as_micros() as f32;
    let avg = elapsed / ntimes as f32;

    if ctx.my_pe() == 0 {
        header();
        println!("Avg Time per Fetch (us): {avg:.08} ({elapsed:.08} total)");
    }
    ctx.barrier_all();
}

fn bench_atomic_add(ntimes: usize, ctx: &ShmemCtx) {
    let shmalloc = ctx.shmallocator();
    let mut dest = shmalloc.shbox(Atomic::new(0u32));
    let mut target_pe = PE(0);
    ctx.barrier_all();
    let start = Instant::now();

    for _ in 0..ntimes {
        black_box(dest.atomic_add(1, target_pe, ctx));
    }
    unsafe { openshmem_rs::ffi::shmem_quiet(); }
    ctx.barrier_all();

    let elapsed = start.elapsed().as_micros() as f32;
    let avg = elapsed / ntimes as f32;

    if ctx.my_pe() == 0 {
        header();
        println!("Avg Time per Add (us): {avg:.08} ({elapsed:.08} total)");
    }
    ctx.barrier_all();
}

fn bench_put(ntimes: usize, sizes: Vec<usize>, ctx: &ShmemCtx) {
    let max_msg = *sizes.iter().max().unwrap();
    let shmalloc = ctx.shmallocator();
    let mut dest = shmalloc.array(|x| x as u8, max_msg);

    ctx.barrier_all();
    let mut times = Vec::with_capacity(sizes.capacity());
    let mut src = vec![ctx.my_pe().raw() as u8; max_msg];

    let access = ctx.pe(0);
    for size in sizes {
        let data = &src[0..size];
        unsafe { openshmem_rs::ffi::shmem_quiet(); }
        ctx.barrier_all();
        let start = Instant::now();
        for _ in 0..ntimes {
            black_box(access.write_from(&mut dest, data));
        }
        unsafe { openshmem_rs::ffi::shmem_quiet(); }
        ctx.barrier_all();
        times.push((size, start.elapsed().as_secs_f32()));
    }

    if ctx.my_pe() == 0 {
        header();
        println!("size (b)\r\t\tlatency (us)\r\t\t\t\tbandwidth (mb/s)");
        for (size, time) in times {
            let latency = (time / ntimes as f32) * US_PER_S;
            let bandwidth = (size * ntimes) as f32 / time / BYTES_PER_MIB;
            println!("{size}\r\t\t{latency:>0.2}\r\t\t\t\t{bandwidth:>0.2}");
        }
    }
}

fn bench_get(ntimes: usize, sizes: Vec<usize>, ctx: &ShmemCtx) {
    let max_msg = *sizes.iter().max().unwrap();
    let shmalloc = ctx.shmallocator();
    let src = shmalloc.array_gen(|x| x as u8, max_msg);

    ctx.barrier_all();
    let mut times = Vec::with_capacity(sizes.capacity());
    let mut dest = vec![0; max_msg];

    let access = ctx.pe(0);
    for size in sizes {
        unsafe { openshmem_rs::ffi::shmem_quiet(); }
        ctx.barrier_all();
        let start = Instant::now();
        for _ in 0..ntimes {
            black_box(access.read_into(&src, 0..size, &mut dest));
        }
        unsafe { openshmem_rs::ffi::shmem_quiet(); }
        ctx.barrier_all();
        times.push((size, start.elapsed().as_secs_f32()));
    }

    if ctx.my_pe() == 0 {
        header();
        println!("size (b)\r\t\tlatency (us)\r\t\t\t\tbandwidth (mb/s)");
        for (size, time) in times {
            let latency = (time / ntimes as f32) * US_PER_S;
            let bandwidth = (size * ntimes) as f32 / time / BYTES_PER_MIB;
            println!("{size}\r\t\t{latency:>0.2}\r\t\t\t\t{bandwidth:>0.2}");
        }
    }
}

fn bench_barrier(ntimes: usize, ctx: &ShmemCtx) {
    ctx.barrier_all();
    let start = Instant::now();

    for _ in 0..ntimes {
        black_box(ctx.barrier_all());
    }

    let elapsed = start.elapsed().as_micros() as f32;
    let avg = elapsed / ntimes as f32;

    if ctx.my_pe() == 0 {
        header();
        println!("Avg Time per Barrier (us): {avg:.08} ({elapsed:.08} total)");
    }
    ctx.barrier_all();
}
