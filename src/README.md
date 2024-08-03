# OCaml Test Specification Compiler
The ocaml code acts as a compiler to create "abstract" testcases for a test specification language input, with the idea that these abstract testcases could later be executed by a symbolic execution engine. 

## Compilation and Execution:
To compile the files, run the build.sh shell script provided (./build.sh). Write the test specifications in input.txt, and run (./result) to get the list of testcases.

## File Descriptions:
build.sh - Shell script used to compile

grammar.txt - The CFG of the test specification language

input.txt - Input file of the compiler

lexer.mll - The tokens recognized by the compiler

parser.mly - Parsing the tokens into the appropriate OCaml structs

TestGen.ml - Contains definitions of the OCaml struct, print functions, and execute functions

main.ml - Reads input uses TestGen.ml to execute the code

## Test Specification Language: 
Given below are the components of the language.

### API Call: 
Represents an API endpoint, its input arguments, and its output arguments.

#### Syntax: 
api_id(type in_arg1, type in_arg2) -> (type out_arg1, type out_arg2);

### Conditions: 
Equality and Inequality conditions on arguments or literals. Multiple Conditions are chained together using &&.

#### Syntax:
x == 5 && x == y && y != 10 && result == "OK"

### Assume: 
Conditions on arguments that the executor can assume to be true before test execution.

### Assert: 
Conditions that must be verified after executing the test case.

### Test Case: 
Consists of a sequence of API calls, along with an optional ASSUME condition and ASSERT condition. Type checking is performed to ensure that the conditions on the arguments are valid.

#### Syntax:
{
    API1;
    API2;
    ASSUME condition1;
    ASSERT condition2;
}

Note that the optional ASSUME and ASSERT must come after the api calls in this order.

### Test Set: 
A set of testcases represented by an identifier and defined using an expression and an optional clause.

#### Syntax:
id = expr WHERE clause;

### Expression:
An expression is used to combine several testsets using set operations. An expression can either be a testcase which defines a singleton, an identifier to a testset, or an operator on sub-expressions.

#### Operators:
A | B - Union 

A & B - Intersection 

A / B - Difference

A * B - Cartesian Product

A ^ n - Power

### Clause:
The clause is used to filter an expression, upon a predicate. As of now there is only one predicate defined. The UNIQUE(n) predicate removes all testcases where there exists an API occuring more than n times.

### Return:
The compiler's output will finally be a testset. This is defined by the return statement.

#### Syntax:

RETURN expr;

For a deeper understanding refer to the example in the input.

## Issues:
Given below are a list of fixes which must be made to convert this application into a usable form, as defined by the "reference.txt" file.

### 1. Advanced Datatypes: 
As of now the only defined datatypes are int and string. Further primitives can easily be added, but it is essential to also support custom datatypes as most API calls return JSON objects defined by a schema. Tuples are also necessary to support data such as username password pairs.

### 2. Global Variables: 
Many of the conditions are not purely based on API arguments and literals alone, and instead operate on global variables defined by the langauge such as "U the set of users". The langauge grammar needs to be modified to include these.

### 3. Advanced Conditions: 
As of now, ASSUME and ASSERT only have equality conditions. One very useful condition not present as of now is "IN" to check if an element belongs to a set. 

### 4. Additional Predicates: 
More predicates can be defined such as the PATTERN predicate which filters out any testcase which does not contain a chain of API calls as a pattern.
