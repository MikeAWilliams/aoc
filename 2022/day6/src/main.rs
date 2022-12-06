use std::{collections::HashSet, fs};
use substring::Substring;

fn main() {
    //let input = get_input_from_file("example_input.txt");
    let input = get_input_from_file("problem_input.txt");

    println!("Problem 1 {}", solve_problem_one(&input));
}

fn all_unique(input: &str) -> bool {
    let set: HashSet<char> = input.chars().into_iter().collect();
    set.len() == input.len()
}

fn find_end_of_first_unique_substr_of_size(input: &str, size: usize) -> i32 {
    for start in 0..(input.len() - (size + 1)) {
        let candidate = input.substring(start, start + size);
        if all_unique(candidate) {
            return (start + size) as i32;
        }
    }
    -1
}

fn solve_problem_one(input: &str) -> i32 {
    find_end_of_first_unique_substr_of_size(input, 4)
}

fn solve_problem_two(input: &str) -> i32 {
    find_end_of_first_unique_substr_of_size(input, 14)
}

fn get_lines_from_file(file: &str) -> Vec<String> {
    let file_contents = fs::read_to_string(file).unwrap();
    file_contents
        .split(|c| c == '\n')
        .map(|s| s.to_string())
        .collect()
}

fn get_input_from_file(file: &str) -> String {
    get_lines_from_file(file).remove(0)
}

#[cfg(test)]
mod tests {

    use super::*;
    #[test]
    fn example_input_works() {
        assert_eq!(7, solve_problem_one("mjqjpqmgbljsphdztnvjfqwrcgsmlb"));
        assert_eq!(5, solve_problem_one("bvwbjplbgvbhsrlpgdmjqwftvncz"));
        assert_eq!(6, solve_problem_one("nppdvjthqldpwncqszvftbrmjlhg"));
        assert_eq!(10, solve_problem_one("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg"));
        assert_eq!(11, solve_problem_one("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw"));

        assert_eq!(19, solve_problem_two("mjqjpqmgbljsphdztnvjfqwrcgsmlb"));
        assert_eq!(23, solve_problem_two("bvwbjplbgvbhsrlpgdmjqwftvncz"));
        assert_eq!(23, solve_problem_two("nppdvjthqldpwncqszvftbrmjlhg"));
        assert_eq!(29, solve_problem_two("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg"));
        assert_eq!(26, solve_problem_two("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw"));
    }

    #[test]
    fn problem_input_works() {
        let input = get_input_from_file("problem_input.txt");
        assert_eq!(1361, solve_problem_one(&input));
        assert_eq!(3263, solve_problem_two(&input));
    }
}
