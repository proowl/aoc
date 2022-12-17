use std::io::{self, BufRead};
use sscanf::sscanf;

fn part1(input : &Vec<String>) {
    let mut cycle = 0;
    let mut x = 1;
    let mut signal_sum = 0;
    let mut i = 0;
    for l in input {
        i += 1;
        cycle += 1;
        let cmd_reg = sscanf!(l, "{str} {i64}");
        match cmd_reg {
            Ok((cmd, num)) => {
                let signal_strength = cycle * x;
                // println!("{} mid cycle {} x {} strength {}", i, cycle, x, signal_strength);
                if (cycle+20)%40 == 0 {
                    signal_sum += signal_strength;
                }
                cycle += 1;
                let signal_strength = cycle * x;
                // println!("{} mid cycle {} x {} strength {}", i, cycle, x, signal_strength);
                if (cycle+20)%40 == 0 {
                    signal_sum += signal_strength;
                }
                x += num;
            },
            Err(MatchFailed) => {
                let signal_strength = cycle * x;
                // println!("{} mid cycle {} x {} strength {}", i, cycle, x, signal_strength);
                if (cycle+20)%40 == 0 {
                    signal_sum += signal_strength;
                }
            },
        }
    }

    println!("total signal = {}", signal_sum);
}

fn part2(input : &Vec<String>) {
    let mut cycle = 0;
    let mut x = 1;
    let mut signal_sum = 0;
    let mut i = 0;

    let width = 40;
    let height = 6;
    let mut crt_pos = 0;

    for l in input {
        i += 1;
        cycle += 1;
        let cmd_reg = sscanf!(l, "{str} {i64}");
        match cmd_reg {
            Ok((cmd, num)) => {
                if crt_pos >= x-1 && crt_pos <= x+1 {
                    print!("#");
                } else {
                    print!(".");
                }
                crt_pos += 1;
                if crt_pos % width == 0 {
                    println!("");
                    crt_pos = 0;
                }
                cycle += 1;
                if crt_pos >= x-1 && crt_pos <= x+1 {
                    print!("#");
                } else {
                    print!(".");
                }
                crt_pos += 1;
                if crt_pos % width == 0 {
                    println!("");
                    crt_pos = 0;
                }
                x += num;
            },
            Err(MatchFailed) => {
                if crt_pos >= x-1 && crt_pos <= x+1 {
                    print!("#");
                } else {
                    print!(".");
                }
                crt_pos += 1;
                if crt_pos % width == 0 {
                    println!("");
                    crt_pos = 0;
                }
            },
        }
    }

    println!("total signal = {}", signal_sum);
}

fn main() {
    let stdin = io::stdin();
    let mut input = Vec::<String>::new();
    for line in stdin.lock().lines() {
        input.push(line.unwrap());
    }

    part2(&input);
}
