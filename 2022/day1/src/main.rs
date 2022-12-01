use std::fs;

fn main() {
    //let input = get_input_from_file("example_input.txt");
    let input = get_input_from_file("problem_input.txt");
    println!("There are {} elves", count_elves(input.as_slice()));
    let elf_with_calories = sum_elf_calories_by_elf(input.as_slice());
    for elf in elf_with_calories {
        println!("This elf has {} callories", elf)
    }
    println!(
        "The elf with the most callories has {} callories",
        get_elf_max_callorie(input.as_slice())
    );
    println!(
        "The callories carried by the top three elves are {} callories",
        get_top_n_callories(input.as_slice(), 3)
    );
}

fn get_example_input() -> Vec<i32> {
    vec![
        1000, 2000, 3000, 0, 4000, 0, 5000, 6000, 0, 7000, 8000, 9000, 0, 10000,
    ]
}

fn get_input_from_file(file: &str) -> Vec<i32> {
    let file_contents = fs::read_to_string(file).unwrap();
    file_contents
        .split(|character| character == '\n')
        .map(|item| item.parse::<i32>().unwrap_or(0))
        .collect::<Vec<i32>>()
}

fn count_elves(elf_calory_count: &[i32]) -> usize {
    elf_calory_count.split(|num| *num == 0_i32).count()
}

fn sum_elf_calories_by_elf(elf_calory_count: &[i32]) -> Vec<i32> {
    elf_calory_count
        .split(|num| *num == 0_i32)
        .map(|item| item.iter().sum())
        .collect()
}

fn get_elf_max_callorie(elf_calory_count: &[i32]) -> i32 {
    elf_calory_count
        .split(|num| *num == 0_i32)
        .map(|item| item.iter().sum())
        .max()
        .unwrap()
}

fn get_top_n_callories(elf_calory_count: &[i32], elves_to_count: i32) -> i32 {
    let mut elf_sum = elf_calory_count
        .split(|num| *num == 0_i32)
        .map(|item| item.iter().sum())
        .collect::<Vec<i32>>();
    elf_sum.sort_by(|first, second| second.cmp(first));
    elf_sum.iter().take(elves_to_count as usize).sum()
}

#[cfg(test)]
mod tests {

    use super::*;
    #[test]
    fn example_input_works() {
        let input = get_input_from_file("example_input.txt");
        assert_eq!(24000, get_elf_max_callorie(input.as_slice()));
        assert_eq!(45000, get_top_n_callories(input.as_slice(), 3))
    }

    #[test]
    fn real_input_works() {
        let input = get_input_from_file("problem_input.txt");
        assert_eq!(66186, get_elf_max_callorie(input.as_slice()));
        assert_eq!(196804, get_top_n_callories(input.as_slice(), 3))
    }
}
