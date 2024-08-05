(*STRUCTURES*)

type datatype = Int | String

type arg = datatype * string

type api_call = {
  id : string;
  input_args : arg list;
  output_args : arg list;
}

type value =
  | Ident of string
  | STR_Literal of string
  | INT_Literal of int

type condition =
  | Equal of value * value
  | NotEqual of value * value
  | And of condition * condition

type testcase = {
  api_calls : api_call list;
  cond : condition option;
  enforce : condition option;
}

type expr =
  | Identifier of string
  | Testcase of testcase
  | Union of expr * expr
  | Intersection of expr * expr
  | CartesianProduct of expr * expr
  | Difference of expr * expr
  | Power of expr * int

type clause = 
  | Unique of int

type testset = {
  id : string;
  expression : expr;
  clause : clause option;
}

type return_stmt = Return of expr

type program = {
  testsets : testset list;
  return_stmt : return_stmt;
}

module TestCaseSet = Set.Make(struct
  type t = testcase
  let compare = compare
end)

let global_memory : (string, TestCaseSet.t) Hashtbl.t = Hashtbl.create 10

(*PRINTING*)

let print_arg (datatype, id) =
  Printf.sprintf "%s %s" (match datatype with Int -> "int" | String -> "string") id

let print_args args =
  String.concat ", " (List.map print_arg args)

let print_api_call { id; input_args; output_args } =
  Printf.sprintf "%s(%s) -> (%s)" id (print_args input_args) (print_args output_args)

let rec print_value = function
  | Ident id -> id
  | STR_Literal s -> Printf.sprintf "\"%s\"" s
  | INT_Literal i -> string_of_int i

let rec print_condition = function
  | Equal (v1, v2) -> Printf.sprintf "%s == %s" (print_value v1) (print_value v2)
  | NotEqual (v1, v2) -> Printf.sprintf "%s != %s" (print_value v1) (print_value v2)
  | And (cond1, cond2) -> Printf.sprintf "%s && %s" (print_condition cond1) (print_condition cond2)

let print_clause = function
  | Some (Unique n) -> Printf.sprintf "WHERE UNIQUE(%d);" n
  | _ -> ";"

let print_testcase {api_calls; cond; enforce} =
  let api_calls_str = String.concat "; " (List.map print_api_call api_calls) in
  let cond_str = match cond with
    | Some c -> Printf.sprintf "; ASSUME %s;" (print_condition c)
    | None -> ";"
  in
  let assert_str = match enforce with
    | Some c -> Printf.sprintf "; ASSERT %s;" (print_condition c)
    | None -> ";"
  in
  Printf.sprintf "{%s%s%s}" api_calls_str cond_str assert_str


let rec print_expr = function
  | Identifier id -> id
  | Testcase testcase -> print_testcase testcase
  | Union (e1, e2) -> Printf.sprintf "(%s | %s)" (print_expr e1) (print_expr e2)
  | Intersection (e1, e2) -> Printf.sprintf "(%s & %s)" (print_expr e1) (print_expr e2)
  | CartesianProduct (e1, e2) -> Printf.sprintf "(%s * %s)" (print_expr e1) (print_expr e2)
  | Difference (e1, e2) -> Printf.sprintf "(%s / %s)" (print_expr e1) (print_expr e2)
  | Power (e, n) -> Printf.sprintf "(%s ^ %d)" (print_expr e) n

let print_testset { id; expression; clause } =
  Printf.sprintf "%s = %s %s\n" id (print_expr expression) (print_clause clause)

let print_program { testsets; return_stmt } =
  List.iter (fun testset -> print_endline (print_testset testset)) testsets;
  match return_stmt with
  | Return expr -> Printf.printf "RETURN %s;\n" (print_expr expr)

(*EXECUTION*)  

let type_of_value local_memory = function
  | Ident id -> 
    (match Hashtbl.find_opt local_memory id with
     | Some t -> t
     | None -> failwith ("Undefined identifier: " ^ id))
  | STR_Literal _ -> String
  | INT_Literal _ -> Int

let rec check_condition_types local_memory cond =
  let check_value v =
    match v with
    | Ident id ->
      (match Hashtbl.find_opt local_memory id with
       | Some _ -> ()
       | None -> failwith ("Undefined identifier: " ^ id))
    | _ -> ()
  in
  let compare_types v1 v2 =
    let t1 = type_of_value local_memory v1 in
    let t2 = type_of_value local_memory v2 in
    if t1 <> t2 then
      failwith ("Type mismatch: " ^ (print_value v1) ^ " and " ^ (print_value v2) ^ " have different types")
  in
  match cond with
  | Equal (v1, v2) | NotEqual (v1, v2) ->
    check_value v1;
    check_value v2;
    compare_types v1 v2
  | And (cond1, cond2) ->
    check_condition_types local_memory cond1;
    check_condition_types local_memory cond2


