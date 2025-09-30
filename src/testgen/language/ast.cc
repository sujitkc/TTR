#include "ast.hh"

using namespace std;

TypeExpr::TypeExpr(TypeExpression typeExpr) : typeExpression(typeExpr) {}

FunDecl(string name, 
    unique_ptr<TypeExpr> param,
    unique_ptr<TypeExpr> outp) : name(move(name)),
        params(move(param)), outp(move(outp)) {
}

TypeConst::TypeConst(string name) : TypeExpr(TypeExpression::TYPE_CONST), name(move(name)) {
}

void TypeExpr::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void TypeExpr::accept(ExpoSEVisitor *visitor) {
}

unique_ptr<TypeExpr> TypeExpr::clone() {
    return make_unique<TypeConst>(name);
}

FuncType::FuncType(vector<unique_ptr<TypeExpr>> params,
    unique_ptr<TypeExpr> returnType)
    : TypeExpr(TypeExpression::FUNC_TYPE), params(move(params)),
        returnType(move(returnType)) {
}

void FuncType::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void FuncType::accept(ExpoSEVisitor *visitor) {
    for (auto &param : params)
    {
    }
}

unique_ptr<TypeExpr> FuncType::clone() {
    vector<unique_ptr<TypeExpr>> clonedParams;
    for (auto &param : params)
    {
        clonedParams.push_back(param->clone());
    }
    auto clonedReturnType = returnType ? returnType->clone() : nullptr;
    return make_unique<FuncType>(move(clonedParams), move(clonedReturnType));
}

MapType::MapType(unique_ptr<TypeExpr> domain, unique_ptr<TypeExpr> range)
    : TypeExpr(TypeExpression::MAP_TYPE),
        domain(move(domain)), range(move(range)) {
}

void MapType::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void MapType::accept(ExpoSEVisitor *visitor) {
}

unique_ptr<TypeExpr> MapType::clone() {
    auto clonedDomain = domain ? domain->clone() : nullptr;
    auto clonedRange = range ? range->clone() : nullptr;
    return make_unique<MapType>(move(clonedDomain), move(clonedRange));
}

TupleType::TupleType(vector<unique_ptr<TypeExpr>> elements)
    : TypeExpr(TypeExpression::TUPLE_TYPE), elements(move(elements)) {}

void TupleType::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void TupleType::accept(ExpoSEVisitor *visitor) {
    for (auto &e : elements)
    {
    }
}

// Clone implementation for TupleType
unique_ptr<TypeExpr> TupleType::clone() {
    vector<unique_ptr<TypeExpr>> clonedElements;
    for ( auto &element : elements)
    {
        clonedElements.push_back(element->clone());
    }
    return make_unique<TupleType>(move(clonedElements));
}

SetType::SetType(unique_ptr<TypeExpr> elementType)
    : TypeExpr(TypeExpression::SET_TYPE), elementType(move(elementType)) {}

void SetType::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void SetType::accept(ExpoSEVisitor *visitor) {
}

unique_ptr<TypeExpr> SetType::clone() {
    auto clonedElementType = elementType ? elementType->clone() : nullptr;
    return make_unique<SetType>(move(clonedElementType));
}

Decl::Decl(string name, unique_ptr<TypeExpr> typeExpr)
    : name(move(name)), type(move(typeExpr)) {}

virtual void Decl::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void Decl::accept(ExpoSEVisitor *visitor) {
}

// Copy constructor for deep copying.
Decl::Decl( Decl &other) : name(other.name) {
    if (other.type)
    {
        type = other.type->clone();
    }
}

virtual unique_ptr<Decl> Decl::clone() {
    return make_unique<Decl>(*this);
}

Expr::Expr(ExpressionType exprType) : expressionType(exprType) {}

Var::Var(string name) : Expr(ExpressionType::VAR), name(move(name)) {}

void Var::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

bool Var::operator<(const Var &v) const {
    return name < v.name;
}

void Var::accept(ExpoSEVisitor *visitor) {
}

unique_ptr<Expr> Var::clone() {
    return make_unique<Var>(name);
}

