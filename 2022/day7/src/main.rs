use std::collections::HashSet;
use std::fs;
use substring::Substring;

fn main() {
    let input = get_input_from_file("example_input.txt");
    //let input = get_input_from_file("problem_input.txt");

    println!("Problem 1 {}", solve_problem_one(&input));
}

fn add_sub_dir(dir: &str, current_index: i32, graph: &mut DirectoryGraph) {
    let new_index = graph.add_node_if_new(dir.to_string());
    graph.add_dependency_if_new(current_index as usize, new_index);
}

fn process_command_line_default(
    line: &String,
    current_index: i32,
    graph: &mut DirectoryGraph,
) -> i32 {
    //println!("default {}", line);
    if line.starts_with("dir ") {
        let destination = line.substring(4, line.len());
        //println!("its a dir {}", destination);
        add_sub_dir(destination, current_index, graph);
    } else {
        let file_size = line.split_whitespace().next().unwrap();
        graph.nodes[current_index as usize].files_size += file_size.parse::<i32>().unwrap();
    }
    current_index
}

fn process_command_line_cd(new_dir: &str, current_index: i32, graph: &DirectoryGraph) -> i32 {
    //println!("its a cd {}", new_dir);
    if new_dir == ".." {
        graph.parents[current_index as usize] as i32
    } else {
        graph
            .nodes
            .iter()
            .position(|node| node.name == new_dir)
            .unwrap() as i32
    }
}

fn process_command_line(line: &String, current_index: i32, graph: &mut DirectoryGraph) -> i32 {
    return if line.starts_with("$ cd ") {
        let parts = line.split(' ').collect::<Vec<&str>>();
        process_command_line_cd(parts[2], current_index, graph)
    } else if line != "$ ls" {
        process_command_line_default(line, current_index, graph)
    } else {
        current_index
    };
}

fn generate_directory_graph(console: &Vec<String>) -> DirectoryGraph {
    let mut result = DirectoryGraph {
        nodes: vec![],
        parents: vec![],
        dependencies: vec![],
    };
    // start with a / dir
    result.add_node_if_new("root".to_string());
    let mut current_node_index = 0;
    let mut input_line: usize = 1;
    while input_line < console.len() {
        current_node_index =
            process_command_line(&console[input_line], current_node_index, &mut result);
        input_line += 1;
    }
    result
}

fn solve_problem_one(input: &Vec<String>) -> i32 {
    println!("generating graph");
    let mut graph = generate_directory_graph(input);
    graph.print();
    println!("sizing it");
    graph.compute_folder_sizes();
    println!("computing the result");
    graph
        .nodes
        .iter()
        .filter(|node| node.total_size <= 100000)
        .map(|node| node.total_size)
        .sum()
}

fn solve_problem_two(input: &Vec<String>) -> i32 {
    0
}

struct DirectoryNode {
    pub files_size: i32,
    pub total_size: i32,
    pub name: String,
}

struct DirectoryGraph {
    pub nodes: Vec<DirectoryNode>,
    pub parents: Vec<usize>,
    pub dependencies: Vec<Vec<usize>>,
}

impl DirectoryGraph {
    fn add_node_if_new(&mut self, name: String) -> usize {
        let possition = self.nodes.iter().position(|node| node.name == name);
        match possition {
            Some(index) => index,
            None => {
                self.nodes.push(DirectoryNode {
                    files_size: 0,
                    total_size: 0,
                    name: name,
                });
                self.dependencies.push(vec![]);
                self.parents.push(0);
                self.nodes.len() - 1
            }
        }
    }

    fn add_dependency_if_new(&mut self, parent: usize, child: usize) {
        if let None = self.dependencies[parent]
            .iter()
            .position(|item| item == &child)
        {
            self.dependencies[parent].push(child);
            self.parents[child] = parent;
        }
    }

    fn print(&self) {
        println!("Nodes");
        for node in &self.nodes {
            println!("{}, {}, {}", node.name, node.files_size, node.total_size)
        }

        println!("Parents");
        for node_index in 0..self.parents.len() {
            let node_name = &self.nodes[node_index].name;
            let parent_name = &self.nodes[self.parents[node_index]].name;
            println!("node {} has parent {}", node_name, parent_name);
        }

        println!("Children");
        for node_index in 0..self.dependencies.len() {
            let node_name = &self.nodes[node_index].name;
            println!(
                "node {} has children {}",
                node_name,
                self.dependencies[node_index]
                    .iter()
                    .map(|item| item.to_string() + ",")
                    .collect::<String>()
            );
        }
    }

    fn compute_folder_size_recursive(&mut self, index: usize, visited: &mut HashSet<usize>) -> i32 {
        if visited.contains(&index) {
            return self.nodes[index].total_size;
        };
        let mut child_sizes = 0;
        let dependencies_copy = self.dependencies[index].clone();
        for child_index in &dependencies_copy {
            child_sizes += self.compute_folder_size_recursive(*child_index, visited);
        }
        self.nodes[index].total_size = self.nodes[index].files_size + child_sizes;
        visited.insert(index);
        self.nodes[index].total_size
    }

    fn compute_folder_sizes(&mut self) {
        let mut visited: HashSet<usize> = HashSet::new();
        self.compute_folder_size_recursive(0, &mut visited);
    }
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
        assert_eq!(95437, solve_problem_one(&input));
        assert_eq!(0, solve_problem_two(&input));
    }

    #[test]
    fn problem_input_works() {
        let input = get_input_from_file("problem_input.txt");
        assert_eq!(0, solve_problem_one(&input));
        assert_eq!(0, solve_problem_two(&input));
    }
}
