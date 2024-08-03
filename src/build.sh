#!/bin/bash

# Compile TestGen.ml first
ocamlc -c TestGen.ml

# Generate lexer.ml from lexer.mll
ocamllex lexer.mll

# Generate parser.ml and parser.mli from parser.mly
menhir --ocamlc "ocamlc -I +ocaml" --explain --infer parser.mly

# Compile parser.mli
ocamlc -c parser.mli

# Compile parser.ml
ocamlc -c parser.ml

# Compile lexer.ml
ocamlc -c lexer.ml

# Link all object files into the final executable
ocamlc -o result lexer.cmo parser.cmo TestGen.cmo main.ml
