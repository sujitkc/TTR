#include <iostream>
#include "unionfind.cc"

using namespace std;

class MyInt {
	private:
		const int value;
	public:
		MyInt(int v) : value (v) {} 
		int getValue() { return value; }
		string toString() { return to_string(value); }


		bool operator == (MyInt& i) {
    			return value == i.getValue();
		}
};

class Substitution : public UnionFind<MyInt> {
    public:
        virtual string valueToString(MyInt v) { return v.toString(); }
};

using namespace std;
int main() {
    try {
        Substitution s;
        MyInt a(1);
        s.addType(a);
        MyInt b(2);
        s.addType(b);
        s.print();
	s.myunion(a, b);
        s.print();
	cout << endl;
        MyInt c(3);
        s.addType(c);
        MyInt d(4);
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
