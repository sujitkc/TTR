#include <iostream>
#include "typechecker.hh"

using namespace std;
using namespace ast;
using namespace typechecker;

class Test {

    protected:
        virtual Program *makeProgram() = 0;
    public:
        void execute() {
            theProgram = makeProgram();
	    cout << "Test case for " << theProgram->getName() << endl;
	    Typechecker *typechecker = new Typechecker();
	    typechecker->typecheckProgram(*theProgram);
	    delete typechecker;
            delete theProgram;
        }
};

/*
Input:
*****************************
var x : int
x := 10
*****************************
*/
class T1 : public Test {
    protected:
        virtual Program *makeProgram() {
            TypeExpr *intType = Language::getInstance().getNativeType("int");
            Declaration *d1 = new DeclarationWithType("x", intType);
            vector<Declaration *> ds = { d1 };
	    DeclarationList *dlist = new DeclarationList(ds);
            Statement *s = new AssignmentStatement("x", new Num(10));
            return new Program("P1", dlist, s);
        }
};

/*
Input:
*****************************
var x : int
x := true // type error
*****************************
*/
class T2 : public Test {
    protected:
        virtual Program *makeProgram() {
            TypeExpr *intType = Language::getInstance().getNativeType("int");
            Declaration *d1 = new DeclarationWithType("x", intType);
            vector<Declaration *> ds = { d1 };
	    DeclarationList *dlist = new DeclarationList(ds);
            Statement *s = new AssignmentStatement("x", new BoolConst(true));
            return new Program("P2", dlist, s);
        }
};

/*
Input:
*****************************
var x : int
var y : int

x := y
*****************************
*/
class T3 : public Test {
    protected:
        virtual Program *makeProgram() {
            TypeExpr *intType = Language::getInstance().getNativeType("int");
            Declaration *d1 = new DeclarationWithType("x", intType);
            Declaration *d2 = new DeclarationWithType("y", intType);
            vector<Declaration *> ds = { d1, d2 };
	    DeclarationList *dlist = new DeclarationList(ds);
            Statement *s = new AssignmentStatement("x", new Var("y"));
            return new Program("P3", dlist, s);
        }
};

/*
Input:
*****************************
var x : int
var y : bool

x := y // type error
*****************************
*/
class T4 : public Test {
    protected:
        virtual Program *makeProgram() {
            TypeExpr *intType = Language::getInstance().getNativeType("int");
            TypeExpr *boolType = Language::getInstance().getNativeType("bool");
            Declaration *d1 = new DeclarationWithType("x", intType);
            Declaration *d2 = new DeclarationWithType("y", boolType);
            vector<Declaration *> ds = { d1, d2 };
	    DeclarationList *dlist = new DeclarationList(ds);
            Statement *s = new AssignmentStatement("x", new Var("y"));
            return new Program("P3", dlist, s);
        }
};

/*
Input:
*****************************
var x
var y : int

x := y // successful unification resulting in x's type becoming int.
*****************************
*/
class T5 : public Test {
    protected:
        virtual Program *makeProgram() {
            TypeExpr *intType = Language::getInstance().getNativeType("int");
            Declaration *d1 = new DeclarationWithoutType("x");
            Declaration *d2 = new DeclarationWithType("y", intType);
            vector<Declaration *> ds = { d1, d2 };
	    DeclarationList *dlist = new DeclarationList(ds);
            Statement *s = new AssignmentStatement("x", new Var("y"));
            return new Program("P5", dlist, s);
        }
};

/*
Input:
*****************************
var x // implicit type
x := 10 // successful unification resulting in x's type becoming int.
*****************************
*/
class T6 : public Test {
    protected:
        virtual Program *makeProgram() {
            TypeExpr *intType = Language::getInstance().getNativeType("int");
            Declaration *d1 = new DeclarationWithoutType("x");
            vector<Declaration *> ds = { d1 };
	    DeclarationList *dlist = new DeclarationList(ds);
            Statement *s = new AssignmentStatement("x", new Num(10));
            return new Program("P6", dlist, s);
        }
};

