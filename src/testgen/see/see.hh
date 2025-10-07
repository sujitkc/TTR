#ifndef SEE_HH
#define SEE_HH

#include<map>
#include<memory>
#include<string>

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

class SymExResult {
    public:
        const bool isSat;
        const map<string, unique_ptr<ResultValue>> model; 
        SymExResult(bool, map<string, unique_ptr<ResultValue> >);
};

class SEE {
    public:
        SymExResult execute();
};
#endif
