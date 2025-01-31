#include <iostream>
#include <bits/stdc++.h>

#include "ast.hh"
#include "codegen.hh"

// namespace codegen {
using std::regex;
using namespace ast;

string Visitor::pop(stack<string>& s) {
    string str = s.top();
    s.pop();
    return str;
}

string Visitor::retrieve() {
    return pop(strings);
}

void Visitor::visitExpression(Expression& e) {
    switch(e.exprtype) {
        case EMPTY: visitEmpty((Empty&)e); break;
        case VAR:   visitVar((Var&)e); break;
        case NUM:   visitNum((Num&)e); break;
        case ADD:   visitAddExpression((AddExpression&)e); break;
        case SUB:   visitSubExpression((SubExpression&)e); break;  
        case MUL:   visitMulExpression((MulExpression&)e); break;
        case DIV:   visitDivExpression((DivExpression&)e); break;
        case FUNCTIONCALL: visitFunctionCall((FunctionCall&)e); break;
        default: throw "Visitor::visitExpression - Unknown expression type.";
    }
}

void Visitor::visitStatement(Statement& s) {
    switch(s.stmttype) {
        case SKIP:     visitSkipStatement((SkipStatement&)s); break;
        case ASSIGN:   visitAssignmentStatement((AssignmentStatement&)s); break;
        case BRANCH:   visitBranchStatement((BranchStatement&)s); break;
        case LOOP:     visitLoopStatement((LoopStatement&)s); break;
        case SEQUENCE: visitSequenceStatement((SequenceStatement&)s); break;
        case BLOCK:    visitBlockStatement((BlockStatement&)s); break;
        case RETURN:   visitReturnStatement((ReturnStatement&)s); break;
        default: throw "Visitor::visitStatement - Unknown statement type.";
    }
}

CodeGenerator::CodeGenerator(Visitor *v) : visitor(v) {}

string CodeGenerator::indent(string line, string istring, int level) {
	string indentation = "";
	for(int i = 0; i < level; i++) {
		indentation += istring;
	}
	line = indentation + line;
	return line;
}

string CodeGenerator::generateCode(Program& program) {
    visitor->visitProgram(program);
    string raw = visitor->retrieve();
    regex del("\n");

    sregex_token_iterator it(raw.begin(), raw.end(), del, -1);
    sregex_token_iterator end;
    vector<string> lines;
    while (it != end) {
        lines.push_back(*it);
        ++it;
    }
	int level = 0;
    for(auto& line : lines) {
		if(line.find("{") != string::npos) {
			line = indent(line, "    ", level);
			level++;
		}
		else if(line.find("}") != string::npos) {
			level--;
			line = indent(line, "    ", level);
		}
		else {
			line = indent(line, "    ", level);
		}
    }
    string indentedCode = "";
    for(auto& line : lines) {
	    indentedCode += line + '\n';
    }
    return indentedCode;
}

CodeGenerator::~CodeGenerator() {
    delete visitor;
}

string CVisitor::string_of_type(Type t) {
    switch(t) {
    case INT: return "int"; break;
    case BOOL: return "bool"; break;
    case VOID: return "void"; break;
    default: throw "String representation of type not defined.";
    }
}

void CVisitor::visitDeclaration(Declaration& d) {
    d.accept(this);
    strings.push(string_of_type(d.getType()) + " " + d.getVariable());
}

void CVisitor::visitDeclarationList(DeclarationList& dlist) {
    dlist.accept(this);
    string str = "";
    for(unsigned int i = 0; i < dlist.getDeclarations().size(); i++) {
        str += pop(strings) + ";\n";
    }
    strings.push(str);
}

void CVisitor::visitEmpty(Empty& s) {
}

void CVisitor::visitNum(Num& n) {
    n.accept(this);
    strings.push(to_string(n.value));
}

void CVisitor::visitVar(Var& v) {
    v.accept(this);
    strings.push(v.name);
}

