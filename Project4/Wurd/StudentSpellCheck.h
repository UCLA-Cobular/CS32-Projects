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

    static char        cleanChar(const char& ch);
    static std::string cleanStr(const std::string& str);

    Trie* m_trie;

    class Node
    {
        friend StudentSpellCheck;
        friend Trie;
    public:
        Node(char ch) : m_value(ch) {}
        virtual            ~Node() = default;
        char               value() const { return m_value; }
        std::vector<Node*> children() const { return m_children; }
        void               add_child(Node* node) { m_children.push_back(node); }

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
        void    findOrCreateNodeForString(std::string str);

    private:
        static void deleteNode(Node* node);

        Node* m_base_node;
    };
};

#endif  // STUDENTSPELLCHECK_H_
