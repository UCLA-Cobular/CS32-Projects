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
    Node *currentPointer = m_head->getPrevNode();

    // Loop through, going to the next node then deleting the last one
    while (currentPointer != m_head) {
        Node *tempNode = currentPointer->getPrevNode();
        delete currentPointer;
        currentPointer = tempNode;
    }
    delete m_head;
}

Sequence::Sequence(const Sequence &sequence) {
    // Size will be made the same by the insert func
    m_current_size = 0;
    m_head = new Node();
    m_head->setNextNode(m_head);
    m_head->setPrevNode(m_head);
    // Basic thing is created, now stitch in other nodes
    int counter;
    for (counter = 0; counter < sequence.size(); ++counter) {
        this->insert(counter, sequence.get_node(counter)->getNodeVal());
    }
}

/// This works by creating a temp sequence as a copy of the input sequence, then swapping it's values with `this`, so
///  `this` is now an exact copy of the input sequence. The temp_sequence, which now contains the values that were in
///  `this`, gets destroyed by the destructor when leaving scope.
/// @param sequence The input sequence to write over this one.
/// @return *`this`, which is now an exact copy of the input sequence
Sequence &Sequence::operator=(const Sequence &sequence) {
    if (&sequence == this) return *this;

    Sequence temp_sequence(sequence);
    this->swap(temp_sequence);

    return *this;
}

bool Sequence::empty() const {
    if (m_head == m_head->getNextNode() && m_head == m_head->getPrevNode()) return true;
    return false;
}

int Sequence::size() const {
    return m_current_size;
}

/// Inserts an item in pos with value value. Creates a new node, doesn't just override.
/// @param pos Zero indexed position where the node will end up
/// @param value The value of type ItemType to put in pos
/// @return -1 if pos is not valid, pos otherwise
int Sequence::insert(int pos, const ItemType &value) {
    // If pos is outside the size of the current array or is greater than or equal to the max number of items, fail.
    if (pos < 0 || pos > size())
        return -1;

    // Get references to the previous and next items
    Node *next = get_node(pos);
    if (next == nullptr) report_null_prt_err();
    Node *prev = next->getPrevNode();
    if (prev == nullptr) report_null_prt_err();

    // Create a new item and link it into the list
    Node *new_node = new Node(value, next, prev);

    prev->setNextNode(new_node);
    next->setPrevNode(new_node);

    m_current_size++;

    return pos;
}

int Sequence::insert(const ItemType &value) {
    Node *search_node = first();
    int index = 0;

    // Loop over the entire list, until the node to search is the head node
    for (;;) {
        // Break out if loop comes back to the head
        if (search_node == m_head) break;
        // Break out if our result is found
        if (value <= search_node->getNodeVal()) break;
        index++;
        search_node = search_node->getNextNode();
    }
    return insert(index, value);
}

/// If 0 <= pos < size(), remove the item at position pos and return true. Otherwise false
/// @param pos The position of the item to remove
/// @return True if item at pos can be erased, false otherwise (no change)
bool Sequence::erase(int pos) {
    if (0 <= pos && pos < size()) {
        Node *nodeToRemove = get_node(pos);
        if (nodeToRemove == nullptr) report_null_prt_err();

        Node *prev = nodeToRemove->getPrevNode();
        Node *next = nodeToRemove->getNextNode();

        delete nodeToRemove;

        prev->setNextNode(next);
        next->setPrevNode(prev);

        m_current_size--;

        return true;
    }
    return false;
}

/// Remove all nodes with value from the list
/// @param value The value to use to search for nodes to remove
/// @return The number of items removed
int Sequence::remove(const ItemType &value) {
    Node *search_node = first();
    int counter = 0;

    // Scan the entire list, until the node to search is the head node
    for (;;) {
        // Store the next node pointer before we delete the node
        Node *next_node = search_node->getNextNode();
        if (search_node->getNodeVal() == value) {
            erase(search_node);
            counter++;
        };
        search_node = next_node;
        if (search_node == m_head) break;
    }

    return counter;
}

