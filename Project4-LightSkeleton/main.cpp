//
// Created by jdc10 on 3/3/2021.
//

#include <string>
#include <cassert>
#include "StudentTextEditor.h"

using namespace std;

int main() {
    {
        string s = "afasdfasdf\n";
        assert(StudentTextEditor::cleanString(s) == "afasdfasdf");
    }
    {
        string s = "afasdfasdf\r";
        assert(StudentTextEditor::cleanString(s) == "afasdfasdf");
    }
    {
        string s = "afasdfasdf\n\r";
        assert(StudentTextEditor::cleanString(s) == "afasdfasdf");
    }
    {
        StudentTextEditor studentTextEditor;
        assert(studentTextEditor.load("/mnt/c/Users/jdc10/Documents/warandpeace.txt"));
        assert(studentTextEditor.save("/mnt/c/Users/jdc10/Documents/war_peace.txt"));
    }
}
