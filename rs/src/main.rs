use std::{error::Error, hint::black_box, time::Instant};

use clap::{Parser, ValueEnum};
use openshmem_rs::ShmemCtx;

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
        .or(args.msg_size_max.map(|to| (1..to).collect()))
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
        println!("  Bindings Version:       1.5.3@74456b");
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
        Routine::Put => todo!(),
        Routine::AtomicAdd => todo!(),
        Routine::AtomicCmpSwp => todo!(),
        Routine::AtomicFetch => todo!(),
        Routine::AtomicInc => todo!(),
        Routine::Barrier => bench_barrier(n_times, &ctx),
    }

    Ok(())
}

fn bench_header() {
    println!("==============================================");
    println!("===          Benchmark Results             ===");
    println!("==============================================");
}

fn bench_get(ntimes: usize, sizes: Vec<usize>, ctx: &ShmemCtx) {
    let max_msg = *sizes.iter().max().unwrap();
    let shmalloc = ctx.shmallocator();
    let buffer = shmalloc.array(0u8, max_msg);

    ctx.barrier_all();
    let mut times = Vec::with_capacity(sizes.capacity());
    let mut dest = vec![0; max_msg];

    let access = ctx.pe(0);
    for size in sizes {
        ctx.barrier_all();
        let start = Instant::now();
        for _ in 0..ntimes {
            black_box(access.read_into(&buffer, 0..size, &mut dest));
        }
        ctx.barrier_all();
        times.push((size, start.elapsed().as_secs_f32()));
    }

    if ctx.my_pe() == 0 {
        bench_header();
        println!("size (b)\r\t\tlatency (us)\r\t\t\t\tbandwidth (mb/s)");
        for (size, time) in times {
            let latency = (time / ntimes as f32) / 1_000_000.0;
            let bandwidth = (size * ntimes) as f32 / time;
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

    let elapsed = start.elapsed().as_secs_f32();
    let avg = elapsed / ntimes as f32;

    if ctx.my_pe() == 0 {
        bench_header();
        println!("Avg Time per Barrier (s): {avg:.08} ({elapsed:.08} total)");
    }
    ctx.barrier_all();
}
