use std::fs;

fn main() {
    let input = get_input_from_file("example_input.txt");
    //let input = get_input_from_file("problem_input.txt");

    println!("Problem 1 {}", solve_problem_one(&input));
}

fn generate_directory_graph(console: &Vec<String>) -> DirectoryGraph {
    let mut result = DirectoryGraph {
        nodes: vec![],
        parents: vec![],
        dependencies: vec![],
    };
    // start with a / dir
    result.nodes.push(DirectoryNode {
        filesSize: 0,
        totalSize: 0,
        name: "root".to_string(),
    });
    let mut current_node_index = 0;
    let mut input_line: usize = 0;
    while input_line < console.len() {
        input_line += 1;
    }
    result
}

fn solve_problem_one(input: &Vec<String>) -> i32 {
    let mut graph = generate_directory_graph(input);
    0
}

fn solve_problem_two(input: &Vec<String>) -> i32 {
    0
}

struct DirectoryNode {
    pub filesSize: i32,
    pub totalSize: i32,
    pub name: String,
}

struct DirectoryGraph {
    pub nodes: Vec<DirectoryNode>,
    pub parents: Vec<usize>,
    pub dependencies: Vec<Vec<usize>>,
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
