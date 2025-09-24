#ifndef UNIONFIND_HH
#define UNIONFIND_HH
#include <set>

using namespace std;
template <typename T> class Node {
    private:
        T& value;
        Node<T> *parent;
    public:
        Node(T&);
        T& getValue();
        Node<T> *getParent();
        Node<T> *getRoot();
        void setParent(Node<T> *);
};

template <typename T> class UnionFind {
    protected:
        set<Node<T> *> nodes;
        Node<T>* findNode(T&);
    public:
        virtual ~UnionFind();
	void addType(T&);
        T& find(T&);
        void myunion(T&, T&);
        void print();
        virtual string valueToString(T) = 0;
};

#endif //UNIONFIND_HH
