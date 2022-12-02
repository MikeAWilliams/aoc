use std::fs;

fn main() {
    //let input = get_input_from_file("example_input.txt");
    let input = get_input_from_file("problem_input.txt");

    println!("Part 1 total points for p2 {}", solve_problem_one(&input));
    println!("Part 2 total points for p2 {}", solve_problem_two(&input));
}

fn solve_problem_two(input: &Vec<Vec<String>>) -> i32 {
    let game_moves = get_game_moves_problem_two(&input);
    let game_results = get_game_results(&game_moves);
    get_total_p2_score(&game_moves, &game_results)
}

fn solve_problem_one(input: &Vec<Vec<String>>) -> i32 {
    let translators = [other_player_translate, my_translate];
    let game_moves = get_game_moves(&input, &translators);
    let game_results = get_game_results(&game_moves);
    get_total_p2_score(&game_moves, &game_results)
}

#[allow(dead_code)]
fn print_raw_data(input: &Vec<Vec<String>>) {
    for vec_pair in input {
        for a_string in vec_pair {
            print!("`{}`, ", a_string);
        }
        print!("\n")
    }
}

#[allow(dead_code)]
fn print_game_moves(input: &Vec<Vec<GameMove>>) {
    for vec_pair in input {
        for tup in vec_pair {
            print!(
                "{}, ",
                match tup {
                    GameMove::Rock => "Rock",
                    GameMove::Paper => "Paper",
                    GameMove::Scisors => "Scisors",
                }
            );
        }
        print!("\n");
    }
}

fn get_input_from_file(file: &str) -> Vec<Vec<String>> {
    let file_contents = fs::read_to_string(file).unwrap();
    file_contents
        .split(|c| c == '\n')
        .map(|line| {
            line.split(|c| c == ' ')
                .map(|item| item.to_string())
                .collect::<Vec<String>>()
        })
        .collect::<Vec<Vec<String>>>()
}

fn get_game_moves_problem_two(input: &Vec<Vec<String>>) -> Vec<Vec<GameMove>> {
    input
        .iter()
        .map(|game_as_strings| {
            let p1_move = other_player_translate(&game_as_strings[0]);
            let p2_move = my_translate_problem_two(&game_as_strings[1], &p1_move);
            vec![p1_move, p2_move]
        })
        .collect()
}

fn get_game_moves(
    input: &Vec<Vec<String>>,
    player_translate: &[fn(&String) -> GameMove],
) -> Vec<Vec<GameMove>> {
    input
        .iter()
        .map(|item| {
            item.iter()
                .zip(player_translate.iter())
                .map(|tup| tup.1(tup.0))
                .collect()
        })
        .collect()
}

#[derive(PartialEq, Eq, Copy, Clone)]
enum GameMove {
    Rock,
    Paper,
    Scisors,
}

fn other_player_translate(data: &String) -> GameMove {
    match data.as_str() {
        "A" => GameMove::Rock,
        "B" => GameMove::Paper,
        "C" => GameMove::Scisors,
        _ => panic!("Other player got a {}", data),
    }
}

fn my_translate_problem_two(input: &String, p1_move: &GameMove) -> GameMove {
    let outcome = my_outcome_from_input_problem_two(input);
    my_move_to_achieve_outcome(p1_move, &outcome)
}

fn my_move_to_achieve_outcome(p1_move: &GameMove, desired_result: &GameResult) -> GameMove {
    if &GameResult::Draw == desired_result {
        p1_move.clone()
    } else if &GameResult::P2Win == desired_result {
        match p1_move {
            GameMove::Rock => GameMove::Paper,
            GameMove::Paper => GameMove::Scisors,
            GameMove::Scisors => GameMove::Rock,
        }
    } else {
        match p1_move {
            GameMove::Rock => GameMove::Scisors,
            GameMove::Paper => GameMove::Rock,
            GameMove::Scisors => GameMove::Paper,
        }
    }
}

fn my_outcome_from_input_problem_two(input: &String) -> GameResult {
    match input.as_str() {
        "X" => GameResult::P1Win,
        "Y" => GameResult::Draw,
        "Z" => GameResult::P2Win,
        _ => panic!("{} is what I got", input),
    }
}

fn my_translate(data: &String) -> GameMove {
    match data.as_str() {
        "X" => GameMove::Rock,
        "Y" => GameMove::Paper,
        "Z" => GameMove::Scisors,
        _ => panic!("{} is what I got", data),
    }
}

#[derive(PartialEq, Eq)]
enum GameResult {
    Draw,
    P1Win,
    P2Win,
}

fn get_game_result(input: &Vec<GameMove>) -> GameResult {
    if 2 != input.len() {
        panic!("Found invalid game with player count {}", input.len());
    }
    let answer = if input[0] == input[1] {
        GameResult::Draw
    } else if GameMove::Rock == input[0] && GameMove::Scisors == input[1]
        || GameMove::Paper == input[0] && GameMove::Rock == input[1]
        || GameMove::Scisors == input[0] && GameMove::Paper == input[1]
    {
        GameResult::P1Win
    } else {
        GameResult::P2Win
    };
    answer
}

fn get_game_results(input: &Vec<Vec<GameMove>>) -> Vec<GameResult> {
    input.iter().map(|game| get_game_result(game)).collect()
}

#[allow(dead_code)]
fn print_game_results(input: &Vec<GameResult>) {
    for result in input {
        println!(
            "{}",
            match result {
                GameResult::Draw => "Draw",
                GameResult::P1Win => "P1",
                GameResult::P2Win => "P2",
            }
        )
    }
}

fn get_p2_score(game: &Vec<GameMove>, result: &GameResult) -> i32 {
    if 2 != game.len() {
        panic!("Found invalid game with player count {}", game.len());
    }
    let result_points = match result {
        GameResult::P1Win => 0,
        GameResult::Draw => 3,
        _ => 6,
    };
    let move_points = match game[1] {
        GameMove::Rock => 1,
        GameMove::Paper => 2,
        _ => 3,
    };
    result_points + move_points
}

fn get_total_p2_score(moves: &Vec<Vec<GameMove>>, points: &Vec<GameResult>) -> i32 {
    moves
        .iter()
        .zip(points.iter())
        .map(|tup| get_p2_score(tup.0, tup.1))
        .sum()
}

#[cfg(test)]
mod tests {

    use super::*;
    #[test]
    fn example_input_works() {
        let input = get_input_from_file("example_input.txt");
        assert_eq!(15, solve_problem_one(&input));
        assert_eq!(12, solve_problem_two(&input));
    }

    #[test]
    fn problem_input_works() {
        let input = get_input_from_file("problem_input.txt");
        assert_eq!(13484, solve_problem_one(&input));
    }
}
