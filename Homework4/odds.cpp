//
// Created by jdc10 on 2/10/2021.
//

#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

using namespace std;

// Remove the odd integers from v.
// It is acceptable if the order of the remaining even integers is not
// the same as in the original vector.
void removeOdds(vector<int> &v) {
    auto vIterator = v.begin();
    for (; vIterator < v.end(); vIterator++) {
        if (*vIterator % 2 == 1) {
            v.erase(vIterator);
            vIterator--;
        }
    }
}

void test() {
    int a[8] = {2, 8, 5, 6, 7, 3, 4, 1};
    vector<int> x(a, a + 8);  // construct x from the array
    assert(x.size() == 8 && x.front() == 2 && x.back() == 1);
    removeOdds(x);
    assert(x.size() == 4);
    sort(x.begin(), x.end());
    int expect[4] = {2, 4, 6, 8};
    for (int k = 0; k < 4; k++)
        assert(x[k] == expect[k]);
}

int main() {
    test();
    cout << "Passed" << endl;
}
