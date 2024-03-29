#include "StudentUndo.h"

#include <iostream>
#include <fstream>

using namespace std;

Undo* createUndo() { return new StudentUndo; }

/// <summary>
/// Accepts a request to save something to the undo stack, adding it or batching it as needed. 
/// </summary>
void StudentUndo::submit(const Action action, int row, int col, char ch)
{
    CursorPos pos = {row, col};
    switch (action)
    {
    case ERROR:
        cerr << "Received an error undo submission!" << endl;
        break;
    case INSERT:
        emplaceOrBatch(Action::INSERT, pos, ch);
        break;
    case DELETE:
        emplaceOrBatch(Action::DELETE, pos, ch);
        break;
    case SPLIT:
        undoStack.emplace(Action::SPLIT, pos, '\0');
        break;
    case JOIN:
        undoStack.emplace(Action::JOIN, pos, '\0');
        break;
    }
}

/// <summary>
/// Gets the next undo on the stack. Handles converting the internal format to the requested format and providing the correct information for each type of request.
/// </summary>
StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text)
{
    if (undoStack.empty()) return ERROR;
    UndoObject top = undoStack.top();
    undoStack.pop();
    // Default items
    row = top.cursor_pos.row;
    col = top.cursor_pos.col;
    count = 1;
    text = "";
    switch (top.action)
    {
    case ERROR:
        return ERROR;
    case INSERT:
        // An insert was made, so we need to delete those chars
        // Starting location where inserted text should be removed from
        // Count should be num chars to delete
        // Text must be empty
        count = top.count();
        return DELETE;
    case SPLIT:
        // The item was split, so we need to join back up
        // The location where the enter key was pressed to do the split originally
        // Count should be one
        // Text must be empty
        return JOIN;
    case DELETE:
        // An item was deleted, so we need to insert back in
        // Pos should be starting location where deleted items should be restored to
        // Count should be one
        // Text should be the string to restore
        text = top.dataAsStr();
        return INSERT;
    case JOIN:
        // Lines were joined, so we need to split them
        // Pos should be where the cursor needs to be to split the line back to the way it was
        // Count should be one
        // Text must be empty
        return SPLIT;
    }
    return Action::ERROR; // TODO
}

/// <summary>
/// Clears the undoStack in O(N)
/// </summary>
void StudentUndo::clear() { while (!undoStack.empty()) undoStack.pop(); }

/// <summary>
/// Handles the conditional logic on sometimes batching insertions and deletions, always does the right thing in my testing. 
/// </summary>
void StudentUndo::emplaceOrBatch(Undo::Action action, CursorPos pos, char ch)
{
    // Avoid checking the top of an empty stack
    if (undoStack.empty())
    {
        undoStack.emplace(action, pos, ch);
        return;
    }
    UndoObject& top  = undoStack.top();
    bool        flag = false;
    switch (action)
    {
    case INSERT:
        // Sequential inserts without any other action
        if (top.action == INSERT && pos.row == top.cursor_pos.row && pos.col == top.cursor_pos.col + 1)
        {
            top.data.push_back(ch);
            top.cursor_pos.col = pos.col;
        }
            // Normal emplace
        else undoStack.emplace(action, pos, ch);
        break;
    case DELETE:
        // Two cases, sequential backspace and sequential delete
        if (top.action == DELETE)
        {
            // Same exact pos, this is an additional del keypress
            if (pos.col == top.cursor_pos.col && pos.row == top.cursor_pos.row)
            {
                top.data.push_back(ch);
                flag = true;
            }
            // A few positions to the left, so this was an additional backspace keypress
            if (pos.col == top.cursor_pos.col - 1 && pos.row == top.cursor_pos.row)
            {
                top.data.push_front(ch);
                // Ensures the cursor position is always at the start of the deletions
                top.cursor_pos.col--;
                flag = true;
            }
        }
        // Normal emplace
        if (!flag) undoStack.emplace(action, pos, ch);
        break;
    case SPLIT:
    case ERROR:
    case JOIN:
        break;
    }
}

/// <summary>
/// Takes the list of chars in an undoObject and returns it as a string
/// </summary>
/// <returns>String form of the given data</returns>
std::string StudentUndo::UndoObject::dataAsStr()
{
    std::string str;
    for (char ch : data) str += ch;
    return str;
}
