#include "StudentTextEditor.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include "Undo.h"
#include <string>
#include <vector>

using namespace std;

TextEditor* createTextEditor(Undo* un) { return new StudentTextEditor(un); }

StudentTextEditor::StudentTextEditor(Undo* undo)
    : TextEditor(undo), m_line(0), m_col(0)
{
    m_linesList.emplace_back("");
    m_lineIterator = m_linesList.begin();
    // TODO
}

StudentTextEditor::~StudentTextEditor()
{
    // TODO
}

bool StudentTextEditor::load(std::string file)
{
    ifstream infile(file); // infile is a name of our choosing
    if (!infile) // Did opening the file fail?
    {
        cerr << "Error: Cannot load file!" << endl;
        return false;
    }

    // This reset is a bit different and requires being split up, so we need to just do it ourselves.
    m_linesList.clear();
    m_col  = 0;
    m_line = 0;

    string s;
    while (getline(infile, s)) { m_linesList.push_back(cleanString(s)); }
    m_lineIterator = m_linesList.begin();

    // Check for empty list, in which we need to create a single row
    if (m_linesList.empty()) { m_linesList.emplace_back(""); }
    // Make sure the iterator is still good
    m_lineIterator = m_linesList.begin();
    return true;
}

bool StudentTextEditor::save(std::string file)
{
    ofstream outfile(file); // outfile is a name of our choosing
    if (!outfile) // Did opening the file fail?
    {
        std::cerr << "Error: Cannot save file!" << endl;
        return false;
    }
    for (const auto& line : m_linesList) { outfile << line << '\n'; }
    return true;
}

void StudentTextEditor::reset()
{
    m_linesList.clear();
    m_linesList.emplace_back("");
    m_lineIterator = m_linesList.begin();
    m_col          = 0;
    m_line         = 0;
    // TODO: Reset undo stack
}

#pragma region Directional Movement
void StudentTextEditor::move_up()
{
    if (m_line > 0)
    {
        --m_lineIterator;
        --m_line;
        reEvaluateColPos();
    }
}

void StudentTextEditor::move_down()
{
    // Needs to fail if the list size is 1 and the line is 0
    if (m_line < m_linesList.size() - 1)
    {
        ++m_lineIterator;
        ++m_line;
        reEvaluateColPos();
    }
}

void StudentTextEditor::move_left()
{
    if (m_col > 0) --m_col;
    else
    {
        const int prev_line = m_line;
        move_up();
        // If the line changed, then we know that we need to reset the column to zero because we went off the right edge actually to somewhere.
        if (prev_line != m_line) { m_col = m_lineIterator->size(); }
    }
}

void StudentTextEditor::move_right()
{
    if (m_col < m_lineIterator->size()) ++m_col;
    else
    {
        const int prev_line = m_line;
        move_down();
        // If the line changed, then we know that we need to reset the column to zero because we went off the right edge actually to somewhere.
        if (prev_line != m_line) { m_col = 0; }
    }
}

void StudentTextEditor::move_home() { m_col = 0; }

int StudentTextEditor::move_end() { return m_col = m_lineIterator->size(); }

void StudentTextEditor::move(Dir dir)
{
    switch (dir)
    {
    case UP:
        move_up();
        break;
    case DOWN:
        move_down();
        break;
    case LEFT:
        move_left();
        break;
    case RIGHT:
        move_right();
        break;
    case HOME:
        move_home();
        break;
    case END:
        move_end();
        break;
    default:
        cerr << "Something has gone horribly wrong with Move!" << endl;
    }
}
#pragma endregion


bool StudentTextEditor::isAtLineEnd() const { return m_col == m_lineIterator->size(); }

void StudentTextEditor::raw_del_end_of_line()
{
    // Combine the current and below lines
    string combinedString;
    combinedString += (*m_lineIterator);
    ++m_lineIterator;
    combinedString += (*m_lineIterator);
    m_lineIterator = m_linesList.erase(m_lineIterator); // Toss the line we're on right now
    --m_lineIterator;
    (*m_lineIterator) = combinedString;
}

char StudentTextEditor::raw_del_middle()
{
    const char deleted = m_lineIterator->at(m_col);
    m_lineIterator->erase(m_lineIterator->begin() + m_col);
    return deleted;
}

void StudentTextEditor::del()
{
    // Handle end of line case, where we need to stitch two lines back together
    if (isAtLineEnd())
    {
        // Do nothing if end of the final line (one less than the size)
        if (m_line == m_linesList.size() - 1) return;
        raw_del_end_of_line();
        getUndo()->submit(Undo::JOIN, m_line, m_col, '\0');
        return;
    }

    // Handle the normal "delete in the middle" case
    const char deleted = raw_del_middle();
    getUndo()->submit(Undo::DELETE, m_line, m_col, deleted);
}

