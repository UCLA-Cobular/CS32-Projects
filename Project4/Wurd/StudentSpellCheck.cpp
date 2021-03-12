#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <locale>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

SpellCheck* createSpellCheck() { return new StudentSpellCheck; }

StudentSpellCheck::~StudentSpellCheck()
{
    delete m_trie;
}

bool StudentSpellCheck::load(std::string dictionaryFile)
{
    Trie* new_trie = new Trie();
    ifstream infile(dictionaryFile); // infile is a name of our choosing
    if (!infile) // Did opening the file fail?
    {
        cerr << "Error: Cannot load file!" << endl;
        return false;
    }

    string s;
    while (getline(infile, s)) { new_trie->findOrCreateNodeForString(s); }
    delete m_trie;
    m_trie = new_trie;
    return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions)
{
    vector<string> m_suggestions;
    const bool     spelledCorrect = correctWord(word, 0, max_suggestions, m_suggestions, m_trie->baseNode(), "");
    // The word is spelled correct
    if (spelledCorrect) { return true; }
    // The word is not spelled correct, so we need to try to switch in our suggestions then return false.
    // This applies even if our suggestions are empty, as we need to know if the word is spelled right either way.
    vector<string> m_correctNumSuggestions = {
        m_suggestions.begin(),
        // If there are less suggestions than the max, return all. Otherwise, return the max number.
        m_suggestions.size() < max_suggestions ? m_suggestions.end() : m_suggestions.begin() + max_suggestions
    };
    swap(m_correctNumSuggestions, suggestions);
    return false;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems)
{
    vector<SpellCheck::Position> words;
    int                          startOfWord = 0;
    int                          endOfWord   = 0;
    for (int i = 0; i < line.size(); i++)
    {
        char clean_ch = cleanChar(line[i]);
        // Char is clean, we're still in the middle of a word, so increment the end of the word
        if (clean_ch != '\0') { endOfWord++; }
        else
            // Char is unclean, so we've ended a word. If end of word != start of word, we actually have a word, so spellcheck it and commit to the list. Either way, reset both to i
        {
            if (endOfWord - startOfWord > 0)
            {
                // Weird ternary operator here accounts for the space or other garbage character in front of words in the middle of sentances, but not at the start
                if (!isWordSpelledCorrect(line.substr(startOfWord, endOfWord - startOfWord), m_trie->baseNode()))
                {
                    const Position position = { startOfWord, endOfWord - 1 };
                    problems.push_back(position);
                }
            }
            endOfWord   = i + 1;
            startOfWord = i + 1;
        }
    }

    // Need to run this one more time after the last word to finish it up
    if (endOfWord - startOfWord > 0)
    {
        // Weird ternary operator here accounts for the space or other garbage character in front of words in the middle of sentances, but not at the start
        if (!isWordSpelledCorrect(line.substr(startOfWord, endOfWord - startOfWord), m_trie->baseNode()))
        {
            const Position position = { startOfWord, endOfWord - 1 };
            problems.push_back(position);
        }
    }
    // TODO
}

bool StudentSpellCheck::correctWord(
    const std::string&       str, int                   pos, const int& max_suggestions, vector<string>& similarWords,
    StudentSpellCheck::Node* currentNode, const string& path, bool      hasFoundDifference)
{
    // We've gone too far, need to abort or we'll overread
    if (pos >= str.length())
    {
        // Check if we're at the end of a word. If so, we've either found our word and can return true or we've found a spelling suggestion 
        if (currentNode->findChild('.') != nullptr)
        {
            if (!hasFoundDifference) { return true; }
            similarWords.push_back(path);
            return false;
        }
        // If we're not at the end of a word, then we haven't found an alternate word so we can just return false
        return false;
    }

    bool status = false;

    // Check the children of this node
    char ch = cleanChar(str[pos]);
    // Somehow, we got a bad char in the string. Say something!
    if (ch == '\0') cerr << "Managed to get a bad char in the spellcheck thing. Oopsie!" << endl;
    Node* nextNode = currentNode->findChild(ch);
    // Child node exists with the correct position, so just go there
    if (nextNode != nullptr)
    {
        if (correctWord(str, pos + 1, max_suggestions, similarWords, nextNode, path + str[pos], hasFoundDifference))
            status = true;
    }
    // If we haven't found our difference yet, we should explore all other paths, so explore all children but mark that the difference has been found
    if (!hasFoundDifference)
    {
        for (Node* node : currentNode->children())
        {
            if (correctWord(str, pos + 1, max_suggestions, similarWords, node, path + node->value(), true)) status =
                true;
        }
    }
    // Child node doesn't exist and we have spent our one difference, so just give up on this branch
    return status;
}

bool StudentSpellCheck::isWordSpelledCorrect(const std::string& str, Node* currentNode, int pos)
{
    // We've gone too far, need to abort or we'll overread
    if (pos >= str.length())
    {
        // If the next char is the end word char, we've found our word! return true
        if (currentNode->findChild('.') != nullptr) { return true; }
            // Otherwise, we've missed the word and can return false
        else { return false; }
    }

    bool status = false;

    // Check the children of this node
    char ch = cleanChar(str[pos]);
    // Somehow, we got a bad char in the string. Say something!
    if (ch == '\0') cerr << "Managed to get a bad char in the spellcheck thing. Oopsie!" << endl;
    Node* nextNode = currentNode->findChild(ch);
    // Child node exists with the correct position, so follow that
    if (nextNode != nullptr) { return isWordSpelledCorrect(str, nextNode, pos + 1); }

    // If not, we don't have a match, return false.
    return false;
}

/// <summary>
/// Normalized a character, nulling it if it's invalid
/// </summary>
/// <param name="ch">The character to check</param>
/// <returns>The lowercase version of the char, a ', or \0 if the char is not alpha or '</returns>
char StudentSpellCheck::cleanChar(const char& ch)
{
    if (isalpha(ch) || ch == '\'') return tolower(ch);
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

StudentSpellCheck::Node* StudentSpellCheck::Node::findChild(char ch) const
{
    // Wacky lambda comparison to handle dereferencing the pointers in the Node* list.
    const auto result = find_if(m_children.begin(), m_children.end(), [&ch](const Node* node)
    {
        return node->value() == ch;
    });
    if (result == m_children.end()) return nullptr;
    return *result;
}

StudentSpellCheck::Trie::Trie() { m_base_node = new Node('\0'); }

// Delete trie using a post-order traversal
StudentSpellCheck::Trie::~Trie() { deleteNode(m_base_node); }

// ReSharper disable once CppMemberFunctionMayBeConst
void StudentSpellCheck::Trie::findOrCreateNodeForString(const std::string& str)
{
    string cleanString = cleanStr(str);
    cleanString += ".";
    Node* currentNode = baseNode();

    for (auto ch : cleanString)
    {
        Node* checkNode = currentNode->findChild(ch);
        if (checkNode == nullptr)
        {
            Node* newNode = new Node(ch);
            currentNode->add_child(newNode);
            currentNode = newNode;
        }
        else { currentNode = checkNode; }
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
