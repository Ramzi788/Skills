#include <iostream>
#include "mystack.cpp"

int main(){
    Stack<int> a;
    Stack<int> b;
    Stack<int>c;

    a.push(1);
    a.push(2);
    a.push(3);

    b.push(4);
    b.push(5);
    b.push(6);
    b.push(7);

    c= a+b;
    while(!c.empty()){
        cout << c.top() << endl;
        c.pop();
    }

}