void StudentTextEditor::raw_backspace_start_of_line()
{
    string combinedString;
    combinedString += (*m_lineIterator);
    m_lineIterator = m_linesList.erase(m_lineIterator);
    --m_lineIterator; // We are now on the previous line from where we started
    m_col = m_lineIterator->size();
    m_lineIterator->append(combinedString);
    m_line--;
}

char StudentTextEditor::raw_backspace_middle()
{
    const char deleted = m_lineIterator->at(m_col - 1);
    m_lineIterator->erase(m_lineIterator->begin() + m_col - 1);
    --m_col;
    return deleted;
}

void StudentTextEditor::backspace()
{
    // Handle start of line case
    if (m_col == 0)
    {
        // Do nothing if on the first (zeroth indexed) line
        if (m_line == 0) return;
        raw_backspace_start_of_line();
        getUndo()->submit(Undo::JOIN, m_line, m_col, '\0');
        return;
    }
    // Handle the normal "delete in the middle" case
    char deleted = raw_backspace_middle();
    getUndo()->submit(Undo::DELETE, m_line, m_col, deleted);
}

void StudentTextEditor::raw_insert(char ch)
{
    // Store the char into the undo stack, including the tab char as a tab.
    if (ch == '\t')
    {
        m_lineIterator->insert(m_col, 4, ' ');
        m_col += 4;
    }
    else
    {
        m_lineIterator->insert(m_col, 1, ch);
        m_col++;
    }
}

void StudentTextEditor::insert(char ch)
{
    raw_insert(ch);
    getUndo()->submit(Undo::INSERT, m_line, m_col, ch);
}

void StudentTextEditor::raw_enter()
{
    if (isAtLineEnd())
    {
        ++m_lineIterator;
        m_linesList.insert(m_lineIterator, "");
        --m_lineIterator;
        m_line++;
        m_col = 0;
    }
    else
    {
        // Handle cursor in middle of line case
        const string str1 = m_lineIterator->substr(0, m_col);
        const string str2 = m_lineIterator->substr(m_col, m_lineIterator->size());
        (*m_lineIterator) = str1; // Replace the current line
        // list.insert works behind the iterator, so we need to go forward, insert, then go back
        ++m_lineIterator;
        m_linesList.insert(m_lineIterator, str2);
        --m_lineIterator;
        m_line++;
        m_col = 0;
    }
}

void StudentTextEditor::enter()
{
    // Handle cursor at end of line case
    getUndo()->submit(Undo::SPLIT, m_line, m_col, '\0');
    raw_enter();
}

void StudentTextEditor::getPos(int& row, int& col) const
{
    row = m_line;
    col = m_col;
    return;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const
{
    if (startRow < 0 || numRows < 0) return -1;

    lines.clear();

    auto temp_lineIterator = m_lineIterator;
    advance(temp_lineIterator, startRow - m_line);

    int j = 0;
    for (; j < numRows; j++, ++temp_lineIterator)
    {
        if (temp_lineIterator == m_linesList.end()) break;
        lines.push_back(*temp_lineIterator);
    }

    return j;
}

void StudentTextEditor::undo()
{
    int    row   = -1;
    int    col   = -1;
    int    count = -1;
    string text;

    Undo::Action action = getUndo()->get(row, col, count, text);

    if (action != Undo::ERROR)
    {
        // If row is the larger number, move_down is true, and then we will increase m_line so the while loop will equalize eventually
        const bool move_down = row - m_line > 0 ? true : false;
        while (m_line - row != 0)
        {
            if (move_down)
            {
                ++m_lineIterator;
                m_line++;
            }
            else
            {
                --m_lineIterator;
                m_line--;
            }
        }
        m_col = col;
    }

    switch (action)
    {
    case Undo::ERROR:
        // Something has gone wrong, just return
        return;
    case Undo::INSERT:
        // Need to insert from string to document at 
        for (char ch : text) raw_insert(ch);
        m_col = col;
        break;
    case Undo::SPLIT:
        raw_enter();
        m_col = col;
        m_line--;
        --m_lineIterator;
        break;
    case Undo::DELETE:
        for (int i = 0; i < count; i++) raw_backspace_middle();
        break;
    case Undo::JOIN:
        raw_del_end_of_line();
        break;
    default: ;
    }
    // TODO
}

/// <summary>
/// Resets the column position if needed to stick to the right edge on movement down.
/// </summary>
/// <returns>True when it moves the column position, false otherwise</returns>
bool StudentTextEditor::reEvaluateColPos()
{
    if (m_lineIterator->size() < m_col)
    {
        m_col = m_lineIterator->size();
        return true;
    }
    return false;
}

// TODO: Speed up checking by checking the end of line only
std::string StudentTextEditor::cleanString(std::string str)
{
    if (str.empty()) return "";
    if (str[str.length() - 1] == '\r') str.erase(str.end() - 1, str.end());
    if (str[str.length() - 1] == '\n') str.erase(str.end() - 1, str.end());
    return str;
}
