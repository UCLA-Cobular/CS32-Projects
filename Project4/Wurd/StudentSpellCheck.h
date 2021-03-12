#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>


class StudentSpellCheck : public SpellCheck
{
public:
    StudentSpellCheck() { m_trie = new Trie(); }
    virtual ~StudentSpellCheck();
    bool    load(std::string dictionaryFile) override;
    bool    spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions) override;
    void    spellCheckLine(const std::string& line, std::vector<Position>& problems) override;

private:
    class Trie;
    class Node;

    /// <summary>
    /// Spellchecks and corrects the given word
    /// </summary>
    /// <param name="str">The given word</param>
    /// <param name="pos">The positionposition, should be zero when called</param>
    /// <param name="max_suggestions">The max num of suggestions to give. TODO: need to finish this part. </param>
    /// <param name="similarWords">The vector through which words will be returned</param>
    /// <param name="currentNode">The node to analyze the children of</param>
    /// <param name="path">A string representing the path taken to this word.</param>
    /// <param name="hasFoundDifference">If a difference has been found yet</param>
    /// <returns>True if the word is found exactly and false otherwise</returns>
    bool correctWord(
        const std::string& str, int pos, const int& max_suggestions, std::vector<std::string>& similarWords,
        Node*              currentNode, const std::string& path, bool hasFoundDifference = false);

    /// <summary>
    /// A simplified version of correctWord which just returns if a word exactly exists in the dictionary, without any suggestions. It's a good bit faster for sure. 
    /// </summary>
    /// <param name="str">The string to look for</param>
    /// <param name="currentNode">The current node, feed in the base node. </param>
    /// <param name="pos">The current position in the string, should be zero to start</param>
    /// <returns>True if the word is found exactly and false otherwise</returns>
    /// <returns>True if the word is found exactly and false otherwise</returns>
    bool isWordSpelledCorrect(const std::string& str, Node* currentNode, int pos = 0);

    static char        cleanChar(const char& ch);
    static std::string cleanStr(const std::string& str);


    Trie* m_trie;


    class Node
    {
    public:
        Node(char ch) : m_value(ch) {}
        virtual            ~Node() = default;
        char               value() const { return m_value; }
        std::vector<Node*> children() const { return m_children; }
        void               add_child(Node* node) { m_children.push_back(node); }
        int                checkChar(char) const;

        /// <summary>
        /// Tries to find the given char in the list of children
        /// </summary>
        /// <param name="ch">The character to look for</param>
        /// <returns>Either the pointer to said child or a nullptr if not found</returns>
        Node* findChild(char ch) const;

    private:
        char               m_value;
        std::vector<Node*> m_children;
    };


    class Trie
    {
    public:
        Trie();
        virtual ~Trie();
        Node*   baseNode() const { return m_base_node; }
        void    findOrCreateNodeForString(const std::string& str);

    private:
        static void deleteNode(Node* node);

        Node* m_base_node;
    };
};

#endif  // STUDENTSPELLCHECK_H_
