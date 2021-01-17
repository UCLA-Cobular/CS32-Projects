//
// Created by cover on 1/14/2021.
//

#include "newSequence.h"
#include <iostream>

Sequence::Sequence(): items(0),  max_items(DEFAULT_MAX_ITEMS){
    array = new ItemType[DEFAULT_MAX_ITEMS];
}

Sequence::Sequence(int len) : items(0), max_items(len) {
    array = new ItemType[len];
}

Sequence::~Sequence() {
    delete [] array;
}

Sequence::Sequence(const Sequence &src) {
    items = src.items;
    for (int i = 0; i < src.items; ++i) {
        this->array[i] = src.array[i];
    }
}

Sequence &Sequence::operator=(const Sequence &src) {
    if (&src == this) return *this;
    delete [] array;
    items = src.items;
    max_items = src.max_items;
    array = new ItemType[src.max_items];
    for (int i = 0; i < src.items; ++i) {
        this->array[i] = src.array[i];
    }
    return *this;
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
    if (pos < 0 || pos > size() || size() >= max_items)
        return -1;
    // Bump items forward
    if (size() > 0)
        for (int i = size(); i > pos; --i)
            array[i] = array[i - 1];
    // Add the item at the newly freed spot
    array[pos] = value;
    // Increment items to keep size up to date
    items++;
    return pos;
}

int Sequence::insert(const ItemType &value) {
    if (size() >= max_items) return -1;
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
    ItemType *tempArray = this->array;
    int tempItems = this->items;
    int tempMax = this->max_items;

    this->array = other.array;
    this->items = other.items;
    this->max_items = other.items;

    other.array = tempArray;
    other.items = tempItems;
    other.max_items = tempMax;
}

void Sequence::dump() const {
    int i;
    for (i = 0; i < size(); ++i) std::cerr << array[i] << "|";
    std::cerr << "|" << array[i] << "::" << size() << std::endl;
}

