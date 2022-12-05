use std::fs;
fn main() {
    let input = get_input_from_file("example_input.txt");
    //let input = get_input_from_file("problem_input.txt");

    println!("Problem 1 {}", solve_problem_one(&input));
    println!("Problem 2 {}", solve_problem_two(&input));
}

fn elf_pair_is_redundant(first:&WorkIndices, second:&WorkIndices)->bool{
    (first.start >= second.start && first.stop <= second.stop) ||
    (second.start >= first.start && second.stop <= first.stop)
}

fn elf_pair_overlaps(first:&WorkIndices, second:&WorkIndices)->bool{
    elf_pair_is_redundant(first, second)||(
        (first.stop >= second.start && first.stop <= second.stop) ||
        (second.stop >= first.start && second.stop <= first.stop)
    )
}

struct WorkIndices{
    pub start:i32,
    pub stop:i32,
}

fn score_elf_par(work_indicies:&Vec<WorkIndices>, predicate: fn(&WorkIndices,&WorkIndices)->bool)->i32{
        match predicate(&work_indicies[0], &work_indicies[1]){
            true => 1,
            _ => 0,
        }
}

fn solve_problem_one(input: &Vec<Vec<WorkIndices>>) -> i32 {
    input.iter().map(|elf_pair|{
        score_elf_par(elf_pair, elf_pair_is_redundant)
   }).sum()
}

fn solve_problem_two(input: &Vec<Vec<WorkIndices>>) -> i32 {
    input.iter().map(|elf_pair|{
        score_elf_par(elf_pair, elf_pair_overlaps)
    }).sum()
}

fn get_work_indices_from_str(input: &str)->WorkIndices{
    input.split_once('-')
        .map(|(start_str, end_str)|{
            WorkIndices { start: start_str.parse::<i32>().unwrap(), stop: end_str.parse::<i32>().unwrap() }
        }).unwrap()
}


fn get_lines_from_file(file: &str) -> Vec<String> {
    let file_contents = fs::read_to_string(file).unwrap();
    file_contents.split(|c| c=='\n')
    .map(|s| s.to_string())
    .collect()
}

fn get_input_from_file(file: &str) -> Vec<Vec<WorkIndices>> {
    let lines = get_lines_from_file(file);
    lines.iter()
        .map(|line| {
            line.split(|c| c == ',')
            .map(|index_pair_str| get_work_indices_from_str(index_pair_str))
            .collect()}).collect()

}

#[cfg(test)]
mod tests {

    use super::*;
    #[test]
    fn example_input_works() {
        let input = get_input_from_file("example_input.txt");
        assert_eq!(2, solve_problem_one(&input));
        assert_eq!(4, solve_problem_two(&input));
    }

    #[test]
    fn problem_input_works() {
        let input = get_input_from_file("problem_input.txt");
        assert_eq!(485, solve_problem_one(&input));
        assert_eq!(857, solve_problem_two(&input));
    }
}