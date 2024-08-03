%{
open TestGen
let print_testset_list testsets =
  List.iter (fun ts -> Printf.printf "Testset ID: %s\n" ts.id) testsets
%}

%token <string> IDENT
%token <int> INT
%token <string> STRING
%token RETURN ASSUME ASSERT WHERE UNIQUE
%token LBRACE RBRACE LPAREN RPAREN SEMI COLON COMMA ARROW EQUAL EQ NEQ AND PLUS TIMES DIV UNION INTER EXP
%token EOF INT_TYPE STRING_TYPE

%start main
%type <TestGen.program> main

%%

main:
  |testset_list expr SEMI EOF{{ testsets = $1; return_stmt = Return $2}}

testset_list:
  | testset SEMI RETURN { [$1] }
  | testset SEMI testset_list {$1 :: $3}

testset:
  | IDENT EQUAL expr clause_opt { { id = $1; expression = $3; clause = $4 } }

clause_opt:
  | WHERE UNIQUE LPAREN INT RPAREN { Some (Unique $4) }
  | /* empty */ { None }

expr:
  | expr UNION expr { Union ($1, $3) }
  | expr INTER expr { Intersection ($1, $3) }
  | expr DIV expr { Difference ($1, $3) }
  | expr TIMES expr { CartesianProduct ($1, $3) }
  | expr EXP INT { Power ($1, $3) }
  | LPAREN expr RPAREN { $2 }
  | IDENT { Identifier $1 }
  | LBRACE testcase { Testcase $2 }

testcase:
  | api_call_list condition_list assert_list { { api_calls = $1; cond = $2; enforce = $3} }
  | api_call_list2 assert_list { { api_calls = $1; cond = None; enforce = $2} }

api_call_list:
  | api_call SEMI ASSUME{ [$1] }
  | api_call SEMI RBRACE{ [$1] }
  | api_call SEMI api_call_list { $1 :: $3 }

api_call_list2:
  | api_call SEMI ASSERT{ [$1] }
  | api_call SEMI api_call_list2 { $1 :: $3 }

api_call:
  | IDENT LPAREN arg_list ARROW LPAREN arg_list { { id = $1; input_args = $3; output_args = $6 } }

arg_list:
  | RPAREN { [] }
  | args RPAREN {[$1]}
  | args COMMA arg_list { $1 :: $3 }

args:
  | INT_TYPE IDENT { (Int, $2) }
  | STRING_TYPE IDENT { (String, $2) }

condition_list:
  | {None}
  | condition SEMI RBRACE { Some $1 }
  | condition SEMI ASSERT { Some $1 }
  | condition AND condition_list { 
      match $3 with
      | None -> Some $1
      | Some cond_list -> Some (And ($1, cond_list))
    }

assert_list:
  | {None}
  | condition SEMI RBRACE { Some $1 }
  | condition AND condition_list { 
      match $3 with
      | None -> Some $1
      | Some cond_list -> Some (And ($1, cond_list))
    }

condition:
  | value EQ value { Equal ($1, $3) } 
  | value NEQ value { NotEqual ($1, $3) }

value:
  | IDENT {Ident $1}
  | STRING {STR_Literal $1}
  | INT {INT_Literal $1}