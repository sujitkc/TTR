#ifndef AST_HH
#define AST_HH

#include <string>
#include <vector>
#include <set>

using namespace std;

namespace ast {

enum TypeType {
    TYPECONST,
    TYPEVAR,
    FUNCTIONTYPE
};

class TypeExpr {
    protected:
        string name;
    public:
        const TypeType typeType;
        TypeExpr(const string, const TypeType);
        string getName();
        friend ostream& operator << (ostream&, TypeExpr&);
        bool operator == (TypeExpr& type);
	virtual string toString() = 0;
};

ostream& operator << (ostream&, TypeExpr&);

class TypeConst : public TypeExpr {
    public:
        TypeConst(const string);
	virtual string toString();
};

class TypeVar : public TypeExpr {
    private:
        const int typeNum;
        static int nextNum;

    public:
        TypeVar(const string, const int n);
        int getTypeNum();
        static TypeVar *getNewTypeVar();
	virtual string toString();
};

class FunctionType : public TypeExpr {
    private:
        TypeExpr* returnType;
        vector<TypeExpr *> parameterTypes;

    public:
        FunctionType(TypeExpr *, vector<TypeExpr *>);
        TypeExpr& getReturnType();
        vector<TypeExpr *>& getParameterTypes();
        virtual ~FunctionType();
	virtual string toString();
};

class Language {
    private:
    set<TypeExpr *> nativeTypes;
        Language();
        static Language *instance;
    public:
        static Language& getInstance();
    TypeExpr* getNativeType(string);
    ~Language();
};

enum ExpressionType {
    VAR,
    NUM,
    BOOL,
    ADD,
    EQ,
    FUNCTIONCALL
};

enum StatementType {
    ASSIGN,
    SEQUENCE,
};

class Declaration {
    protected:
        string& variable;
        TypeExpr* type;
    public:
        Declaration(string&, TypeExpr *);
        string& getVariable();
        TypeExpr& getType();
        ~Declaration();
};

class DeclarationWithoutType : public Declaration {
    public:
        DeclarationWithoutType(string&);
};

class DeclarationWithType : public Declaration {
    public:
        DeclarationWithType(string&, TypeExpr *);
};

class DeclarationList {
    private:
        vector<Declaration *> declarations;
    public:
        DeclarationList(vector<Declaration *>&);
        vector<Declaration *>& getDeclarations();
        virtual ~DeclarationList();
};

class Expression {
    public:
        const ExpressionType exprtype;
    public:
        Expression(ExpressionType);
        virtual ~Expression() = 0;
    virtual string toString() = 0;
};

class Num : public Expression {
    public:
        const int value;
        Num(int v);
        virtual string toString();
};

class BoolConst : public Expression {
    public:
        const bool value;
        BoolConst(bool v);
        virtual string toString();
};

class Var : public Expression {
    private:
        string& name;
    public:
        string& getName();
        Var(string& n);
        virtual string toString();
};

class BinaryExpression : public Expression {
    protected:
        Expression *left;
        Expression *right;

    public:
        BinaryExpression(Expression *l, Expression *r, ExpressionType ty);
        virtual ~BinaryExpression();    
        Expression& getLeft();
        Expression& getRight();
};

class AddExpression : public BinaryExpression {
    public:
        AddExpression(Expression *l, Expression *r);
    virtual string toString();
};

class EqExpression : public BinaryExpression {
    public:
        EqExpression(Expression *l, Expression *r);
    virtual string toString();
};

class FunctionCall : public Expression {
    private:
        string& name;
        vector<Expression *> arguments;
    public:
        FunctionCall(string&, vector<Expression *>);
        string& getName();
        vector<Expression *>& getArguments();
        virtual ~FunctionCall();
    virtual string toString();
};

class Statement {
    public:
        const StatementType stmttype;
    public:
        Statement(StatementType);
        virtual ~Statement() = 0;
};

class AssignmentStatement : public Statement {
    private:
        string& variable;
        Expression *expression;
    public:
        AssignmentStatement(string&, Expression *);
        virtual ~AssignmentStatement();
        string& getVariable();
        Expression *getExpression();
};

class SequenceStatement : public Statement {
    private:
        vector<Statement *> statements;
    public:
        SequenceStatement(vector<Statement *>);
        SequenceStatement();
        void addStatement(Statement *s);
        vector<Statement *>& getStatements();
        virtual ~SequenceStatement();
};

class Program {
    private:
        static string defaultName;
        string& name;
        DeclarationList * declarations;
        Statement *statement; 

    public:
        Program(string&, DeclarationList *, Statement *);
        Program();
        string& getName();
        DeclarationList& getDeclarationList();
        Statement& getStatement();
        ~Program();
};

extern Program *theProgram;
} // namespace ast
#endif // AST_HH
