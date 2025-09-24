#ifndef UNIONFIND_CC
#define UNIONFIND_CC

#include <iostream>
#include <set>
#include <vector>

#include "unionfind.hh"

using namespace std;

template <typename T> Node<T>::Node(T& v) : value(v), parent(NULL) {}
template <typename T> T& Node<T>::getValue() { return value; }
template <typename T> Node<T> *Node<T>::getParent() { return parent; }
template <typename T> void Node<T>::setParent(Node<T> *p) { parent = p; }


template <typename T> Node<T>* UnionFind<T>::findNode(T& type) {
    Node<T>* node = NULL;
    for(auto& n : nodes) {
        if(n->getValue() == type) {
            node = n;
            break;
        }
    }
    return node;
}

template <typename T> Node<T>* Node<T>::getRoot() {
    if(parent == NULL) {
        return this;
    }
    else {
        return parent->getRoot();
    }
}

template <typename T> void UnionFind<T>::addType(T& type) {
    if(findNode(type) == 0) {
        Node<T> *node = new Node<T>(type);
        nodes.insert(node);
    }
    else {
        throw "Substition<T>::addType : type already exists.";
    }
}

template <typename T> T& UnionFind<T>::find(T& type) {
    Node<T> *node = findNode(type);
    return node->getRoot()->getValue();
}

template <typename T> void UnionFind<T>::myunion(T& t1, T& t2) {
    T& r1 = find(t1); // representative type 1
    T& r2 = find(t2); // representative type 2
    if(!(r1 == r2)) {
	    Node<T> *n1 = findNode(r1);
	    Node<T> *n2 = findNode(r2);
        n2->setParent(n1);
    }
}

template <typename T> void UnionFind<T>::print() {
    for(auto& node : nodes) {
        if(node->getParent() != NULL) {
            cout << valueToString(node->getValue()) << " : " << 
		    node->getParent()->getValue().toString() << endl;
        }
        else {
            cout << valueToString(node->getValue()) << " : " << "nil" << endl;
        }
    }
}

template <typename T> UnionFind<T>::~UnionFind() {
    for(auto& node : nodes) {
        cout << "node deleted." << endl;
        delete(node);
    }
    nodes.clear();
}
#endif // UNIONFIND_CC
