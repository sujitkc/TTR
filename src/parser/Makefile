all: calculator

calculator: parser.tab.o ast.o lexer.yy.o main.o
	g++ -o calculator parser.tab.o ast.o lexer.yy.o main.o -lfl

ast.o : ast.hh ast.cc
	g++ -c ast.cc

main.o : main.cc ast.hh parser.tab.hh lexer.yy.cc
	g++ -c main.cc

parser.tab.o : parser.tab.cc parser.tab.hh
	g++ -c parser.tab.cc

parser.tab.cc : parser.yy
	bison -d parser.yy

lexer.yy.o : lexer.yy.cc
	g++ -c lexer.yy.cc

lexer.yy.cc : lexer.ll parser.tab.hh
	flex lexer.ll

clean:
	rm -f calculator parser.tab.* parser.tab.* *.o lex.yy.cc
