use sscanf::*;
use std::collections::HashMap;
use std::io::{self, BufRead};
use strfmt::strfmt;

#[derive(Debug, Eq, Hash, PartialEq, Copy, Clone)]
struct Position {
    x: i32,
    y: i32,
}

fn part12(input: &Vec<String>, length: i32) {
    let mut rope = Vec::<Position>::new();
    for i in 0..length {
        rope.push(Position { x: 0, y: 0 });
    }
    let mut visited = HashMap::<Position, bool>::new();
    visited.insert(rope[0].clone(), true);
    for line in input {
        let mut head_dest = rope[0].clone();
        // println!("{}", line);
        match sscanf!(line, "{char} {i32}") {
            Ok((direction, steps)) => match direction {
                'R' => {
                    head_dest.x += steps;
                }
                'U' => {
                    head_dest.y += steps;
                }
                'L' => {
                    head_dest.x -= steps;
                }
                'D' => {
                    head_dest.y -= steps;
                }
                _ => {}
            },
            Err(MatchFailed) => {}
        };
        while rope[0] != head_dest {
            if head_dest.x != rope[0].x {
                let sign = if head_dest.x > rope[0].x { 1 } else { -1 };
                rope[0].x += sign;
            } else {
                let sign = if head_dest.y > rope[0].y { 1 } else { -1 };
                rope[0].y += sign;
            }
            for j in 1..rope.len() {
                if rope[j].x == rope[j - 1].x || rope[j].y == rope[j - 1].y {
                    if (rope[j].x - rope[j - 1].x).abs() > 1 {
                        let sign = if rope[j - 1].x > rope[j].x { 1 } else { -1 };
                        rope[j].x += sign;
                    } else if (rope[j].y - rope[j - 1].y).abs() > 1 {
                        let sign = if rope[j - 1].y > rope[j].y { 1 } else { -1 };
                        rope[j].y += sign;
                    }
                } else if (rope[j].x - rope[j - 1].x).abs() > 1 || (rope[j].y - rope[j - 1].y).abs() > 1 {
                    let sign = if rope[j - 1].x > rope[j].x { 1 } else { -1 };
                    rope[j].x += sign;
                    let sign = if rope[j - 1].y > rope[j].y { 1 } else { -1 };
                    rope[j].y += sign;
                }
                if j == rope.len() - 1 {
                    // println!("tail at {:?}", rope[j]);
                    visited.insert(rope[j].clone(), true);
                }
            }
        }
    }
    println!("tail positions: {}", visited.keys().len());
}

fn main() {
    let mut lines = io::stdin().lock().lines();
    let mut input: Vec<String> = Vec::new();
    while let Some(line) = lines.next() {
        input.push(line.as_ref().unwrap().to_string());
    }

    part12(&input, 2);
    part12(&input, 10);
}