/*
Input:
*****************************
var x : int // implicit type
x := 10 + 20 // successful unification resulting in x's type becoming int.
*****************************
*/
class T7 : public Test {
    protected:
        virtual Program *makeProgram() {
            TypeExpr *intType = Language::getInstance().getNativeType("int");
            Declaration *d1 = new DeclarationWithType("x", intType);
            vector<Declaration *> ds = { d1 };
	    DeclarationList *dlist = new DeclarationList(ds);
	    Num *n1 = new Num(10);
	    Num *n2 = new Num(20);
	    AddExpression *add1 = new AddExpression(n1, n2);
            Statement *s = new AssignmentStatement("x", add1);
            return new Program("P7", dlist, s);
        }
};

/*
Input:
*****************************
var x // implicit type
x := 10 + 20 // successful unification resulting in x's type becoming int.
*****************************
*/
class T8 : public Test {
    protected:
        virtual Program *makeProgram() {
            Declaration *d1 = new DeclarationWithoutType("x");
            vector<Declaration *> ds = { d1 };
	    DeclarationList *dlist = new DeclarationList(ds);
	    Num *n1 = new Num(10);
	    Num *n2 = new Num(20);
	    AddExpression *add1 = new AddExpression(n1, n2);
            Statement *s = new AssignmentStatement("x", add1);
            return new Program("P8", dlist, s);
        }
};

/*
Input:
*****************************
// x has been declared with implicit type.  assigning 10 to x will give it
// int type. Subsequently, typechecking of function call to f by
// passing x as an argument to f : int --> int, its type will be unified with int.

var x // implicit type
var y : int
var f : int --> int
x := 10
y := f(x)
*****************************
*/
class T9 : public Test {
    protected:
        virtual Program *makeProgram() {
            TypeExpr *intType = Language::getInstance().getNativeType("int");
            Declaration *d1 = new DeclarationWithoutType("x");
            Declaration *d2 = new DeclarationWithType("y", intType);
	    vector<TypeExpr *> ptypes = {intType};
	    DeclarationWithType *d3 = new DeclarationWithType("f", new FunctionType(intType, ptypes));
            vector<Declaration *> ds = { d1, d2, d3 };
	    DeclarationList *dlist = new DeclarationList(ds);
	    Num *n1 = new Num(10);
            Statement *s1 = new AssignmentStatement("x", n1);
	    vector<Expression*> args = { new Var("x") };
	    Statement *s2 = new AssignmentStatement("y", new FunctionCall("f", args));
	    Statement *s = new SequenceStatement({s1, s2});
            return new Program("P9", dlist, s);
        }
};

/*
Input:
*****************************
// Here the function declaration is implicit type
// But the information in the program is sufficient for the type inferencer
// to deduce that f is of int --> int type.
var x // implicit type
var y : int
var f // implicit type
x := 10
y := f(x)
*****************************
*/
class T10 : public Test {
    protected:
        virtual Program *makeProgram() {
            TypeExpr *intType = Language::getInstance().getNativeType("int");
            Declaration *d1 = new DeclarationWithoutType("x");
            Declaration *d2 = new DeclarationWithType("y", intType);
	    vector<TypeExpr *> ptypes = {intType};
	    Declaration *d3 = new DeclarationWithoutType("f");
            vector<Declaration *> ds = { d1, d2, d3 };
	    DeclarationList *dlist = new DeclarationList(ds);
	    Num *n1 = new Num(10);
            Statement *s1 = new AssignmentStatement("x", n1);
	    vector<Expression*> args = { new Var("x") };
	    Statement *s2 = new AssignmentStatement("y", new FunctionCall("f", args));
	    Statement *s = new SequenceStatement({s1, s2});
            return new Program("P10", dlist, s);
        }
};

