use std::fs;
fn main() {
    let input = get_input_from_file("example_input.txt");
    //let input = get_input_from_file("problem_input.txt");

    println!("Problem 1 {}", solve_problem_one(&input));
    println!("Problem 2 {}", solve_problem_two(&input));
}

fn elf_pair_is_redundant(first_start:i32, first_stop:i32, second_start:i32, second_stop:i32)->bool{
    (first_start >= second_start && first_stop <= second_stop) ||
    (second_start >= first_start && second_stop <= first_stop)
}

fn solve_problem_one(input: &Vec<Vec<Vec<i32>>>) -> i32 {
    input.iter().map(|elf_pair|{
    let first = &elf_pair[0];
    let first_start = first[0];
    let first_stop = first[1];

    let second = &elf_pair[1];
    let second_start = second[0];
    let second_stop = second[1];
    if elf_pair_is_redundant(first_start, first_stop, second_start, second_stop){
        1
    } else {
        0
    }
   }).sum()
}

fn solve_problem_two(_input: &Vec<Vec<Vec<i32>>>) -> i32 {
    0
}

fn get_input_from_file(file: &str) -> Vec<Vec<Vec<i32>>> {
    let file_contents = fs::read_to_string(file).unwrap();
    file_contents
        .split(|c| c == '\n')
        .map(|line| {
            line.split(|c| c == ',')
                .map(|substr| 
                    substr.split(|c| c=='-')
                        .map(|str|str.parse::<i32>().unwrap()).collect::<Vec<i32>>())
                        .collect::<Vec<Vec<i32>>>()})
                    .collect::<Vec<Vec<Vec<i32>>>>()
}

#[cfg(test)]
mod tests {

    use super::*;
    #[test]
    fn example_input_works() {
        let input = get_input_from_file("example_input.txt");
        assert_eq!(2, solve_problem_one(&input));
        assert_eq!(0, solve_problem_two(&input));
    }

    #[test]
    fn problem_input_works() {
        let input = get_input_from_file("problem_input.txt");
        assert_eq!(485, solve_problem_one(&input));
        assert_eq!(0, solve_problem_two(&input));
    }
}