//
// Created by cover on 1/13/2021.
//

#include <iostream>
#include "Sequence.h"

Sequence::Sequence() {
    items = 0;
}

bool Sequence::empty() const {
    if (items == 0) return true;
    return false;
}

int Sequence::size() const {
    return items;
}

int Sequence::insert(int pos, const ItemType &value) {
    // If pos is outside the size of the current array or is greater than or equal to the max number of items, fail.
    if (pos < 0 || pos > size() || size() >= DEFAULT_MAX_ITEMS)
        return -1;
    // Bump items forward
    for (int i = size(); i > pos; --i)
        array[i] = array[i - 1];
    // Add the item at the newly freed spot
    array[pos] = value;
    // Increment items to keep size up to date
    items++;
    return pos;
}

int Sequence::insert(const ItemType &value) {
    if (size() >= DEFAULT_MAX_ITEMS) return -1;
    int p;
    for (p = 0; p < size(); ++p) {
        if (value <= array[p])
            break;
    }
    return insert(p, value);
}

bool Sequence::erase(int pos) {
    if (pos < 0 || pos >= size())
        return false;
    for (int i = pos + 1; i < size(); ++i) {
        array[i - 1] = array[i];
    }
    items--;
    return true;
}

int Sequence::remove(const ItemType &value) {
    int counter = 0;
    for (int i = 0; i < size(); ++i) {
        if (array[i] == value) {
            erase(i);
            ++counter;
            --i;
        }
    }
    return counter;
}

bool Sequence::get(int pos, ItemType &value) const {
    if (pos < 0 || pos >= size())
        return false;
    value = array[pos];
    return true;
}

bool Sequence::set(int pos, const ItemType &value) {
    if (pos < 0 || pos >= size())
        return false;
    array[pos] = value;
    return true;
}

int Sequence::find(const ItemType &value) const {
    for (int i = 0; i < size(); ++i) {
        if (array[i] == value) return i;
    }
    return -1;
}

void Sequence::swap(Sequence &other) {
    Sequence temp;
    ItemType value;

    // Copy this into temp
    temp.items = this->size();
    for (int i = 0; i < this->size(); ++i) {
        this->get(i, value);
        temp.array[i] = value;
    }

    // Copy other into this
    this->items = other.size();
    for (int i = 0; i < other.size(); ++i) {
        other.get(i, value);
        this->array[i] = value;
    }

    // Copy temp into other
    other.items = temp.size();
    for (int i = 0; i < temp.size(); ++i) {
        temp.get(i, value);
        other.array[i] = value;
    }
}

void Sequence::dump() const {
    int i;
    for (i = 0; i < size(); ++i) std::cerr << array[i] << "|";
    std::cerr << "|" << array[i] << "::" << size() << std::endl;
}