/*
Input:
*****************************
// Since f is declared as bool --> int, passing a parameter of int type
// will cause a type error.
var x // implicit type
var y : int
var f : bool --> int
x := 10
y := f(x)
*****************************
*/
class T11 : public Test {
    protected:
        virtual Program *makeProgram() {
            TypeExpr *intType = Language::getInstance().getNativeType("int");
            TypeExpr *boolType = Language::getInstance().getNativeType("bool");
            Declaration *d1 = new DeclarationWithoutType("x");
            Declaration *d2 = new DeclarationWithType("y", intType);
	    vector<TypeExpr *> ptypes = {intType};
	    Declaration *d3 = new DeclarationWithType("f", new FunctionType(boolType, ptypes));
            vector<Declaration *> ds = { d1, d2, d3 };
	    DeclarationList *dlist = new DeclarationList(ds);
	    Num *n1 = new Num(10);
            Statement *s1 = new AssignmentStatement("x", n1);
	    vector<Expression*> args = { new Var("x") };
	    Statement *s2 = new AssignmentStatement("y", new FunctionCall("f", args));
	    Statement *s = new SequenceStatement({s1, s2});
            return new Program("P11", dlist, s);
        }
};

/*
Input:
*****************************
// Since f is declared as int --> int, assigning the value returned
// from f(x) to y (which is declared to be bool)
// will cause a type error.
var x // implicit type
var y : bool
var f : bool --> int
x := 10
y := f(x)
*****************************
*/
class T12 : public Test {
    protected:
        virtual Program *makeProgram() {
            TypeExpr *intType = Language::getInstance().getNativeType("int");
            TypeExpr *boolType = Language::getInstance().getNativeType("bool");
            Declaration *d1 = new DeclarationWithoutType("x");
            Declaration *d2 = new DeclarationWithType("y", boolType);
	    vector<TypeExpr *> ptypes = {intType};
	    Declaration *d3 = new DeclarationWithType("f", new FunctionType(intType, ptypes));
            vector<Declaration *> ds = { d1, d2, d3 };
	    DeclarationList *dlist = new DeclarationList(ds);
	    Num *n1 = new Num(10);
            Statement *s1 = new AssignmentStatement("x", n1);
	    vector<Expression*> args = { new Var("x") };
	    Statement *s2 = new AssignmentStatement("y", new FunctionCall("f", args));
	    Statement *s = new SequenceStatement({s1, s2});
            return new Program("P12", dlist, s);
        }
};

/*
Input:
*****************************
// Since f is declared as int --> int, assigning the value returned
// from f(x) to y (which is declared with implicit type) will give y
// int type. Subsequently trying to assigning boolean constant true
// to y will cause a type error.
var x // implicit type
var y
var f : int --> int
x := 10
y := f(x)
y := true
*****************************
*/
class T13 : public Test {
    protected:
        virtual Program *makeProgram() {
            TypeExpr *intType = Language::getInstance().getNativeType("int");
            TypeExpr *boolType = Language::getInstance().getNativeType("bool");
            Declaration *d1 = new DeclarationWithoutType("x");
            Declaration *d2 = new DeclarationWithType("y", boolType);
	    vector<TypeExpr *> ptypes = {intType};
	    Declaration *d3 = new DeclarationWithType("f", new FunctionType(intType, ptypes));
            vector<Declaration *> ds = { d1, d2, d3 };
	    DeclarationList *dlist = new DeclarationList(ds);
	    Num *n1 = new Num(10);
            Statement *s1 = new AssignmentStatement("x", n1);
	    vector<Expression*> args = { new Var("x") };
	    Statement *s2 = new AssignmentStatement("y", new FunctionCall("f", args));
	    Statement *s3 = new AssignmentStatement("y", new BoolConst(true));
	    Statement *s = new SequenceStatement({s1, s2, s3});
            return new Program("P13", dlist, s);
        }
};

int main() {
    vector<Test *> testcases = {
        new T1(), new T2(), new T3(),
	new T4(), new T5(), new T6(),
	new T7(), new T8(), new T9 (),
	new T10(), new T11(), new T12 (),
	new T13 ()
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
