#ifndef AST_HH
#define AST_HH

#include <string>
#include <map>
#include <vector>

using namespace std;
class Visitor;

namespace ast {

enum Type {
    INT,
    BOOL,
    VOID,
    NIL
};

enum ExpressionType {
    EMPTY,
    VAR,
    NUM,
    ADD,
    SUB,
    MUL,
    DIV,
    FUNCTIONCALL
};

enum StatementType {
    SKIP,
    ASSIGN,
    BRANCH,
    LOOP,
    SEQUENCE,
    BLOCK,
    RETURN
};

class Declaration {
    private:
        string variable;
        Type type;
    public:
        Declaration(string, Type);
        string getVariable();
        Type getType();
        virtual void accept(Visitor *);
};

class DeclarationList {
    private:
        vector<Declaration *> declarations;
    public:
	    DeclarationList(vector<Declaration *>&);
        vector<Declaration *>& getDeclarations();
        virtual ~DeclarationList();
        virtual void accept(Visitor *);
};

class Expression {
    public:
        const ExpressionType exprtype;
    public:
        Expression(ExpressionType);
        virtual ~Expression() = 0;
        virtual void accept(Visitor *) = 0;
};

class Empty : public Expression {
    public:
        Empty();
        virtual void accept(Visitor *);
};

class Num : public Expression {
    public:
        const int value;
        Num(int v);
        virtual void accept(Visitor *);
};

class Var : public Expression {
    public:
        const string name;
        Var(string n);
        virtual void accept(Visitor *);
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
        virtual void accept(Visitor *) = 0;
};

class AddExpression : public BinaryExpression {
    public:
        AddExpression(Expression *l, Expression *r);
        virtual void accept(Visitor *);
};

class SubExpression : public BinaryExpression {
    public:
        SubExpression(Expression *l, Expression *r);
        virtual void accept(Visitor *);
};

class MulExpression : public BinaryExpression {
    public:
        MulExpression(Expression *l, Expression *r);
        virtual void accept(Visitor *);
};

class DivExpression : public BinaryExpression {
    public:
        DivExpression(Expression *l, Expression *r);
        virtual void accept(Visitor *);
};

class FunctionCall : public Expression {
    private:
        string name;
        vector<Expression *> arguments;
    public:
        FunctionCall(string, vector<Expression *>);
        string getName();
        vector<Expression *> getArguments();
        virtual ~FunctionCall();
        virtual void accept(Visitor *);
};

class Statement {
    public:
        const StatementType stmttype;
    public:
        Statement(StatementType);
        virtual ~Statement() = 0;
        virtual void accept(Visitor *) = 0;
};

class SkipStatement : public Statement {
    public:
        SkipStatement();
        virtual ~SkipStatement();
        virtual void accept(Visitor *);
};

class AssignmentStatement : public Statement {
    private:
        string variable;
        Expression *expression;
    public:
        AssignmentStatement(string, Expression *);
        virtual ~AssignmentStatement();
        string getVariable();
        Expression *getExpression();
        virtual void accept(Visitor *);
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
        virtual void accept(Visitor *);
};

class BranchStatement : public Statement {
    private:
        Expression *condition;
        Statement *thenStatement;
        Statement *elseStatement;
    public:
        BranchStatement(Expression *, Statement *, Statement *);
        Expression& getCondition();
        Statement& getThenStatement();
        Statement& getElseStatement();
        virtual ~BranchStatement();
        virtual void accept(Visitor *);
};

class LoopStatement : public Statement {
    private:
        Expression *condition;
        Statement *body;
    public:
        LoopStatement(Expression *, Statement *);
        Expression& getCondition();
        Statement& getBody();
        virtual ~LoopStatement();
        virtual void accept(Visitor *);
};

class BlockStatement : public Statement {
    private:
        DeclarationList * declarations;
        Statement *statement;
    public:
        BlockStatement(DeclarationList *, Statement *);
        BlockStatement(Statement *);
        DeclarationList& getDeclarations();
        Statement& getStatement();
        virtual ~BlockStatement();
        virtual void accept(Visitor *);
};

class ReturnStatement : public Statement {
    private:
        Expression *expression;
    public:
        ReturnStatement(Expression *);
        Expression& getExpression();
        virtual ~ReturnStatement();
        virtual void accept(Visitor *);
};

class FunctionDefinition {
    private:
        Type returnType;
        string name;
        DeclarationList *parameters;
        BlockStatement *body;

    public:
        FunctionDefinition(string, Type, DeclarationList *, BlockStatement *);
        Type getReturnType();
        string getName();
        DeclarationList& getParameters();
        BlockStatement& getBody();
        virtual ~FunctionDefinition();
        virtual void accept(Visitor *);
};

class FunctionDefinitionList {
    private:
        vector<FunctionDefinition *> functionDefinitions;
    public:
        FunctionDefinitionList(vector<FunctionDefinition *>&);
        FunctionDefinitionList();
        vector<FunctionDefinition *>& getFunctionDefinitions();
        virtual ~FunctionDefinitionList();
        virtual void accept(Visitor *);
};

class Program {
    private:
        string name;
        DeclarationList * declarations;
        FunctionDefinitionList * functionDefinitions;
        Statement *statement; 

    public:
        Program(string, DeclarationList *, FunctionDefinitionList *, Statement *);
        Program();
        string getName();
        DeclarationList& getDeclarationList();
        FunctionDefinitionList& getFunctionDefinitionList();
        Statement& getStatement();
        ~Program();
        virtual void accept(Visitor *);
};

extern Program *theProgram;
} // namespace ast
#endif // AST_HH
