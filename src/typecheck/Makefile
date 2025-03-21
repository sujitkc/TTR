CC=g++ -g

test_unionfind : test_unionfind.o unionfind.o
	$(CC) -o test_unionfind test_unionfind.o unionfind.o 

test_typechecker : test_typechecker.o typechecker.o ast.o
	$(CC) -o test_typechecker test_typechecker.o typechecker.o ast.o

ast.o : ast.cc ast.hh
	$(CC) -c ast.cc

test_unionfind.o : test_unionfind.cc unionfind.hh unionfind.cc
	$(CC) -c test_unionfind.cc

unionfind.o : unionfind.cc unionfind.hh
	$(CC) -c unionfind.cc

typechecker.o : typechecker.cc typechecker.hh ast.hh unionfind.hh unionfind.cc
	$(CC) -c typechecker.cc

clean:
	rm test_unionfind; \
	rm *.o
