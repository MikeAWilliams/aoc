use std::fs;

fn main() {
    let input = get_input_from_file("example_input.txt");
    //let input = get_input_from_file("problem_input.txt");

    println!("Problem 1 {}", solve_problem_one(&input));
}

fn solve_problem_one(input: &Vec<String>) -> i32 {
    0
}

fn solve_problem_two(input: &Vec<String>) -> i32 {
    0
}

fn get_lines_from_file(file: &str) -> Vec<String> {
    let file_contents = fs::read_to_string(file).unwrap();
    file_contents
        .split(|c| c == '\n')
        .map(|s| s.to_string())
        .collect()
}

fn get_input_from_file(file: &str) -> Vec<String> {
    get_lines_from_file(file)
}

#[cfg(test)]
mod tests {

    use super::*;
    #[test]
    fn example_input_works() {
        let input = get_input_from_file("example_input.txt");
        assert_eq!(48381165, solve_problem_one(&input));
        assert_eq!(0, solve_problem_two(&input));
    }

    #[test]
    fn problem_input_works() {
        let input = get_input_from_file("problem_input.txt");
        assert_eq!(0, solve_problem_one(&input));
        assert_eq!(0, solve_problem_two(&input));
    }
}
