#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <locale>
#include <iostream>
#include <fstream>

using namespace std;

SpellCheck* createSpellCheck() { return new StudentSpellCheck; }

StudentSpellCheck::~StudentSpellCheck()
{
    // TODO
}

bool StudentSpellCheck::load(std::string dictionaryFile)
{
    ifstream infile(dictionaryFile); // infile is a name of our choosing
    if (!infile) // Did opening the file fail?
    {
        cerr << "Error: Cannot load file!" << endl;
        return false;
    }

    string s;
    while (getline(infile, s)) { m_trie->findOrCreateNodeForString(s); }
    return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions)
{
    return false; // TODO
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems)
{
    // TODO
}

/// <summary>
/// Normalized a character, nulling it if it's invalid
/// </summary>
/// <param name="ch">The character to check</param>
/// <returns>The lowercase version of the char, a ', or the null char if the char is not alpha or '</returns>
char StudentSpellCheck::cleanChar(const char& ch)
{
    if (isalnum(ch) || ch == '\'') return tolower(ch);
    return '\0';
}

std::string StudentSpellCheck::cleanStr(const std::string& str)
{
    string outputStr;
    for (char ch : str)
    {
        ch = cleanChar(ch);
        if (ch != '\0') outputStr += ch;
    }
    return outputStr;
}

StudentSpellCheck::Trie::Trie() { m_base_node = new Node('\0'); }

// Delete trie using a post-order traversal
StudentSpellCheck::Trie::~Trie() { deleteNode(m_base_node); }

// ReSharper disable once CppMemberFunctionMayBeConst
void StudentSpellCheck::Trie::findOrCreateNodeForString(std::string str)
{
    Node* currentNode = m_base_node;

    for (auto ch : str)
    {
        currentNode = currentNode->findChild(*str.begin());
        if (currentNode == nullptr)
        {
            Node* newNode = new Node(*str.begin());
            currentNode->add_child(newNode);
            currentNode = newNode;
        }
    }
}

/// <summary>
/// A post-order traversal based recursive node deletion
/// </summary>
/// <param name="node">The node to delete</param>
void StudentSpellCheck::Trie::deleteNode(Node* node)
{
    for (auto* child : node->children()) deleteNode(child);
    delete node;
    return;
}
