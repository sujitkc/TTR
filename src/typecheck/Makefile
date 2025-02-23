CC=g++ -g

test_ast : test_ast.o ast.o
	$(CC) -o test_ast ast.o test_ast.o

test_substitution : test_substitution.o substitution.o
	$(CC) -o test_substitution test_substitution.o substitution.o 

test_ast.o : test_ast.cc ast.hh
	$(CC) -c test_ast.cc

ast.o : ast.cc ast.hh
	$(CC) -c ast.cc

test_substitution.o : test_substitution.cc substitution.hh substitution.cc
	$(CC) -c test_substitution.cc

substitution.o : substitution.cc substitution.hh
	$(CC) -c substitution.cc

clean:
	rm test_ast; \
	rm test_substitution; \
	rm *.o
