#ifndef AST_HH
#define AST_HH

#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;

enum class HTTPResponseCode
{
    OK_200,
    CREATED_201,
    BAD_REQUEST_400,
    // Add other response codes as needed
};

enum class ExprType
{
    MAP,
    VAR,
    STRING,
    NUM,
    TUPLE,
    SET,
    FUNCTIONCALL_EXPR,
    POLYMORPHIC_FUNCTIONCALL_EXPR
};

enum class TypeExprType
{
    TYPE_CONST,
    TYPE_VARIABLE,
    FUNC_TYPE,
    MAP_TYPE,
    SET_TYPE,
    TUPLE_TYPE
};

enum class StatementType
{
    ASSIGN,
    FUNCTIONCALL_STMT,
    DECL,
};

class TypeExpr
{
public:
    virtual ~TypeExpr() = default;
//    virtual void accept(ASTVisitor &visitor) = 0;
    virtual unique_ptr<TypeExpr> clone() = 0;
    TypeExprType typeExprType;

protected:
    TypeExpr(TypeExprType);
};

class FuncDecl
{
public:
    string name;
    unique_ptr<TypeExpr> params;
    unique_ptr<TypeExpr> outp;

    FuncDecl(string, unique_ptr<TypeExpr>, unique_ptr<TypeExpr>);
};

// Type Expressions
class TypeConst : public TypeExpr
{
public:
    explicit TypeConst(string);
//    void accept(ASTVisitor &) override;
//    void accept(ExpoSEVisitor *);
    unique_ptr<TypeExpr> clone() override;
    const string name;
};

class FuncType : public TypeExpr
{
public:
    FuncType(vector<unique_ptr<TypeExpr>>, unique_ptr<TypeExpr>);
//    void accept(ASTVisitor &) override;
//    void accept(ExpoSEVisitor *);
    unique_ptr<TypeExpr> clone() override;
    vector<unique_ptr<TypeExpr>> params;
    unique_ptr<TypeExpr> returnType;
};

class MapType : public TypeExpr
{
public:
    MapType(unique_ptr<TypeExpr>, unique_ptr<TypeExpr>);
//    void accept(ASTVisitor &) override;
//    void accept(ExpoSEVisitor *);
    unique_ptr<TypeExpr> clone() override;
    unique_ptr<TypeExpr> domain;
    unique_ptr<TypeExpr> range;
};

class TupleType : public TypeExpr
{
public:
    explicit TupleType(vector<unique_ptr<TypeExpr>>); 
//    void accept(ASTVisitor &) override;
//    void accept(ExpoSEVisitor *);
    unique_ptr<TypeExpr> clone() override;
    vector<unique_ptr<TypeExpr>> elements;
};

class SetType : public TypeExpr
{
public:
    explicit SetType(unique_ptr<TypeExpr>);
//    void accept(ASTVisitor &) override;
//    void accept(ExpoSEVisitor *visitor);
    unique_ptr<TypeExpr> clone()  override;
    unique_ptr<TypeExpr> elementType;
};

class Decl
{
public:
    Decl(string, unique_ptr<TypeExpr>);
    virtual ~Decl() = default;
//    virtual void accept(ASTVisitor &);
//    void accept(ExpoSEVisitor *);
    Decl( Decl &);
    virtual unique_ptr<Decl> clone();
    string name;
    unique_ptr<TypeExpr> type;
};

// Expressions
class Expr
{
public:
    virtual ~Expr() = default;
//    virtual void accept(ASTVisitor&) = 0;
//    virtual void accept(ExpoSEVisitor *) = 0;
    virtual unique_ptr<Expr> clone() const = 0;

    ExprType exprType;

protected:
    Expr(ExprType);
};

class Var : public Expr
{
public:
    explicit Var(string);
//    void accept(ASTVisitor &)  override;
    bool operator<(const Var &v) const;
//    void accept(ExpoSEVisitor *visitor) override;
    unique_ptr<Expr> clone() const override;

    string name;
};

class FuncCall : public Expr
{
public:
    FuncCall(string, vector<unique_ptr<Expr>>);
//    void accept(ASTVisitor &)  override;
//    void accept(ExpoSEVisitor *) override;
    unique_ptr<Expr> clone() const override;
    string name;
    vector<unique_ptr<Expr>> args;
};

class Num : public Expr
{
public:
    explicit Num(int);
//    void accept(ASTVisitor &) override;
//    void accept(ExpoSEVisitor *) override;
    unique_ptr<Expr> clone() const override;

    int value;
};

class String : public Expr
{
public:
    explicit String(string);
//    void accept(ASTVisitor &) override;
//    void accept(ExpoSEVisitor *) override;
    unique_ptr<Expr> clone() const override;
    string value;
};

