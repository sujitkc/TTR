#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H
#include <set>

using namespace std;
template <typename T> class Node {
    private:
        T value;
        Node<T> *parent;
    public:
        Node(T&);
        T& getValue();
        Node<T> *getParent();
	Node<T>* getRoot();
        void setParent(Node<T> *);
};

template <typename T> class Substitution {
    private:
        set<Node<T> *> typeNodes;
        Node<T>* findTypeNode(T&);
    public:
        void addType(T&);
        T& find(T&);
        void myunion(T&, T&);
	void print();

};
#endif
