CC=g++ -g
BUILD=build

INC=-I language

$(BUILD)/see.o : see/see.cc see/see.hh
	$(CC) -c see/see.cc -o $(BUILD)/see.o $(INC)

$(BUILD)/solver.o : see/solver.cc see/solver.hh
	$(CC) -c see/solver.cc -o $(BUILD)/solver.o $(INC)

$(BUILD)/ast.o : language/ast.cc language/ast.hh
	$(CC) -c language/ast.cc -o $(BUILD)/ast.o $(INC)

$(BUILD)/symvar.o : language/symvar.cc language/symvar.hh
	$(CC) -c language/symvar.cc -o $(BUILD)/symvar.o $(INC)
