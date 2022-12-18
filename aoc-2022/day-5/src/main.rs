mod inputs;

fn part1(crates_in: &Vec<Vec<char>>, moves: &Vec<(usize, usize, usize)>) {
    let mut crates: Vec<Vec<char>> = (*crates_in.clone()).to_vec();

    for (count, from, to) in moves {
        let mut i = 0;
        while i < *count {
            let c = crates[*from - 1].pop().unwrap();
            crates[*to - 1].push(c);
            i += 1;
        }
    }

    for c in crates {
        print!("{}", c[c.len() - 1]);
    }
    println!("");
}

fn part2(crates_in: &Vec<Vec<char>>, moves: &Vec<(usize, usize, usize)>) {
    let mut crates: Vec<Vec<char>> = (*crates_in.clone()).to_vec();

    for (count, from, to) in moves {
        let move_pos = crates[*from-1].len() - *count;
        for i in 0..*count {
            let c = crates[*from - 1][move_pos+i];
            crates[*to - 1].push(c);
        }
        for i in 0..*count {
            crates[*from - 1].pop();
        }
    }

    for c in crates {
        print!("{}", c[c.len() - 1]);
    }
    println!("");
}

fn main() {
    part1(&inputs::crates1(), &inputs::moves1());
    part1(&inputs::crates2(), &inputs::moves2());
    part2(&inputs::crates1(), &inputs::moves1());
    part2(&inputs::crates2(), &inputs::moves2());
}