FuncCall::FuncCall(string name, vector<unique_ptr<Expr>> args)
    : Expr(ExpressionType::FUNCTIONCALL_EXPR),
      name(move(name)), args(move(args)) {
}

void FuncCall::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void FuncCall::accept(ExpoSEVisitor *visitor) {
    for (auto &arg : args)
    {
        visitor->visitExpr(*arg);
    }
}

unique_ptr<Expr> FuncCall::clone() {
    vector<unique_ptr<Expr>>clonedArgs;
    for(auto &arg:args){
        clonedArgs.push_back(arg->clone());
    }

    return make_unique<FuncCall>(name,move(clonedArgs));
}

Num::Num(int value) : Expr(ExpressionType::NUM), value(value) {}

void Num::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void Num::accept(ExpoSEVisitor *visitor) {
    visitor->visitNum(*this);
}

unique_ptr<Expr> Num::clone() {
    return make_unique<Num>(value);
}

String::String(string value) : Expr(ExpressionType::STRING), value(value) {}

void String::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void String::accept(ExpoSEVisitor *visitor) {
    // visitor->visitString(*this);
}

unique_ptr<Expr> String::clone() {
    return make_unique<String>(value);
}

Set::Set(vector<unique_ptr<Expr>> elements)
    : Expr(ExpressionType::SET), elements(move(elements)) {}

void Set::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void Set::accept(ExpoSEVisitor *visitor) {
    for (auto &e : elements)
    {
        visitor->visitExpr(*e);
    }
}

unique_ptr<Expr> Set::clone() {
    vector<unique_ptr<Expr>> clonedElements;
    for(auto &element:elements){
        clonedElements.push_back(element->clone());
    }

    return make_unique<Set>(move(clonedElements));
}

Map::Map(vector<pair<unique_ptr<Var>, unique_ptr<Expr>>>)
    : Expr(ExpressionType::MAP), value(move(value)) {}

void Map::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void Map::accept(ExpoSEVisitor *visitor) {
    for (auto &v : value)
    {
        visitor->visitVar(*(v.first));
        visitor->visitExpr(*(v.second));
    }
}

unique_ptr<Expr> Map::clone() {
    vector<pair<unique_ptr<Var>, unique_ptr<Expr>>> clonedValue;

    for (const auto& element : value) {
        auto clonedExpr = element.first->clone();
        auto rawPtr = dynamic_cast<Var*>(clonedExpr.get());

        if (!rawPtr) {
            throw runtime_error("Map key is not of type Var");
        }

        unique_ptr<Var> clonedVar(static_cast<Var*>(clonedExpr.release()));
        // unique_ptr<Var> clonedVar(clonedExpr);
        unique_ptr<Expr> clonedExprValue = element.second->clone();

        clonedValue.emplace_back(move(clonedVar), move(clonedExprValue));
    }

    return make_unique<Map>(move(clonedValue));
}

Tuple::Tuple(vector<unique_ptr<Expr>> exprs) :
    Expr(ExpressionType::TUPLE), expr(move(expr)) {}

void Tuple::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void Tuple::accept(ExpoSEVisitor *visitor) {
    for (auto &e : expr)
    {
        visitor->visitExpr(*e);
    }
}

unique_ptr<Expr> Tuple::clone() {
    vector<unique_ptr<Expr>> cloneexpr;
    for(auto &e:expr){
        cloneexpr.push_back(e->clone());
    }
    return make_unique<Tuple>(move(cloneexpr));
}

FuncDecl::FuncDecl(string name,
         vector<unique_ptr<TypeExpr>> params,
         pair<HTTPResponseCode, vector<unique_ptr<TypeExpr>>> returnType)
    : name(move(name)), params(move(params)), returnType(move(returnType)) {}

void FuncDecl::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void FuncDecl::accept(ExpoSEVisitor *visitor) {
    for(auto& te: returnType.second){
    }
}

Init::Init(string varName, unique_ptr<Expr> expression)
    : varName(move(varName)), expr(move(expression)) {}

void Init::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void Init::accept(ExpoSEVisitor *visitor) {
    visitor->visitExpr(*expr);
}

