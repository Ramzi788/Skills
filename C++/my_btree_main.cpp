#include <iostream>
#include "my_btree.cpp"
using namespace std;

int main(){
    Btree<int> node;

    node.addNode(1);
    node.addNode(2);
    node.addNode(3);


    node.preOrderTraversal();
    cout<<""<<endl;
    node.postOrderTraversal();
    cout<<""<<endl;
    node.inOrderTraversal();
    cout<<""<<endl;

    node.deleteNode(2);
    cout<<node.treeSize()<<endl;
    node.deleteNode(1);
    cout<<node.treeSize()<<endl;
}