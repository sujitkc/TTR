#ifndef SOLVER_HH
#define SOLVER_HH

#include<map>
#include<memory>
#include<string>

#include "ast.hh"

using namespace std;

enum class ResultType {
    BOOL,
    INT
};

class ResultValue {
    public:
        const ResultType type;
        ResultValue(ResultType);
};

class BoolResultValue : public ResultValue {
    public:
        BoolResultValue(bool);
        const bool value;
};

class IntResultValue : public ResultValue {
    public:
        IntResultValue(int);
        const int value;
};

class Result {
    public:
        const bool isSat;
        const map<string, unique_ptr<ResultValue>> model; 
        Result(bool, map<string, unique_ptr<ResultValue> >);
};

class Solver {
    public:
        virtual Result solve(unique_ptr<Expr>) const = 0;
};
#endif
