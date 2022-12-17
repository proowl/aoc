mod inputs;

use crate::inputs::Monkey;

fn part12(input: &Vec<Monkey>, relief_enabled: bool, rounds: i64) {
    let mut monkeys = input.clone();

    let mut inspected_by_monkey: Vec<i64> = vec![];
    inspected_by_monkey.resize_with(monkeys.len(), || 0);

    let mut relief_divider: i64 = 1;
    for m in &monkeys {
        relief_divider *= m.testa_is_div;
    }

    let mut round = 0;
    while round < rounds {
        for i in 0..monkeys.len() {
            for j in 0..monkeys[i].items.len() {
                let mut worry = monkeys[i].items[j];
                // inspect
                inspected_by_monkey[i] += 1;
                match monkeys[i].operation {
                    '*' => worry *= monkeys[i].operand,
                    '+' => worry += monkeys[i].operand,
                    '^' => worry = worry * worry,
                    _ => todo!(),
                }
                // relief
                if relief_enabled {
                    worry /= 3;
                } else {
                    worry = worry % relief_divider;
                }
                // test
                let dest_monkey = if worry % monkeys[i].testa_is_div == 0 {
                    monkeys[i].next_monkey_a
                } else {
                    monkeys[i].next_monkey_b
                };
                monkeys[dest_monkey as usize].items.push(worry);
            }
            monkeys[i].items.clear();
        }
        round += 1;
    }

    inspected_by_monkey.sort();
    println!("inspected: {:?}", inspected_by_monkey);
    let mbusiness = inspected_by_monkey[inspected_by_monkey.len() - 1]
        * inspected_by_monkey[inspected_by_monkey.len() - 2];
    println!("monkey business: {}", mbusiness);
}

fn main() {
    part12(&inputs::in1(), true, 20);
    part12(&inputs::in2(), true, 20);
    part12(&inputs::in1(), false, 10000 );
    part12(&inputs::in2(), false, 10000);
}
