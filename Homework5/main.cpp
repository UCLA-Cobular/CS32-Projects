#include <iostream>

class DoublyLinkedBinaryTree {
public:
    // Constructors - one for each use case
    DoublyLinkedBinaryTree(int data) : data(data), m_parent(nullptr), m_left_child(nullptr), m_right_child(nullptr) {}
    DoublyLinkedBinaryTree(int data, DoublyLinkedBinaryTree *parent) : data(data), m_parent(parent), m_left_child(nullptr), m_right_child(nullptr) {}
    DoublyLinkedBinaryTree(int data, DoublyLinkedBinaryTree *left_child, DoublyLinkedBinaryTree *right_child) : data(data), m_parent(nullptr), m_left_child(left_child), m_right_child(right_child) {}
    DoublyLinkedBinaryTree(int data, DoublyLinkedBinaryTree *parent, DoublyLinkedBinaryTree *left_child, DoublyLinkedBinaryTree *right_child) : data(data), m_parent(parent), m_left_child(left_child), m_right_child(right_child) {}

    // Linking funcs for re-attaching things at later times
    void linkParent(DoublyLinkedBinaryTree *parent) { m_parent = parent; }
    void linkLeftChild(DoublyLinkedBinaryTree *left_child) { m_left_child = left_child; }
    void linkRightChild(DoublyLinkedBinaryTree *right_child) { m_right_child = right_child; }

    // Accessors for following the pointers
    DoublyLinkedBinaryTree *parent() {return m_parent; }
    DoublyLinkedBinaryTree *left_child() {return m_left_child; }
    DoublyLinkedBinaryTree *right_child() {return m_right_child; }
private:
    // The member vars
    int data;
    DoublyLinkedBinaryTree *m_parent;
    DoublyLinkedBinaryTree *m_left_child;
    DoublyLinkedBinaryTree *m_right_child;
};



int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
