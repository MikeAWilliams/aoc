use std::{fs, collections::HashSet};
use substring::Substring;

fn main() {
    let input = get_input_from_file("problem_input.txt");
    //let input = get_input_from_file("example_input.txt");
    println!("total priority {}", solve_problem_one(&input));
}

fn get_input_from_file(file: &str) -> Vec<String> {
    fs::read_to_string(file).unwrap().split(|c|'\n' == c).map(|item|item.to_string()).collect()
}

fn solve_problem_one(input: &Vec<String>) -> i32 {
   input.iter().map(|sack|{
        let pocket1 = sack.substring(0, sack.len()/2).chars().collect::<HashSet<char>>();
        let pocket2 = sack.substring(sack.len()/2, sack.len()).chars().collect::<HashSet<char>>();
        let commonChars = pocket1.intersection(&pocket2).collect::<Vec<&char>>();
        //println!("{},{}", commonChars.len(), commonChars[0]);
        commonChars[0].clone()
    }).map(|letter|{
        char_to_priority(letter)
    }).sum()
}

fn char_to_priority(letter:char)->i32{
    let mut result = letter as i32;
    if letter.is_uppercase(){
        result = result - 65 + 27;
    } else {
        result = result - 97 + 1;
    }
    result
}

#[cfg(test)]
mod tests {

    use super::*;
    #[test]
    fn example_input_works() {
        let input = get_input_from_file("example_input.txt");
        assert_eq!(157, solve_problem_one(&input));
        //assert_eq!(12, solve_problem_two(&input));
    }

    //#[test]
    //fn problem_input_works() {
        //let input = get_input_from_file("problem_input.txt");
        //assert_eq!(13484, solve_problem_one(&input));
    //}
}