use std::io::{self, BufRead};
use std::collections::HashMap;

#[derive(Debug)]
#[derive(Clone)]
struct Rock {
    x: i64,
    y: i64,
    shape: Vec<Vec<i64>>,
}

fn do_they_collide(r2: &Rock, stack: &Rock) -> bool {
    for (dy, _) in r2.shape.iter().enumerate() {
        for (dx, _) in r2.shape[dy].iter().enumerate() {
            if r2.shape[dy][dx] == 0 {
                continue;
            }
            let x = dx + r2.x as usize;
            let y = dy + r2.y as usize;
            let sy = y - (stack.y as usize);
            if sy >= stack.shape.len() {
                continue;
            }
            if stack.shape[sy][x] != 0 {
                return true;
            }
        }
    }
    return false;
}

fn can_move_side(diff : i64, rock: &Rock, stack: &Rock) -> bool {
    let mut r2 = rock.clone();
    r2.x += diff;
    if r2.x < 0 {
        return false;
    }
    if r2.x + r2.shape[0].len() as i64 > stack.x + stack.shape[0].len() as i64 {
        return false;
    }
    return !do_they_collide(&r2, &stack);
}

fn can_move_down(rock: &Rock, stack: &Rock) -> bool {
    let mut r2 = rock.clone();
    r2.y -= 1;
    return !do_they_collide(&r2, &stack);
}

fn add_pixel(stack: &mut Rock, dy: usize, dx: usize) {
    while dy >= stack.shape.len() + stack.y as usize {
        stack.shape.push(vec![0,0,0,0,0,0,0]);
    }
    stack.shape[dy-stack.y as usize][dx] = 1;
}

fn print_rock(rock: &Rock) {
    for row in rock.shape.iter().rev() {
        print!(".");
        for point in row {
            print!("{}", if *point == 1 { '#' } else {'.'});
        }
        println!(".");
    }
}

fn part1(input: Vec<char>, rocks_count: i64) {
    let rock_shapes = vec![
        Rock{
            x: 0, y: 0,
            shape: vec![
                vec![1, 1, 1, 1],
            ],
        },
        Rock{
            x: 0, y: 0,
            shape: vec![
                vec![0, 1, 0],
                vec![1, 1, 1],
                vec![0, 1, 0],
            ],
        },
        Rock{
            x: 0, y: 0,
            shape: vec![
                vec![1, 1, 1],
                vec![0, 0, 1],
                vec![0, 0, 1],
            ],
        },
        Rock{
            x: 0, y: 0,
            shape: vec![
                vec![1],
                vec![1],
                vec![1],
                vec![1],
            ],
        },
        Rock{
            x: 0, y: 0,
            shape: vec![
                vec![1, 1],
                vec![1, 1],
            ],
        },
    ];
    
    let mut rocks_fallen = 0;
    let x_start = 3;
    let y_start = 4;
    let mut irock = 0;
    let mut iwind = 0;
    let mut stack = Rock {
        x: 0, y: 0,
        shape: vec![vec![1, 1, 1, 1, 1, 1, 1]],
    };
    let mut rock = rock_shapes[irock].clone();
    rock.x = x_start - 1;
    rock.y = y_start - 1 + stack.shape.len() as i64;

    let mut memory = HashMap::new();

    while rocks_fallen < rocks_count {
        let wind_side = input[iwind];
        iwind = (iwind + 1) % input.len();

        match wind_side {
            '>' => {
                if can_move_side(1, &rock, &stack) {
                    // push >
                    rock.x += 1;
                }
            },
            '<' => {
                if can_move_side(-1, &rock, &stack) {
                    // push <
                    rock.x -= 1;
                }
            },
            _ => {
                println!("BUGGGG");
                todo!()
            }
        };

        // fall 1 unit down
        if can_move_down(&rock, &stack) {
            rock.y -= 1;
            continue;
        }

        /* merge rock into stack*/
        for (dy, _) in rock.shape.iter().enumerate() {
            for (dx, _) in rock.shape[dy].iter().enumerate() {
                if rock.shape[dy][dx] == 1 {
                    add_pixel(&mut stack, dy + rock.y as usize, dx + rock.x as usize);
                }
            }
        }

        while stack.shape.len() > 100 {
            stack.y += 1;
            stack.shape.remove(0);
        }

        /* rock has fallen */
        irock += 1;
        rocks_fallen += 1;

        // try skip
        match memory.get(&(irock % rock_shapes.len(), iwind, stack.shape.clone())) {
            Some((mrock, mfallen, my)) => {
                let drock = irock - mrock;
                let dfallen = rocks_fallen - mfallen;
                let dy = stack.y - my;
                let skip_count : usize = (rocks_count as usize - rocks_fallen as usize) / dfallen as usize;
                if skip_count > 0 {
                    rocks_fallen += (skip_count as i64)*dfallen;
                    irock += skip_count*drock;
                    stack.y += (skip_count as i64)*dy;
                }
            },
            None => {
                memory.insert((irock % rock_shapes.len(), iwind, stack.shape.clone()), (irock, rocks_fallen, stack.y));
            }
        };

        rock = rock_shapes[irock % rock_shapes.len()].clone();
        rock.x = x_start - 1;
        rock.y = stack.y + (stack.shape.len() as i64) + y_start - 1;

        // print_rock(&stack);
        // println!("");
        // println!("new rock {:?}", rock);
    }

    // print_rock(&stack);
    println!("rocks fallen: {}", rocks_fallen);
    println!("tower height: {}", stack.y as usize + stack.shape.len() - 1);
}

fn main() {
    let stdin = io::stdin();
    let mut input = Vec::<String>::new();
    for line in stdin.lock().lines() {
        input.push(line.unwrap());
    }

    let chars = input[0].chars().collect::<Vec<_>>();
    // part1(chars, 2022);
    part1(chars, 1000000000000);
    // part1(chars, 1);
    // part1(chars, 2);
    // part1(chars, 3);
    // part1(chars, 4);
    // part1(chars, 5);
    // part1(chars, 6);
    // part1(chars, 10);
    // part1(chars, 22);
}
