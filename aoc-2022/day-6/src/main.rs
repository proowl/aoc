mod inputs;

use std::collections::HashMap;

fn part1(input: &str, length : usize) {
    let mut seen_chars = HashMap::<char, i32>::new();
    for (i, c) in input.chars().into_iter().enumerate() {
        if i >= length {
            let prev_c = input.chars().nth(i - length).unwrap();
            match seen_chars.get(&prev_c) {
                Some(count) => {
                    if *count > 1 {
                        seen_chars.insert(prev_c, count - 1);
                    } else {
                        seen_chars.remove(&prev_c);
                    }
                }
                None => {}
            };
        }
        match seen_chars.get(&c) {
            Some(count) => seen_chars.insert(c, count + 1),
            None => seen_chars.insert(c, 1),
        };
        if seen_chars.len() == length {
            println!("len {} at {}", length, i+1);
            break;
        }
    }
}

fn main() {
    part1(inputs::in1(), 4);
    part1(inputs::in2(), 4);
    part1(inputs::in3(), 4);
    part1(inputs::in4(), 4);
    part1(inputs::in5(), 4);
    part1(inputs::in_mine(), 4);

    part1(inputs::in1(), 14);
    part1(inputs::in2(), 14);
    part1(inputs::in3(), 14);
    part1(inputs::in4(), 14);
    part1(inputs::in5(), 14);
    part1(inputs::in_mine(), 14);
}