/// Returns the value of the node at said position.
/// @param pos The desired 0 indexed position in the list of the requested item.
/// @param value A variable that will be set to the value of the node, if the position is valid
/// @return True if a node is found and false otherwise.
bool Sequence::get(int pos, ItemType &value) const {
    if (pos >= size()) return false;
    Node *node = get_node(pos);
    if (node != nullptr) {
        value = node->getNodeVal();
        return true;
    }
    return false;
}

/// If pos is valid, replace the value of the node at pos with the given value.
/// @param pos The position to replace at
/// @param value The value to replace with
/// @return True if a value is set and false otherwise
bool Sequence::set(int pos, const ItemType &value) {
    if (0 <= pos && pos < size()) {
        Node *node = get_node(pos);
        if (node == nullptr) report_null_prt_err();

        node->setNodeVal(value);
        return true;
    }
    return false;
}

/// Gets the position of the element with value `value` in the list, or -1 if not found
/// @param value The value to search for
/// @return The zero-indexed position in the of the item if found, or -1 otherwise.
int Sequence::find(const ItemType &value) const {
    Node *search_node = first();
    int index = 0;

    // Loop over the entire list, until the node to search is the head node
    for (;;) {
        if (search_node == m_head) return -1;
        if (search_node->getNodeVal() == value) return index;
        index++;
        search_node = search_node->getNextNode();
    }
}

void Sequence::swap(Sequence &other) {
    Node *temp_head = other.m_head;
    other.m_head = this->m_head;
    this->m_head = temp_head;
    int temp_length = other.m_current_size;
    other.m_current_size = this->m_current_size;
    this->m_current_size = temp_length;
}

/// Returns the first real node in the array. It's what the head node points to.
/// @return the first node
Sequence::Node *Sequence::first() const {
    return m_head->getNextNode();
}

/// Gets a node of value pos in the sequence. Will return nullptr if the pos is out of range, so don't be dumb.
/// @param pos The zero-indexed position of the node you're looking for.
/// @return the pointer to the node at said position, or the nullptr if the node is going somewhere impossible.
Sequence::Node *Sequence::get_node(int pos) const {
    if (pos > size() || pos < 0) return nullptr;
    Node *current_pointer = first();
    for (int i = 0; i <= pos - 1; ++i) {
        current_pointer = current_pointer->getNextNode();
    }
    return current_pointer;
}

void Sequence::report_null_prt_err() const {
#ifdef DEVELOPING
    std::cerr << "About to follow a null ptr! Aborting..." << std::endl;
    exit(-1);
#endif
}

void Sequence::erase(Sequence::Node *target) {
    if (target == nullptr) report_null_prt_err();
    Node *prev = target->getPrevNode();
    Node *next = target->getNextNode();

    delete target;

    prev->setNextNode(next);
    next->setPrevNode(prev);
    m_current_size--;
}

void Sequence::dump() const {
    Node *search_node = first();

    // Loop over the entire list, until the node to search is the head node
    for (;;) {
        std::cerr << search_node->getNodeVal() << "|";
        search_node = search_node->getNextNode();
        if (search_node == m_head) break;
    }
    std::cerr << std::endl;
}

/// Constructs a head node, using itself as a prev and next pointer. This should only be used for the head.
/// Does __not__ initialize the value to anything either.
Sequence::Node::Node() : m_prev(this), m_next(this) {
}

/// Constructs a node, given a value and a next and previous node. This basically does the linking for you.
/// @param val The value to assign this node
/// @param prev The forward pointer
/// @param next The reverse pointer
Sequence::Node::Node(const ItemType &val, Node *next, Node *prev) : m_val(val), m_prev(prev), m_next(next) {
}

/// Sets the forward pointer for this node
/// @param next The node to set the forward pointer to
void Sequence::Node::setNextNode(Node *next) {
    m_next = next;
}

