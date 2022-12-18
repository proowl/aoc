mod inputs;

use std::collections::HashMap;
use std::process;

#[derive(Debug)]
#[derive(PartialEq)]
#[derive(Copy, Clone)]
enum ActionId {
    Rock,
    Paper,
    Scissors,
}

fn does_a_win(a : ActionId, b: ActionId) -> bool {
    return a == ActionId::Rock && b == ActionId::Scissors
        || a == ActionId::Paper && b == ActionId::Rock
        || a == ActionId::Scissors && b == ActionId::Paper;
}

fn action_score(a: ActionId) -> i32 {
    match a {
        ActionId::Rock => 1,
        ActionId::Paper => 2,
        ActionId::Scissors => 3,
    }
}

fn lose_to(a: ActionId) -> ActionId {
    match a {
        ActionId::Rock => ActionId::Scissors,
        ActionId::Paper => ActionId::Rock,
        ActionId::Scissors => ActionId::Paper,
    }
}

fn win_to(a: ActionId) -> ActionId {
    match a {
        ActionId::Rock => ActionId::Paper,
        ActionId::Paper => ActionId::Scissors,
        ActionId::Scissors => ActionId::Rock,
    }
}

fn draw_to(a: ActionId) -> ActionId {
    return a
}

fn part1(input: Vec<(char, char)>) -> i32 {
    let mut actions = HashMap::new();
    actions.insert('A', ActionId::Rock);
    actions.insert('X', ActionId::Rock);
    actions.insert('B', ActionId::Paper);
    actions.insert('Y', ActionId::Paper);
    actions.insert('C', ActionId::Scissors);
    actions.insert('Z', ActionId::Scissors);
    let mut score = 0;
    for (cha, chb) in &input {
        let a = *actions.get(cha).unwrap();
        let b = *actions.get(chb).unwrap();
        let a_wins = does_a_win(a, b);
        let b_wins = does_a_win(b, a);
        let mut s = action_score(b);
        if !a_wins && !b_wins {
            s += 3; // draw
        } else if a_wins {
        } else /*b_wins*/ {
            s += 6; // won
        }
        score += s;
    }
    return score;
}

fn part2(input: Vec<(char, char)>) -> i32 {
    let mut actions = HashMap::new();
    actions.insert('A', ActionId::Rock);
    actions.insert('B', ActionId::Paper);
    actions.insert('C', ActionId::Scissors);
    let mut score = 0;
    for (cha, chb) in &input {
        let a = *actions.get(cha).unwrap();
        let b = match chb {
            'X' => lose_to(a),
            'Y' => draw_to(a),
            'Z' => win_to(a),
            _ => process::exit(1),
        };
        let a_wins = does_a_win(a, b);
        let b_wins = does_a_win(b, a);
        let mut s = action_score(b);
        if !a_wins && !b_wins {
            s += 3; // draw
        } else if a_wins {
        } else /*b_wins*/ {
            s += 6; // won
        }
        score += s;
    }
    return score;
}

fn main() {
    println!("p1 i1 {}", part1(inputs::i1()));
    println!("p1 i2 {}", part1(inputs::i2()));
    println!("p2 i1 {}", part2(inputs::i1()));
    println!("p2 i2 {}", part2(inputs::i2()));
}
