CC=g++ -g

driver : driver.o ast.o codegen.o
	$(CC) -o driver driver.o codegen.o ast.o

driver.o : driver.cc ast.hh
	$(CC) -c driver.cc

codegen.o : codegen.cc codegen.hh ast.hh
	$(CC) -c codegen.cc

ast.o : ast.cc ast.hh
	$(CC) -c ast.cc

clean :
	rm *.o driver
