void removeOdds(vector<int> &v) {
    auto vIterator = v.begin();
    for (; vIterator < v.end(); vIterator++) {
        if (*vIterator % 2 == 1) {
            v.erase(vIterator);
            vIterator--;
        }
    }
}