let check_init_types local_memory { api_calls; cond; enforce} =
  List.iter (fun { input_args; output_args; _ } ->
    List.iter (fun (datatype, id) -> Hashtbl.add local_memory id datatype) input_args;
    List.iter (fun (datatype, id) -> Hashtbl.add local_memory id datatype) output_args
  ) api_calls;
  (match cond with
  | Some condition -> check_condition_types local_memory condition
  | None -> ());
  (match enforce with
  | Some condition -> check_condition_types local_memory condition
  | None -> ())

let create_local_memory () : (string, datatype) Hashtbl.t = Hashtbl.create 10

let execute_testcase {api_calls; cond; enforce} =
  let local_memory = create_local_memory () in
  check_init_types local_memory {api_calls; cond; enforce};
  TestCaseSet.singleton {api_calls; cond; enforce}

let prod set1 set2 = 
  TestCaseSet.fold (fun tc1 acc ->
      TestCaseSet.fold (fun tc2 acc ->
        let combined_calls = tc1.api_calls @ tc2.api_calls in
        let combined_cond = match tc1.cond, tc2.cond with
          | Some c1, Some c2 -> Some (And (c1, c2))
          | Some c, None | None, Some c -> Some c
          | None, None -> None
        in
        let combined_assert = match tc1.enforce, tc2.enforce with
          | Some c1, Some c2 -> Some (And (c1, c2))
          | Some c, None | None, Some c -> Some c
          | None, None -> None
        in
        let combined_tc = { api_calls = combined_calls; cond = combined_cond; enforce = combined_assert} in
        TestCaseSet.add combined_tc acc
      ) set2 acc
    ) set1 TestCaseSet.empty

let rec power set n =
  if n <= 1 then set
  else prod (power set (n - 1)) set

let rec execute_expr = function
  | Identifier id -> Hashtbl.find global_memory id
  | Testcase testcase -> execute_testcase testcase
  | Union (e1, e2) ->
    let set1 = execute_expr e1 in
    let set2 = execute_expr e2 in
    TestCaseSet.union set1 set2
  | Intersection (e1, e2) ->
    let set1 = execute_expr e1 in
    let set2 = execute_expr e2 in
    TestCaseSet.inter set1 set2
  | CartesianProduct (e1, e2) ->
    let set1 = execute_expr e1 in
    let set2 = execute_expr e2 in
    prod set1 set2
  | Difference (e1, e2) ->
    let set1 = execute_expr e1 in
    let set2 = execute_expr e2 in
    TestCaseSet.diff set1 set2
  | Power (e, n) ->
    let base_set = execute_expr e in
    power base_set n


let frequency_hashmap (api_calls : api_call list) : (string, int) Hashtbl.t =
  let freq_table = Hashtbl.create 10 in
  List.iter (fun (api_call : api_call) ->
    let count = try Hashtbl.find freq_table api_call.id with Not_found -> 0 in
    Hashtbl.replace freq_table api_call.id (count + 1)
  ) api_calls;
  freq_table

let frequencies_within_limit freq_table n =
  Hashtbl.fold (fun _ count acc -> acc && (count <= n)) freq_table true

let unique_predicate n testcases =
  let filter_function testcase =
    let freq_table = frequency_hashmap testcase.api_calls in
    frequencies_within_limit freq_table n
  in
  TestCaseSet.filter filter_function testcases



let execute_testset { id; expression; clause } =
  let set = execute_expr expression
  in
  let set = match clause with
    | Some (Unique n) -> unique_predicate n set
    | _ -> set
  in
  Hashtbl.add global_memory id set

let execute_program { testsets; return_stmt } =
  List.iter execute_testset testsets;
  match return_stmt with
  | Return expr ->
    let set = execute_expr expr in
    let i = ref 1 in
    TestCaseSet.iter (fun { api_calls; cond; enforce} ->
      Printf.printf "Testcase #%d\n" !i;
      incr i;
      List.iter (fun { id; input_args; output_args } ->
        Printf.printf "%s(%s) -> (%s);\n" id (print_args input_args) (print_args output_args)
      ) api_calls;
      (match cond with
       | Some c -> Printf.printf "ASSUME %s;\n" (print_condition c)
       | None -> ());
      (match enforce with
       | Some c -> Printf.printf "ASSERT %s;\n" (print_condition c)
       | None -> ());
      print_newline ()
    ) set

