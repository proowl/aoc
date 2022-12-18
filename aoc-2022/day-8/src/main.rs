mod inputs;

fn part1(input: &Vec<Vec<i32>>) {
    let mut visibility_map: Vec<Vec<bool>> = Vec::<Vec<bool>>::new();
    for i in 0..input.len() {
        visibility_map.push(Vec::<bool>::new());
        for j in 0..input[i].len() {
            visibility_map[i]
                .push(j == 0 || j == input[i].len() - 1 || i == 0 || i == input.len() - 1);
        }
    }

    // horizontal
    for i in 1..input.len() {
        {
            // ->
            let mut max_height = input[i][0];
            for j in 1..input[i].len() {
                if input[i][j] > max_height {
                    visibility_map[i][j] = true;
                    max_height = input[i][j];
                }
            }
        }
        {
            // <-
            let mut max_height = input[i][input[i].len() - 1];
            for kj in 0..input[i].len() {
                let j = input[i].len() - 1 - kj;
                if input[i][j] > max_height {
                    visibility_map[i][j] = true;
                    max_height = input[i][j];
                }
            }
        }
    }

    // vertical
    for j in 1..input[0].len() {
        {
            // ->
            let mut max_height = input[0][j];
            for i in 1..input.len() {
                if input[i][j] > max_height {
                    visibility_map[i][j] = true;
                    max_height = input[i][j];
                }
            }
        }
        {
            // <-
            let mut max_height = input[input.len() - 1][j];
            for ki in 0..input.len() {
                let i = input.len() - 1 - ki;
                if input[i][j] > max_height {
                    visibility_map[i][j] = true;
                    max_height = input[i][j];
                }
            }
        }
    }

    for i in 0..input.len() {
        for j in 0..input[i].len() {
            print!("{}", input[i][j]);
        }
        println!("");
    }
    println!("");
    let mut total_visible = 0;
    for i in 0..input.len() {
        for j in 0..input[i].len() {
            let mut v = 0;
            if visibility_map[i][j] {
                v = 1;
                total_visible += 1;
            }
            print!("{}", v);
        }
        println!("");
    }
    println!("total visible {}", total_visible);
}

fn compute_score(input: &Vec<Vec<i32>>, i: usize, j: usize) -> i32{
    let mut score = 1;
    {
        // to the left <-
        let mut subscore = 0;
        let mut max_height = input[i][j];
        for kj in 0..j {
            let k = j - kj - 1;
            subscore += 1;
            if input[i][k] >= input[i][j] {
                break;
            }
        }
        score *= subscore;
    }
    {
        // to the right ->
        let mut subscore = 0;
        let mut max_height = input[i][j];
        for kj in j+1..input[i].len() {
            let k = kj;
            subscore += 1;
            if input[i][k] >= input[i][j] {
                break;
            }
        }
        score *= subscore;
    }
    {
        // to the up
        let mut subscore = 0;
        let mut max_height = input[i][j];
        for ki in 0..i {
            let k = i - ki - 1;
            subscore += 1;
            if input[k][j] >= input[i][j] {
                break;
            }
        }
        score *= subscore;
    }
    {
        // to the down
        let mut subscore = 0;
        let mut max_height = input[i][j];
        for ki in i+1..input.len() {
            let k = ki;
            subscore += 1;
            if input[k][j] >= input[i][j] {
                break;
            }
        }
        score *= subscore;
    }

    return score;
}

fn part2(input: &Vec<Vec<i32>>) {
    let mut max_scenic_score = 0;
    for i in 1..input.len() {
        for j in 1..input[i].len() {
            let tree_score = compute_score(input, i, j);
            if tree_score > max_scenic_score {
                max_scenic_score = tree_score;
            }
        }
    }

    println!("max scenic score {}", max_scenic_score);
}

fn main() {
    part1(&inputs::in1());
    part1(&inputs::in2());
    part2(&inputs::in1());
    part2(&inputs::in2());
}
