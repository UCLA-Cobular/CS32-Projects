void removeOdds(vector<int> &v) {
    auto vIterator = v.begin();
    while (vIterator != v.end()) {
        if (*vIterator % 2 == 1) {
            vIterator = v.erase(vIterator);
        } else {
            vIterator++;
        }
    }
}
