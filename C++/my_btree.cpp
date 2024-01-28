#include <iostream>
using namespace std;

template <typename T>
struct treeNode {
    T data;
    struct treeNode *left;
    struct treeNode *right;
};
template <typename T>
class Btree {
    treeNode<T>* root; // Creating a root node for later use.
public:
    Btree(){
        root = nullptr;
    }
public:

    /*
     Requires: No Arguments Required.
     Effect: Adds a node at the highest possible level and fills from left to right.

     Test Cases:
     1. Add int/char node to left
     2. Add int/char node to right
     3. Empty Tree? --> Place Node at Root.
     4. No other types of data types are allowed as nodes.
     5. Different data types cannot be placed in the same tree (i.e. int with char)
     */
    void addNode(T data){
        root = addNode(root, data);
    }

    /*
     Requires: Node Pointer and Data for the Node.
     Effect: same Effects as addNode (T data).

     Test Cases:
     - Same as addNode(T data).
     */
    treeNode<T>* addNode(treeNode<T>* node, T data){
        if(node == nullptr){
            node = new treeNode<T>;
            node->data = data;
            node->left = node->right = nullptr;
        }

        else {
            if (minHeight(node->left) <= minHeight(node->right))
            {
                node->left = addNode(node->left, data);
            } else
            {
                node->right = addNode(node->right, data);
            }
        }
        return node;
    }

    /*
     Requires: Node Pointer
     Effect: Determines the highest possible level we can add the node at.
     */

    //Helper Method for addNode.
    int minHeight(treeNode<T>* node){
        if(node == nullptr)
            return 0;
        else
            return 1 + min(minHeight(node->left),minHeight(node->right));
    }

    /*
     Requires: Data for the Node we want to delete
     Effect: Finds the Node and deletes it. It deletes only if the node is a leaf node, and if not, prints that it is not possible.

     Test Cases:
        1. Delete int/char node from left.
        2. Delete int/char node from right.
        3. Tree Empty? --> Prints Cannot Delete Node
        4. No other data types are allowed.
     */
    void deleteNode(T data) {
        int counter = 0;
        bool findNode = false;
        root = deleteNode(root, data, &counter, &findNode);
        if (!findNode){
            cout<<"CANNOT DELETE NODE"<<endl;
        }
    }

    /*
     Requires: Node Pointer, Node Data, int pointer, boolean flag.
     Effect: Same as deleteNode(T data);

     Test Cases:
     - Same as deleteNode(T data);
     */

    //Helper method for deleteNode
    treeNode<T>* deleteNode(treeNode<T>* node, T data, int* count, bool* findNode){
        if(node == nullptr)
            return nullptr;
        else{
            if(node->data == data){
                if(node->left == nullptr && node->right == nullptr) {
                    *findNode = true;
                    return nullptr;
                }
                else
                    return node;
            }
            else{
                if(*findNode)
                    return node;

                node->left = deleteNode(node->left, data, count, findNode);
                node->right= deleteNode(node->right, data, count, findNode);
                return node;
            }
        }
    }


    /*
     Requires: No arguments required.
     Effect: Gets the size of tree.
     */
    int treeSize(){
        return treeSize(root);
    }

    /*
     Requires: Node Data
     Effect: Same as treeSize();
     */

    //Helper method for treeSize
    int treeSize(treeNode<T> * node) {
        if (node == nullptr) {
            return 0;
        } else return (1 + treeSize(node->left) + treeSize(node->right));
    }

    /*
     Requires: Node Data
     Effect: Gets the size of subtree.
     */
    int subTreeSize(T data){
        bool findNode = false;
        return subTreeSize(root, data, &findNode, 0);
    }
    /*
     Requires: Node pointer, Node Data, boolean flag, size of subtree.
     Effect: same as subTreeSize(T data);
     */

    //Helper method for subTreeSize
    int subTreeSize(treeNode<T> * node, T data, bool *findNode, int size) {
        if (node == nullptr || *findNode){
            return 0;
        }
        else{
            treeNode<T> * current = node;
            if (current->data == data){
                *findNode = true;
            }

            if (*findNode){
                size = treeSize(current);
                return size;
            }
            else
                return subTreeSize(current->left, data, findNode, size);
        }

    }
    /*
     Requires: No arguments required
     Effect: Print Data of Tree in order: left, right, parent (postOrder)
     */

    void postOrderTraversal(){
        postOrderTraversal(root);
    }

    /*
     Requires: Node Data
     Effect: same as postOrderTraversal()
     */

    //Helper method for postOrderTraversal
    void postOrderTraversal(treeNode<T> * node) {
        if (node == nullptr) {
            return;
        }
        postOrderTraversal(node->left);
        postOrderTraversal(node->right);
        cout << node->data << " ";
    }

    /*
     Requires: No arguments required
     Effect: Print Data of Tree in order: parent, left, right (preOrder)
     */
    void preOrderTraversal(){
        preOrderTraversal(root);
    }

    /*
     Requires: Node Data
     Effect: same as preOrderTraversal()
     */

    //Helper method for preOrderTraversal
    void preOrderTraversal(treeNode<T> * node) {
        if (node == nullptr) {
            return;
        }
        cout << node->data << " ";
        preOrderTraversal(node->left);
        preOrderTraversal(node->right);
    }

    /*
     Requires: No arguments required
     Effect: Print Data of Tree in order: left, parent, right (inOrder)
     */
    void inOrderTraversal(){
        inOrderTraversal(root);
    }

    /*
     Requires: No arguments required
     Effect: same as inOrderTraversal()
     */

    //Helper method for inOrderTraversal
    void inOrderTraversal(treeNode<T> * node) {
        if (node == nullptr) {
            return;
        }
        inOrderTraversal(node->left);
        cout << node->data<< " ";
        inOrderTraversal(node->right);

    }
};