Response::Response(HTTPResponseCode code, unique_ptr<Expr> expr) :
    code(code), expr(move(expr)) {}

void Response::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void Response::accept(ExpoSEVisitor *visitor) {
    visitor->visitExpr(*expr);
}

void Response::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void Response::accept(ExpoSEVisitor *visitor) {
    visitor->visitFuncCall(*call);
}

APIcall::APIcall(unique_ptr<FuncCall> Call, Response Response) :
	call(move(Call)), response(move(Response)) {}

API::API(unique_ptr<Expr> precondition,
    unique_ptr<APIcall> functionCall,
    Response response)
    : pre(move(precondition)), call(move(functionCall)),
      response(move(response)) {}

void API::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void API::accept(ExpoSEVisitor *visitor) {
    visitor->visitExpr(*pre);
}

Spec::Spec(vector<unique_ptr<Decl>> globals,
     vector<unique_ptr<Init>> init,
     vector<unique_ptr<FuncDecl>> functions,
     vector<unique_ptr<API>> blocks)
    : globals(move(globals)), init(move(init)),
	functions(move(functions)), blocks(move(blocks)) {
}

void Spec::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void Spec::accept(ExpoSEVisitor *visitor) {
    for(auto& i: init){
        visitor->visitInit(*i);
    }

    for(auto& function: functions){
    }

    for(auto& block: blocks){
    }
}

Stmt::Stmt(StatementType type) : statementType(type) {}

Assign::Assign(unique_ptr<Var> left, unique_ptr<Expr> right)
    : Stmt(StatementType::ASSIGN), left(move(left)), right(move(right)) {}

void Assign::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void Assign::accept(ExpoSEVisitor *visitor) {
    visitor->visitVar(*left);
    visitor->visitExpr(*right);
}

unique_ptr<Stmt> Assign::clone() {
    // Clone the left-hand side; left->clone() returns a unique_ptr<Expr>.
    unique_ptr<Expr> leftCloneBase = left->clone();
    // Cast the result to a unique_ptr<Var>.
    Var* leftCloneRaw = dynamic_cast<Var*>(leftCloneBase.release());
    if (!leftCloneRaw) {
        throw runtime_error("Clone of left-hand side did not produce a Var instance");
    }
    unique_ptr<Var> clonedLeft(leftCloneRaw);

    // Clone the right-hand side normally (returns a unique_ptr<Expr>).
    unique_ptr<Expr> clonedRight = right->clone();

    // Return a new Assign node constructed with the cloned children.
    return make_unique<Assign>(move(clonedLeft), move(clonedRight));
}

FuncCallStmt::FuncCallStmt(unique_ptr<FuncCall> call)
    : Stmt(StatementType::FUNCTIONCALL_STMT), call(move(call)) {}

void FuncCallStmt::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void FuncCallStmt::accept(ExpoSEVisitor *visitor) {
    cout<<"Hey funcCall\n"; 
    visitor->visitFuncCall(*call);
}

unique_ptr<Stmt> FuncCallStmt::clone() {
    // Clone the function call; call->clone() returns a unique_ptr<Expr>.
    unique_ptr<Expr> callCloneBase = call->clone();
    // Cast the result to a unique_ptr<FuncCall>.
    FuncCall* callCloneRaw = dynamic_cast<FuncCall*>(callCloneBase.release());
    if (!callCloneRaw) {
        throw runtime_error("Clone of FuncCall did not produce a FuncCall instance");
    }
    unique_ptr<FuncCall> clonedCall(callCloneRaw);

    // Return a new FuncCallStmt node with the cloned FuncCall.
    return make_unique<FuncCallStmt>(move(clonedCall));
}

Program::Program(vector<unique_ptr<Stmt>> Statements)
    : statements(move(Statements)) {}

void Program::accept(ASTVisitor &visitor) {
    visitor.visit(*this);
}

void Program::accept(ExpoSEVisitor *visitor) {
    for (auto &stmt : statements)
    { // Use  reference to avoid unnecessary copies
        visitor->visitStmt(*stmt);
    }
}
