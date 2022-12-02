use std::fs;

fn main() {
    let input = get_input_from_file("example_input.txt");
    //let input = get_input_from_file("problem_input.txt");
    print_raw_data(&input);
    let translators = [other_player_translate, my_translate];
    let game_moves = get_game_moves(&input, &translators);
    print_game_moves(&game_moves)
}

fn print_raw_data(input: &Vec<Vec<String>>){
    for vec_pair in input {
        for a_string in vec_pair {
            print!("`{}`, ", a_string);
        }
        print!("\n")
    }
}
fn print_game_moves(input: &Vec<Vec<GameMove>>){
    for vec_pair in input {
        for tup in vec_pair {
            print!("{}, ", match tup{
                GameMove::Rock=>"Rock",
                GameMove::Paper=>"Paper",
                GameMove::Scisors=>"Scisors",
            });
        }
        print!("\n");
    }
}

fn get_input_from_file(file: &str) -> Vec<Vec<String>> {
    let file_contents = fs::read_to_string(file).unwrap();
    file_contents.split(|c|c=='\n')
    .map(|line| line.split(|c|c==' ')
        .map(|item|item.to_string())
        .collect::<Vec<String>>())
    .collect::<Vec<Vec<String>>>()
}

fn get_game_moves(input: &Vec<Vec<String>>, player_translate: &[fn(&String)->GameMove])->Vec<Vec<GameMove>>{
    input.iter().map(|item| item.iter()
        .zip(player_translate.iter())
            .map(|tup| tup.1(tup.0))
            .collect())
    .collect()
}

enum GameMove{
    Rock,
    Paper,
    Scisors,
}

fn other_player_translate(data:&String)->GameMove{
    match data.as_str(){
        "A"=>GameMove::Rock,
        "B"=>GameMove::Paper,
        "C"=>GameMove::Scisors,
        _ => panic!("Other player got a {}", data)
    }
}

fn my_translate(data:&String)->GameMove{
    match data.as_str(){
        "X"=>GameMove::Rock,
        "Y"=>GameMove::Paper,
        "Z"=>GameMove::Scisors,
        _ => panic!("{} is what I got", data)
    }
}