/// Sets the reverse pointer for this node
/// @param prev The node to set the reverse pointer to
void Sequence::Node::setPrevNode(Node *prev) {
    m_prev = prev;
}

/// Gets the next node in the list.
/// @return A pointer to the next node down the line.
Sequence::Node *Sequence::Node::getNextNode() {
    if (m_next != nullptr) return m_next;
    std::cerr << "Uh oh! getNextNode just tried to follow a null pointer!" << std::endl;
    return nullptr;
}

/// Gets the previous node in the list.
/// @return A pointer to the previous node up the line.
Sequence::Node *Sequence::Node::getPrevNode() {
    if (m_prev != nullptr) return m_prev;
    std::cerr << "Uh oh! prevNextNode just tried to follow a null pointer!" << std::endl;
    return nullptr;
}

/// Sets the value of the current node
/// @param val The value to set the node to
void Sequence::Node::setNodeVal(const ItemType &val) {
    m_val = val;
}

/// Accesses the node's value
/// @return the value stored in the node.
ItemType Sequence::Node::getNodeVal() const {
    return m_val;
}


/// Returns the start of the seq2 consecutively in seq1, if it exists, or returns -1
/// @param seq1 The sequence to find subsequence seq2 in.
/// @param seq2 The subsequence to find inside seq2
/// @return The position of the start of the subsequence in the overall sequence, or -1 if not found
int subsequence(const Sequence &seq1, const Sequence &seq2) {
    // The plan: Compare the first value in seq2 to the current item in seq1, incrementing seq1_count by one until something is found.
    //  Then, store the position in seq1 in seq1_saved_pos and go to the check subsystem.
    //   Check subsystem can either succeed, in which case return seq1_count
    //   Check subsystem can fail, in which case resume checking back at seq1_count
    // If nothing is found, return -1.
    if (seq2.size() == 0) return -1;

    ItemType seq1_val;
    ItemType seq2_val;
    // This skips loops where there are less items left than seq2, in which case a match is impossible
    int end_loop = seq1.size() - seq2.size() + 1;

    for (int seq1_count = 0; seq1_count < end_loop; ++seq1_count) {
        seq1.get(seq1_count, seq1_val);
        seq2.get(0, seq2_val);
        // First item match has been found, need to do check subsystem
        if (seq1_val == seq2_val) {
            bool match_failed = false;
            for (int i = 1; i < seq2.size(); ++i) {
                seq1.get(seq1_count + i, seq1_val);
                seq2.get(i, seq2_val);
                // If at any point in the loop, seq1 and seq2 don't match, that means the match has failed so we need
                //  to set the flag and break out
                if (seq1_val != seq2_val) {
                    match_failed = true;
                    break;
                }
            }
            // If we get here without the flag being set, we know that the check has passed!
            // That means we just return seq1_count, which stores the index of the start of the subsequence.
            if (!match_failed) return seq1_count;
        }
    }

    // If we've gotten to here, that means we've failed and need to return -1.
    return -1;
}

void interleave(const Sequence &seq1, const Sequence &seq2, Sequence &result) {
    // This way we can make sure result is empty without having to know if it was dynamically allocated or allocated at compiletime.
    Sequence temp_sequence = Sequence();
    // Cover the edge cases of one of the list being zero
    if (seq1.size() == 0 && seq2.size() == 0) {
        result = temp_sequence;
        return;
    }
    else if (seq1.size() == 0) {
        temp_sequence = seq2;
        result = temp_sequence;
        return;
    } else if (seq2.size() == 0) {
        temp_sequence = seq1;
        result = temp_sequence;
        return;
    }

    int max = seq1.size() > seq2.size() ? seq1.size() : seq2.size();

    ItemType item;

    for (int i = 0; i < max; ++i) {
        if (i < seq1.size()) {
            seq1.get(i, item);
            temp_sequence.insert(temp_sequence.size(), item);
        }
        if (i < seq2.size()) {
            seq2.get(i, item);
            temp_sequence.insert(temp_sequence.size(), item);
        }
    }
    result = temp_sequence;
}
