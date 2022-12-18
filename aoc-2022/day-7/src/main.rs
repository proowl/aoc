use sscanf::*;
use std::collections::HashMap;
use std::io::{self, BufRead};

fn part12(input: &Vec<String>) {
    let mut dir_sizes = HashMap::<String, i64>::new();
    let mut dirs_stack = Vec::<String>::new();
    let mut i = 0;
    while i < input.len() {
        let line = &input[i];
        let cd_cmd = sscanf!(line, "$ cd {str}");
        let ls_cmd = sscanf!(line, "$ ls");
        i += 1;
        match cd_cmd {
            Ok(dirname) => match dirname {
                ".." => {
                    dirs_stack.pop();
                }
                _ => {
                    dirs_stack.push(dirname.to_string());
                }
            },
            Err(MatchFailed) => {}
        }
        match ls_cmd {
            Ok(()) => {
                println!("scanning {:?}", dirs_stack);
                while i < input.len() {
                    let line = &input[i];
                    if line.chars().nth(0).unwrap() == '$' {
                        break;
                    }
                    match sscanf!(line, "{i64} {str}") {
                        Ok((filesize, filename)) => {
                            let mut prefix = String::new();
                            for f in &dirs_stack {
                                prefix = prefix.to_owned() + "/" + &f;
                                match dir_sizes.get(&prefix.to_string()) {
                                    Some(size) => {
                                        dir_sizes.insert(prefix.to_string(), size + filesize)
                                    }
                                    None => dir_sizes.insert(prefix.to_string(), filesize),
                                };
                            }
                        }
                        Err(MatchFailed) => {}
                    }
                    i += 1;
                }
            }
            Err(MatchFailed) => {}
        }
    }
    let mut count = 0;
    let mut total_size = 0;

    println!("full tree: {:?}", dir_sizes);

    for (d, s) in &dir_sizes {
        if *s <= 100000 {
            count += 1;
            total_size += s;
            println!("{} {} {} -> {}", count, d, s, total_size);
        }
    }

    let fs_size = 70000000;
    let space_to_free = 30000000 - (fs_size - dir_sizes.get("//").unwrap());
    println!("searching to free {}", space_to_free);
    let mut free_dir_size = fs_size;
    for (d, s) in &dir_sizes {
        if *s < free_dir_size && *s >= space_to_free {
            free_dir_size = *s;
        }
    }
    println!("found {}", free_dir_size);
}

fn main() {
    let mut lines = io::stdin().lock().lines();
    let mut input: Vec<String> = Vec::new();
    while let Some(line) = lines.next() {
        input.push(line.as_ref().unwrap().to_string());
    }

    part12(&input);
}
