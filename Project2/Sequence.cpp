//
// Created by cover on 1/20/2021.
//

#include "Sequence.h"
#include <iostream>


// At the end of this func, we will have a circular linked list with only one uninitialized head node and a
//  current size of 0
Sequence::Sequence() {
    m_current_size = 0;
    m_head = new Node();
    m_head->setNextNode(m_head);
    m_head->setPrevNode(m_head);
}

Sequence::~Sequence() {
    Node *current_pointer = m_head->getPrevNode();

    // Loop through, going to the next node then deleting the last one
    while (current_pointer != m_head) {
        Node *temp_node = current_pointer->getPrevNode();
        delete current_pointer;
        current_pointer = temp_node;
    }
    delete m_head;
}

Sequence::Sequence(const Sequence &sequence) {

}

Sequence &Sequence::operator=(const Sequence &sequence) {
    if (&sequence == this) return *this;
    return *this;
//    return nullptr;
}

bool Sequence::empty() const {
    if (m_head == m_head->getNextNode() && m_head == m_head->getPrevNode()) return true;
    return false;
}

int Sequence::size() const {
    return m_current_size;
}

// 4 cases:
//  this is the first node (pos = )
//  this is not the first node but is going at the beginning (pos = 0)
//  this is going somewhere in the middle
//  this is going at the end (call the other insert)
// Link forward new node to next node, so there are 2 forward pointers to the next node,
//  then change prev node's forward pointer to link to this node.
// Do this in reverse for the revese pointer.
int Sequence::insert(int pos, const ItemType &value) {
    // If pos is outside the size of the current array or is greater than or equal to the max number of items, fail.
    if (pos < 0 || pos > size())
        return -1;
    // Get references to the previous and next items
    Node *prev;
    get_node(0);
    Node *next;

    // Create a new item
    Node *new_item = new Node(value)

    return pos;
}

int Sequence::insert(const ItemType &value) {
    return 0;
}

bool Sequence::erase(int pos) {
    return false;
}

int Sequence::remove(const ItemType &value) {
    return 0;
}

bool Sequence::get(int pos, ItemType &value) const {
    if (pos > size() || pos < 0) return false;
    Node *current_pointer = first();
    for (int i = 0; i <= pos; ++i) {
        current_pointer = current_pointer->getNextNode();
    }
    return current_pointer->getNodeVal();
}

bool Sequence::set(int pos, const ItemType &value) {
    return false;
}

int Sequence::find(const ItemType &value) const {
    return 0;
}

void Sequence::swap(Sequence &other) {

}

Sequence::Node *Sequence::first() const {
    return m_head->getNextNode();
}

Sequence::Node *Sequence::last() const {
    return m_head->getPrevNode();
}

bool Sequence::loop_complete() {
    return false;
}

// You better be really quite sure that this pos is valid cause there's not really a safe return type for this
Sequence::Node *Sequence::get_node(int pos) {
    if (pos > size() || pos < 0) return nullptr;
    Node *current_pointer = first();
    for (int i = 0; i <= pos; ++i) {
        current_pointer = current_pointer->getNextNode();
    }
    return current_pointer;
}


Sequence::Node::Node() : m_prev(this), m_next(this) {
}

Sequence::Node::Node(const ItemType &val, Node *prev, Node *next) : m_val(val), m_prev(prev), m_next(next) {
}

void Sequence::Node::setNextNode(Node *next) {
    m_next = next;
}

void Sequence::Node::setPrevNode(Node *prev) {
    m_prev = prev;
}

Sequence::Node *Sequence::Node::getNextNode() {
    if (m_next != nullptr) return m_next;
    std::cerr << "Uh oh! getNextNode just tried to follow a null pointer!" << std::endl;
    return nullptr;
}

Sequence::Node *Sequence::Node::getPrevNode() {
    if (m_prev != nullptr) return m_prev;
    std::cerr << "Uh oh! prevNextNode just tried to follow a null pointer!" << std::endl;
    return nullptr;
}

void Sequence::Node::setNodeVal(const ItemType &val) {
    m_val = val;
}

ItemType Sequence::Node::getNodeVal() const {
    return m_val;
}


