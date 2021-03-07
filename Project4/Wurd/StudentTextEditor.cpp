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
        if (prev_line != m_line) { m_col = (*m_lineIterator).size(); }
    }
}

void StudentTextEditor::move_right()
{
    if (m_col < (*m_lineIterator).size()) ++m_col;
    else
    {
        const int prev_line = m_line;
        move_down();
        // If the line changed, then we know that we need to reset the column to zero because we went off the right edge actually to somewhere.
        if (prev_line != m_line) { m_col = 0; }
    }
}

void StudentTextEditor::move_home() { m_col = 0; }

int         StudentTextEditor::move_end() { return m_col = (*m_lineIterator).size(); }

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
    // TODO
}
#pragma endregion


void StudentTextEditor::del()
{
    // TODO
}

void StudentTextEditor::backspace()
{
    // TODO
}

void StudentTextEditor::insert(char ch)
{
    if (ch == '\t')
    {
        line().insert(m_col, 4, ' ');
        m_col += 4;
        return;
    }
    line().insert(m_col, 1, ch);
    m_col++;
    // TODO
}

void StudentTextEditor::enter()
{
    string str1 = line().substr(0, m_col);
    string str2 = line().substr(m_col, line().size());
    // TODO
}

void StudentTextEditor::getPos(int& row, int& col) const
{
    row = m_line;
    col = m_col;
    return;
    // TODO
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

    // TODO
    return j;
}

void StudentTextEditor::undo()
{
    // TODO
}

/// <summary>
/// Resets the column position if needed to stick to the right edge on movement down.
/// </summary>
/// <returns>True when it moves the column position, false otherwise</returns>
bool StudentTextEditor::reEvaluateColPos()
{
    if (line().size() < m_col)
    {
        m_col = line().size();
        return true;
    }
    return false;
}

// TODO: Speed up checking by checking the end of line only
std::string StudentTextEditor::cleanString(std::string str)
{
    if (str[str.length() - 1] == '\r') str.erase(str.end() - 1, str.end());
    if (str[str.length() - 1] == '\n') str.erase(str.end() - 1, str.end());
    return str;
}

std::string StudentTextEditor::line() const { return (*m_lineIterator); }
