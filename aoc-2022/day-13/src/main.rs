use std::io::{self, BufRead};


fn part1(input: &Vec<String>) {
    let mut right_order_pairs_idx_sum = 0;
    let mut i : usize = 0;
    while i < input.len() {
        let p1 = input[i];
        let p2 = input[i+1];

        
        let mut is_right_order = false;
        // todo
        if is_right_order {
            right_order_pairs_idx_sum += i/3 + 1;
        }
        i += 3;
    }
}

fn main() {
    let mut lines = io::stdin().lock().lines();
    let mut input: Vec<String> = Vec::new();
    while let Some(line) = lines.next() {
        input.push(line.as_ref().unwrap().to_string());
    }
    part1(&lines);
}
