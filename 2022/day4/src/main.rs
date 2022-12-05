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

fn score_elf_par(pair_as_vec:&Vec<Vec<i32>>, predicate: fn(&WorkIndices,&WorkIndices)->bool)->i32{
        let first = get_work_indices(&pair_as_vec[0]);
        let second = get_work_indices(&pair_as_vec[1]);

        match predicate(&first, &second){
            true => 1,
            _ => 0,
        }
}

fn get_work_indices(input:&Vec<i32>)-> WorkIndices{
    WorkIndices { start: input[0], stop: input[1] }
}

fn solve_problem_one(input: &Vec<Vec<Vec<i32>>>) -> i32 {
    input.iter().map(|elf_pair|{
        score_elf_par(elf_pair, elf_pair_is_redundant)
   }).sum()
}

fn solve_problem_two(input: &Vec<Vec<Vec<i32>>>) -> i32 {
    input.iter().map(|elf_pair|{
        score_elf_par(elf_pair, elf_pair_overlaps)
    }).sum()
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
        assert_eq!(4, solve_problem_two(&input));
    }

    #[test]
    fn problem_input_works() {
        let input = get_input_from_file("problem_input.txt");
        assert_eq!(485, solve_problem_one(&input));
        assert_eq!(857, solve_problem_two(&input));
    }
}