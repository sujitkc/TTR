open TestGen

let read_file filename =
  let in_channel = open_in filename in
  let file_contents = really_input_string in_channel (in_channel_length in_channel) in
  close_in in_channel;
  file_contents

let parse_input (input: string) : program =
  let lexbuf = Lexing.from_string input in
  try
    Parser.main Lexer.token lexbuf
  with
  | Lexer.Lexing_error msg ->
    failwith ("Lexing error: " ^ msg)
  | Parser.Error ->
    let pos = Lexing.lexeme_start_p lexbuf in
    let line = pos.Lexing.pos_lnum in
    let col = pos.Lexing.pos_cnum - pos.Lexing.pos_bol in
    failwith (Printf.sprintf "Syntax error at line %d, column %d" line col)

let execute_from_file filename =
  let input = read_file filename in
  let program = parse_input input in
  execute_program program

let () =
  try
    execute_from_file "input.txt"
  with
  | Failure msg -> Printf.eprintf "Error: %s\n" msg
