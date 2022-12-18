use std::io::{self, BufRead};
use scanf::sscanf;

fn part1(input: &Vec<String>) -> i32 {
    let mut c: i32 = 0;
    for line in input {
        let mut a1: i32 = 0;
        let mut a2: i32 = 0;
        let mut b1: i32 = 0;
        let mut b2: i32 = 0;
        sscanf!(&line, "{}-{},{}-{}", a1, a2, b1, b2);
        if a1 <= b1 && b2 <= a2
            || b1 <= a1 && a2 <= b2 {
                c += 1;
            }
    }
    return c;
}

fn part2(input: &Vec<String>) -> i32 {
    let mut c: i32 = 0;
    for line in input {
        let mut a1: i32 = 0;
        let mut a2: i32 = 0;
        let mut b1: i32 = 0;
        let mut b2: i32 = 0;
        sscanf!(&line, "{}-{},{}-{}", a1, a2, b1, b2);
        if a1 <= b1 && b1 <= a2
            || a1 <= b2 && b2 <= a2
            || b1 <= a1 && a1 <= b2
            || b1 <= a2 && a2 <= b2 {
                c += 1;
            }
    }
    return c;
}

fn main() {
    let mut lines = io::stdin().lock().lines();
    let mut input: Vec<String> = Vec::new();
    while let Some(line) = lines.next() {
        input.push(line.as_ref().unwrap().to_string());
    }

    println!("part1: {}", part1(&input));
    println!("part2: {}", part2(&input));
}
