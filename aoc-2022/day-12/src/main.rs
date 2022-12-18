use priority_queue::PriorityQueue;

mod input;

#[derive(Debug, Clone, Copy)]
#[derive(Eq, Hash, PartialEq)]
struct Position {
    i: i32,
    j: i32,
}

fn find_pos(input: &Vec<Vec<char>>, token : char) -> Position {
    for i in 0..input.len() {
        for j in 0..input[i].len() {
            if input[i][j] == token {
                return Position{i: i as i32, j: j as i32};
            }
        }
    }
    return Position{i: -1, j: -1};
}

fn step_cost(from: char, to: char) -> i32 {
    let ufrom = match from {
        'S' => 'a',
        'E' => 'z',
        other => other
    };
    let uto = match to {
        'S' => 'a',
        'E' => 'z',
        other => other
    };
    return uto as i32 - ufrom as i32; 
}

fn estimated(from: Position, to: Position) -> i32 {
    return (from.i - to.i).abs() + (from.j - to.j).abs();
}

// A*
fn find_shortest_path(input: &Vec<Vec<char>>, start_pos: Position, end_pos: Position) -> i32 {
    let mut open_set = PriorityQueue::new();
    open_set.push(start_pos, std::i32::MAX);

    let mut gScore = Vec::<Vec<i32>>::new();
    for i in 0..input.len() {
        gScore.push(vec![]);
        for j in 0..input[i].len() {
            gScore[i].push(std::i32::MAX);
        }
    }
    gScore[start_pos.i as usize][start_pos.j as usize] = 0;

    let mut fScore = Vec::<Vec<i32>>::new();
    for i in 0..input.len() {
        fScore.push(vec![]);
        for j in 0..input[i].len() {
            fScore[i].push(std::i32::MAX);
        }
    }
    fScore[start_pos.i as usize][start_pos.j as usize] = estimated(start_pos, end_pos);

    while !open_set.is_empty() {
        let current = open_set.pop();
        match current {
            Some((cur_pos, _)) => {
                // println!("at: {:?}", cur_pos);
                if cur_pos == end_pos {
                    return gScore[end_pos.i as usize][end_pos.j as usize];
                }
                for step in vec![
                    Position{i: -1, j: 0}, Position{i: 0, j: 1}, Position{i: 1, j: 0}, Position{i: 0, j: -1}
                ] {
                    let next_step = Position {
                        i: cur_pos.i + step.i,
                        j: cur_pos.j + step.j
                    };
                    // println!("next: {:?}", next_step);
        
                    if next_step.i < 0 || (next_step.i as usize) >= input.len()
                        || next_step.j < 0 || (next_step.j as usize) >= input[next_step.i as usize].len() {
                        continue;
                    }
                    if step_cost(input[cur_pos.i as usize][cur_pos.j as usize], input[next_step.i as usize][next_step.j as usize]) > 1 {
                        continue;
                    }
                    // println!("next valid: {:?}", next_step);
                    
                    let tentative_gScore = gScore[cur_pos.i as usize][cur_pos.j as usize] + 1;
                    if tentative_gScore < gScore[next_step.i as usize][next_step.j as usize] {
                        gScore[next_step.i as usize][next_step.j as usize] = tentative_gScore;
                        fScore[next_step.i as usize][next_step.j as usize] = tentative_gScore + estimated(next_step, end_pos);
                        match open_set.get(&next_step) {
                            Some(_) => {},
                            None => {
                                open_set.push(next_step, std::i32::MAX - fScore[next_step.i as usize][next_step.j as usize]);
                            },
                        };
                    }
                }
            },
            None => {}
        };

    }

    return std::i32::MAX;
}

fn part1(input: &Vec<Vec<char>>) {
    let start_pos: Position = find_pos(input, 'S');
    let end_pos: Position = find_pos(input, 'E');
    let min_path_len = find_shortest_path(input, start_pos, end_pos);
    println!("shortest S->E path: {}", min_path_len);
}

fn part2(input: &Vec<Vec<char>>) {
    let end_pos: Position = find_pos(input, 'E');
    let mut min_path_len = std::i32::MAX;
    for i in 0..input.len() {
        for j in 0..input[i].len() {
            match input[i][j] {
                'S' | 'a' => {
                    let start_pos = Position{i: i as i32, j: j as i32};
                    let path_len = find_shortest_path(input, start_pos, end_pos);
                    if min_path_len > path_len {
                        min_path_len = path_len;
                    }
                },
                _ => {},
            };
        }
    }
    println!("shortest S|a->E path: {}", min_path_len);
}

fn main() {
    part1(&input::in1());
    part1(&input::in2());
    part2(&input::in1());
    part2(&input::in2());
}
