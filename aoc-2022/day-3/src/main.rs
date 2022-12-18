use std::io::{self, BufRead};

fn char_to_pos(c: char) -> usize {
    if c >= 'a' && c <= 'z' {
        return c as usize - 'a' as usize;
    }
    return 26 + c as usize - 'A' as usize;
}

fn part1(input: Vec<String>) -> usize {
    let mut weights_sum = 0;

    for line in input {
        let mut charset: [bool; 52] = [false; 52];
        let mid = line.len() / 2;

        for (i, c) in line.to_string().chars().enumerate() {
            if i < mid {
                charset[char_to_pos(c)] = true;
            } else {
                if charset[char_to_pos(c)] {
                    weights_sum += char_to_pos(c) + 1;
                    break;
                }
            }
        }
    }

    return weights_sum;
}

fn part2(input: Vec<String>) -> usize {
    let mut weights_sum = 0;

    let mut i = 0;
    let mut charset: [i32; 52] = [0; 52];
    for line in input {
        i += 1;
        let mut localset: [i32; 52] = [0; 52];
        for c in line.to_string().chars() {
            localset[char_to_pos(c)] = 1;
        }
        for j in 0..charset.len() {
            charset[j] += localset[j];
        }
        if i == 3 {
            for (i, c) in charset.into_iter().enumerate() {
                if c == 3 {
                    weights_sum += i + 1;
                    break;
                }
            }
            i = 0;
            charset = [0; 52];
        }
    }

    return weights_sum;
}

fn main() {
    let mut lines = io::stdin().lock().lines();
    let mut input: Vec<String> = Vec::new();
    while let Some(line) = lines.next() {
        input.push(line.as_ref().unwrap().to_string());
    }

    println!("p1 {}", part1(input.clone()));
    println!("p2 {}", part2(input));
}
