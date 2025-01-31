#include <iostream>
#include "ast.hh"
#include "codegen.hh"
using namespace std;
using namespace ast;
class Test {

    protected:
        virtual Program *makeProgram() = 0;
    public:
        void execute() {
            theProgram = makeProgram();
            CCodeGenerator cg;
            string code = cg.generateCode(*theProgram);
            cout << code;;
            delete theProgram;
        }
};

/*
Input:
*****************************
    int x1
    x1 := 10
*****************************
*/
class T1 : public Test {
    protected:
        virtual Program *makeProgram() {
            Declaration *d1 = new Declaration("x1", INT);
            vector<Declaration *> declarations = { d1 };
            DeclarationList *dlist = new DeclarationList(declarations);
            Statement *s = new AssignmentStatement("x1", new Num(10));
            vector<FunctionDefinition *> fdefs;
            FunctionDefinitionList *fd = new FunctionDefinitionList(fdefs);
            return new Program("P1", dlist, fd, s);
        }
};

/*
Input:
*****************************
    int x2
    int y2
    x2 := 10
    y2 := 1
*****************************
*/
class T2 : public Test {
    protected:
        virtual Program *makeProgram() {
            Declaration *d1 = new Declaration("x2", INT);
            Declaration *d2 = new Declaration("y2", INT);
            vector<Declaration *> declarations = { d1, d2 };
            DeclarationList *dlist = new DeclarationList(declarations);
            Statement *s1 = new AssignmentStatement("x2", new Num(10));
            Statement *s2 = new AssignmentStatement("y2", new Num(1));
            SequenceStatement *seq1 = new SequenceStatement();
            seq1->addStatement(s1);
            seq1->addStatement(s2);
            vector<FunctionDefinition *> fdefs;
            FunctionDefinitionList *fd = new FunctionDefinitionList(fdefs);
            return new Program("P2", dlist, fd, seq1);
        }
};

/*
Input:
*****************************
  int x3;
  int add(int x4, int y4) { return x4 + y4 }
  x3 := add(1, 2)
*****************************
*/
class T3 : public Test {
    protected:
        virtual Program *makeProgram() {
        
            Declaration *d1 = new Declaration("x3", INT); // int x3
            vector<Declaration *> declarations{d1};
            DeclarationList *dlist = new DeclarationList(declarations);
            Declaration *p1 = new Declaration("x4", INT); // parameter 1
            Declaration *p2 = new Declaration("y4", INT); // parameter 2
            vector<Declaration *> par = {p1, p2}; // parameter list
            DeclarationList *params = new DeclarationList(par);
            Var *v1 = new Var("x4");
            Var *v2 = new Var("y4");
            AddExpression *e1 = new AddExpression(v1, v2); // x4 + y4
            ReturnStatement *s1 = new ReturnStatement(e1); // return x4 + y4
            BlockStatement *b1 = new BlockStatement(s1); // { return x4 + y4 }
            FunctionDefinition *fd1 = 
                new FunctionDefinition("add", INT, params, b1);
                    // int add(int x4, int y4) { return x4 + y4 }
            vector<FunctionDefinition *> fdefs{fd1};
            FunctionDefinitionList *fd = new FunctionDefinitionList(fdefs);
            Num *a1 = new Num(1); // 1
            Num *a2 = new Num(2); // 2
            vector<Expression *> args{a1, a2}; // (1, 2)
            FunctionCall *fc1 = new FunctionCall("add", args); // add(1, 2)
            AssignmentStatement *s2 = 
                new AssignmentStatement("x3", fc1); // x3 := add(1, 2)

            return new Program("P3", dlist, fd, s2);
        }
};

int main() {
    vector<Test *> testcases = {
        new T1(),
        new T2(),
        new T3()
    };
    try {
        for(auto& t : testcases) {
            t->execute();
            delete t;
        }
    }
    catch(const string& m) {
        cout << "Typechecker exception : " << m << endl;
    }
    catch(const char* m) {
        cout << "Typechecker exception : " << m << endl;
    }
    catch(...) {
        cout << "Typechecker exception : " << endl;
    }
    return 0;
}
