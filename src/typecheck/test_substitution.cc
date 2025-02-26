#include <iostream>
#include "substitution.cc"

class Substitution : public UnionFind<int> {};
using namespace std;
int main() {
    try {
        Substitution s;
        int a = 1;
        s.addType(a);
        int b = 2;
        s.addType(b);
        s.print();
	s.myunion(a, b);
        s.print();
	cout << endl;
        int c = 3;
        s.addType(c);
        int d = 4;
        s.addType(d);
        s.print();
	s.myunion(c, d);
        s.print();
	cout << endl;
	s.myunion(b, d);
	s.print();

        return 0;
    }
    catch(const char* s) {
        cout << s << endl;
        return 1;
    }
}
