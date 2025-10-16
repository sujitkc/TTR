#pragma once
#include <map>
#include <string>

#include "ast.hh"

using namespace std;
template<typename T1, typename T2> class Env {
    protected:
        map<T1 *, T2 *> table;
        Env<T1, T2> *parent;

    public:
        Env(Env<T1, T2> *parent);
        virtual Env<T1, T2> *getParent();
        virtual T2& get(T1 *);
        virtual bool hasKey(T1 *);
        virtual void addMapping(T1 *, T2 *);
        virtual string keyToString(T1 *) = 0;
        virtual void print() = 0;
        virtual ~Env();
};

class SymbolTable : public Env<string, TypeExpr> {
    public:
        SymbolTable(SymbolTable *parent);
        virtual void print();
        virtual string keyToString(string *);
};