void CVisitor::visitAddExpression(AddExpression& e) {
    e.accept(this);
    string s1 = pop(strings);
    string s2 = pop(strings);
    string str = s1 + "+" + s2;
    strings.push(str);
}

void CVisitor::visitSubExpression(SubExpression& e) {
    e.accept(this);
    string s1 = pop(strings);
    string s2 = pop(strings);
    string str = s1 + "-" + s2;
    strings.push(str);
}

void CVisitor::visitMulExpression(MulExpression& e) {
    e.accept(this);

    string s1 = pop(strings);
    string s2 = pop(strings);
    string str = s1 + "*" + s2;
    strings.push(str);
}

void CVisitor::visitDivExpression(DivExpression& e) {
    e.accept(this);

    string s1 = pop(strings);
    string s2 = pop(strings);
    string str = s1 + "/" + s2;
    strings.push(str);
}

void CVisitor::visitFunctionCall(FunctionCall& e) {
    e.accept(this);
}

void CVisitor::visitSkipStatement(SkipStatement& s) {
}

void CVisitor::visitAssignmentStatement(AssignmentStatement& s) {
    s.accept(this);

    string s1 = s.getVariable();
    string s2 = pop(strings);
    string str = s1 + "=" + s2 + ';';
    strings.push(str);
}

void CVisitor::visitSequenceStatement(SequenceStatement& s) {
    s.accept(this);
    string str = "";
    for(unsigned int i = 0; i < s.getStatements().size(); i++) {
        str += strings.top() + '\n';
        strings.pop();
    }
    strings.push(str);
}

void CVisitor::visitBranchStatement(BranchStatement& s) {
    s.accept(this);

    string scond = pop(strings);
    string sthen = pop(strings);
    string selse = pop(strings);
    string str = "if(";
    str += scond + ")\n";
    str += sthen + "\n";
    str += "}\n";
    str += "else\n";
    str += "{\n";
    str += selse + "\n";
    str += "}\n";
    strings.push(str);
}

void CVisitor::visitLoopStatement(LoopStatement& s) {
    s.accept(this);
    string scond = pop(strings);
    string sbody = pop(strings);
    string str = "while(";
    str += scond + ")\n";
    str += "{\n";
    str += sbody + "\n";
    str += "}\n";
    strings.push(str);
}

void CVisitor::visitBlockStatement(BlockStatement& s) {
    s.accept(this);
    string sbody = pop(strings);
    string sdecs = pop(strings);
    string str = "{\n";
    str += sdecs + sbody + "\n";
    str += "}\n";
    strings.push(str);
}

void CVisitor::visitReturnStatement(ReturnStatement& s) {
    s.accept(this);
    string se = pop(strings);
    string str = "return " + se + ';';
    strings.push(str);
}

void CVisitor::visitFunctionDefinition(FunctionDefinition& fd) {
    fd.accept(this);
    string str = string_of_type(fd.getReturnType()) + " " + fd.getName() + "(";
    str += pop(strings) + ")\n";
    str += pop(strings) + "\n";
    strings.push(str);
}

void CVisitor::visitParameterList(DeclarationList& dlist) {
    dlist.accept(this);
    string str = "";
    vector<Declaration *> ds = dlist.getDeclarations();
    for(unsigned int i = 0; i < ds.size() - 1; i++) {
        str += pop(strings) + ",";
    }
    str += pop(strings);
    strings.push(str);
}

void CVisitor::visitFunctionDefinitionList(FunctionDefinitionList& fdlist) {
    fdlist.accept(this);
    string str = "";
    for(auto& _ : fdlist.getFunctionDefinitions()) {
        str += pop(strings);
    }
    strings.push(str);
}

void CVisitor::visitProgram(Program& program) {
    program.accept(this);
    string s1 = pop(strings);
    string s2 = pop(strings);
    string s3 = pop(strings);
    string str = "// " + program.getName() + 
        "\n" + s1 + "\n" + s2 + "\n" + s3 + "\n";
    strings.push(str);
}


// } // namespace codegen
