mod inputs;

fn main() {
    let i1 = inputs::in1();
    let i2 = inputs::in2();

    // let input = i1;
    let input = i2;
    let mut s = 0;
    let mut max_s = 0;
    let mut all_sums = Vec::new();
    for v in &input {
        if *v > 0 {
            s += *v;
        } else {
            all_sums.push(s);
            if s > max_s {
                max_s = s;
            }
            s = 0;
        }
    }
    all_sums.sort();  
    println!("max_s = {max_s}");
    let mut i = 0;
    let mut sum3 = 0;
    loop {
        sum3 += all_sums.get(all_sums.len() - i - 1).unwrap();
        i += 1;
        if i > 2 {
            break;
        }
    }
    println!("sum(-3..-1): {}", sum3);
}