class Set : public Expr
{
public:
    explicit Set(vector<unique_ptr<Expr>>);
//    void accept(ASTVisitor &visitor)  override;
//    void accept(ExpoSEVisitor *visitor) override;
    unique_ptr<Expr> clone() const override;

    vector<unique_ptr<Expr>> elements;
};

class Map : public Expr
{
public:
    explicit Map(vector<pair<unique_ptr<Var>, unique_ptr<Expr>>>);
//    void accept(ASTVisitor &visitor)  override;
//    void accept(ExpoSEVisitor *visitor) override;
    unique_ptr<Expr> clone() const override;

    vector<pair<unique_ptr<Var>, unique_ptr<Expr>>> value;
};

class Tuple : public Expr
{
private:
    vector<unique_ptr<Expr>> exprs;
public:
    explicit Tuple(vector<unique_ptr<Expr>> exprs);
//    void accept(ASTVisitor &visitor)  override;
//    void accept(ExpoSEVisitor *visitor);
    unique_ptr<Expr> clone() const override;
};

// Function Declaration
class APIFuncDecl
{
public:
    APIFuncDecl(string,
             vector<unique_ptr<TypeExpr>>,
             pair<HTTPResponseCode, vector<unique_ptr<TypeExpr>>>);
//    void accept(ASTVisitor &);
//    void accept(ExpoSEVisitor *);

    string name;
    vector<unique_ptr<TypeExpr>> params;
    pair<HTTPResponseCode, vector<unique_ptr<TypeExpr>>> returnType;
};

// Initialization
class Init
{
public:
    Init(string, unique_ptr<Expr>);
//    void accept(ASTVisitor &) 
//    void accept(ExpoSEVisitor *)

    string varName;
    unique_ptr<Expr> expr;
};

class Response
{
public:
    HTTPResponseCode code;
    unique_ptr<Expr> expr;
    Response(HTTPResponseCode, unique_ptr<Expr>);
//    void accept(ASTVisitor &);
//    void accept(ExpoSEVisitor *);
};

class APIcall
{
public:
    unique_ptr<FuncCall> call;
    Response response;
//    void accept(ASTVisitor &);
//    void accept(ExpoSEVisitor *visitor);

    APIcall(unique_ptr<FuncCall>, Response);
};

// API
class API
{
public:
    API(unique_ptr<Expr>, unique_ptr<APIcall>, Response);
//    void accept(ASTVisitor &);
//    void accept(ExpoSEVisitor *);

    unique_ptr<Expr> pre;
    unique_ptr<APIcall> call;
    Response response;
};

// Block class (placeholder as it wasn't fully specified in the grammar)
// Top-level Spec class
class Spec
{
public:
    Spec(vector<unique_ptr<Decl>>,
         vector<unique_ptr<Init>>,
         vector<unique_ptr<APIFuncDecl>>,
         vector<unique_ptr<API>>);
//    void accept(ASTVisitor &);
//    void accept(ExpoSEVisitor *);

    vector<unique_ptr<Decl>> globals;
    vector<unique_ptr<Init>> init;
    vector<unique_ptr<APIFuncDecl>> functions;
    vector<unique_ptr<API>> blocks;
};

class Stmt
{
public:
    virtual ~Stmt() = default;
//    virtual void accept(ASTVisitor &) = 0;
//    virtual void accept(ExpoSEVisitor *) = 0;

    StatementType statementType;
    virtual unique_ptr<Stmt> clone() const = 0;
protected:
    Stmt(StatementType);
};

// Assignment statement: l = r
class Assign : public Stmt
{
public:
    Assign(unique_ptr<Var>, unique_ptr<Expr>);
//    void accept(ASTVisitor &visitor)  override;
//    void accept(ExpoSEVisitor *visitor)  override;
    unique_ptr<Stmt> clone() const override;
    
    unique_ptr<Var> left;
    unique_ptr<Expr> right;
};

// Function call statement
class FuncCallStmt : public Stmt
{
public:
    explicit FuncCallStmt(unique_ptr<FuncCall>);
//    void accept(ASTVisitor &visitor)  override;
//    void accept(ExpoSEVisitor *visitor)  override;
    unique_ptr<Stmt> clone() const override;
    unique_ptr<FuncCall> call;
};

// Program is the root of our AST
class Program
{
public:
    explicit Program(vector<unique_ptr<Stmt>>);
//    void accept(ASTVisitor &);
//    void accept(ExpoSEVisitor *);

    vector<unique_ptr<Stmt>> statements;
};
#endif
