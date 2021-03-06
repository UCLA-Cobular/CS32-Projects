#include "StudentTextEditor.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

StudentTextEditor::StudentTextEditor(Undo *undo) {
    // TODO
}

StudentTextEditor::~StudentTextEditor() {
    // TODO
}

bool StudentTextEditor::load(std::string file) {
    ifstream infile(file);  // infile is a name of our choosing
    if (!infile)  // Did opening the file fail?
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    string s;
    while (getline(infile, s)) {
        linesList.push_back(cleanString(s));
    }
    return true;
}

bool StudentTextEditor::save(std::string file) {
    ofstream outfile(file);  // outfile is a name of our choosing
    if (!outfile)  // Did opening the file fail?
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    for (const auto& line : linesList) {
        outfile << line << '\n';
    }
    return true;
}

std::string StudentTextEditor::cleanString(std::string string) {
    string.erase(remove(string.begin(), string.end(), '\r'), string.end());
    string.erase(remove(string.begin(), string.end(), '\n'), string.end());
    return string;
}
