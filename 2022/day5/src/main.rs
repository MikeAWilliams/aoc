use std::{collections::VecDeque, fs};

fn main() {
    let input = get_input_from_file("example_input.txt", 3);
    //let input = get_input_from_file("problem_input.txt", 9);

    println!("Problem 1 {}", solve_problem_one(&input));
}

fn solve_problem_one(input: &ProblemData) -> String {
    "ABC".to_string()
}

fn solve_problem_two(input: &ProblemData) -> i32 {
    0
}

fn get_lines_from_file(file: &str) -> Vec<String> {
    let file_contents = fs::read_to_string(file).unwrap();
    file_contents
        .split(|c| c == '\n')
        .map(|s| s.to_string())
        .collect()
}

struct ProblemData {
    pub stacks: Vec<VecDeque<char>>,
    pub instructions: Vec<String>,
}

fn get_input_from_file(file: &str, stack_count: i32) -> ProblemData {
    let lines = get_lines_from_file(file);
    let mut result = ProblemData {
        stacks: Vec::new(),
        instructions: Vec::new(),
    };
    for i in 0..stack_count {
        result.stacks.push(VecDeque::new());
    }

    // build the initial stacks
    let mut input_index = 0;
    for line in &lines {
        if None == line.find("[") {
            break;
        }

        for stack_index in 0..stack_count {
            let char_index = (stack_index * 4) + 1;
            let stack_char = line.as_bytes()[char_index as usize] as char;
            if ' ' == stack_char {
                continue;
            }
            //use push_back here so first thing pushed will end up on front
            result.stacks[stack_index as usize].push_back(stack_char);
        }

        input_index += 1;
    }
    // copy out the instructions for later
    result.instructions = lines[(input_index + 2)..].to_vec();

    result
}

#[cfg(test)]
mod tests {

    use super::*;
    #[test]
    fn example_input_works() {
        let input = get_input_from_file("example_input.txt", 3);
        assert_eq!("CMZ", solve_problem_one(&input));
        assert_eq!(4, solve_problem_two(&input));
    }

    #[test]
    fn problem_input_works() {
        let input = get_input_from_file("problem_input.txt", 9);
        assert_eq!("CMZ", solve_problem_one(&input));
        assert_eq!(857, solve_problem_two(&input));
    }
}
