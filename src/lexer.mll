{
open Parser
exception Lexing_error of string
}

rule token = parse
  | [' ' '\t' '\r' '\n'] { token lexbuf }
  | "{" { LBRACE }
  | "}" { RBRACE }
  | "(" { LPAREN }
  | ")" { RPAREN }
  | ";" { SEMI }
  | ":" { COLON }
  | "," { COMMA }
  | "->" { ARROW }
  | "==" { EQ }
  | "!=" { NEQ }
  | "&&" { AND }
  | "+" { PLUS }
  | "*" { TIMES }
  | "/" { DIV }
  | "|" { UNION }
  | "&" { INTER }
  | "^" { EXP }
  | "=" { EQUAL }
  | "RETURN" { RETURN }
  | "ASSUME" { ASSUME }
  | "ASSERT" { ASSERT }
  | "WHERE" { WHERE }
  | "UNIQUE" { UNIQUE }
  | "int" { INT_TYPE }
  | "string" { STRING_TYPE }
  | ['a'-'z' 'A'-'Z' '_'] ['a'-'z' 'A'-'Z' '0'-'9' '_']* as id { IDENT id }
  | ['0'-'9']+ as n { INT (int_of_string n) }
  | "\""[^'\"']* "\"" as s { STRING (String.sub s 1 (String.length s - 2)) }
  | eof { EOF }
  | _ as c { raise (Lexing_error (Printf.sprintf "Unexpected character: %c" c)) }
