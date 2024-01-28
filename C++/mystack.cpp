#include <iostream>
#include <vector>
using namespace std;


template <typename T>
class Stack {
public:
    vector<T> v;

    /*
    Requires: No Arguments
    Effect: Checks if the Stack is Empty. If the return value is 1, then the stack is empty, if 0 then it is not.
    */
    bool empty() {
        return v.empty();
    }

    /*
     Requires:  A constant value that is pushed to Stack
     Effect: Pushes a const element into top of stack.

     Test Cases:
     1. Place only int or char.
     */
    void push(const T &item) {
        v.push_back(item);
    }

    /*
    Requires: No arguments required.
    Effect: Returns the top element of the stack. However, if the stack is empty an error is thrown.

    Test Cases:
     1. Cannot return the top element if stack is empty. So error is thrown.
    */
    T &top() {
        if (v.empty()) {
            throw invalid_argument("Empty Stack!");
        }
        T &temp = v[v.size() - 1];
        return temp;
    }

    /*
    Requires: No arguments required.
    Effect: Prints and then removes the top element of the stack. Then stack size shrinks by 1.
    */
    void pop() {
        if (v.empty()) {
            cout << "Empty" << "\n";
        } else
            top();
        v.pop_back();
        v.shrink_to_fit();
    }

    /*
    Requires: Two stacks a & b.
    Effect: Returns a stack, c, that combines the elements of a and b, where the elements of a are placed first followed by b.

     Test Cases:
     1. If stacks are empty, we can't add to c.
     2. If a is empty, c only contains b.
     3. If b is empty, c only contains a.
     4. Can have strings, int and char.
     5. Stacks a & b can have different sizes.
    */
    friend Stack<T> operator+(Stack<T> a, Stack<T> b) {
        Stack<T> c;
        if (a.empty()) {
            return b;
        }

        if (b.empty()) {
            return a;
        }

        if (a.empty() && b.empty()) {
            return c;
        }

        for (int i = 0; i < a.v.size(); i++) {
            c.push(a.v[i]);
        }
        for (int j = 0; j < b.v.size(); j++) {
            c.push(b.v[j]);
        }
        return c;
    }
};

