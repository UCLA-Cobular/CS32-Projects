void removeBad(list<Movie *> &li) {
    auto listIterator = li.begin();
    while (listIterator != li.end()) {
        if ((*listIterator)->rating() < 55) {
            delete *listIterator;
            listIterator = li.erase(listIterator);
        } else {
            listIterator++;
        }
    }
}
