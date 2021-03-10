#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"
#include <stack>
#include <list>


class StudentUndo : public Undo
{
public:

    void   submit(Action action, int row, int col, char ch = 0) override;
    Action get(int& row, int& col, int& count, std::string& text) override;
    void   clear() override;

private:
    struct CursorPos;

    static Undo::Action invertAction(Undo::Action action);
    void                emplaceOrBatch(Undo::Action action, CursorPos pos, char ch);


    struct CursorPos
    {
        int row = 0;
        int col = 0;
    };


    /// <summary>
    /// A single undo object, containing all the information needed to restore that operation.
    /// * action is the action taken, one of Undo::Action. This is what was done by the user, i.e. enter gives SPLIT, delete gives del DELETE or JOIN. Notably, not what will be returned at undotime.
    /// * cursor_pos is the position of the cursor, usually after the operation, but before the operation for enter
    /// * data is the data payload for the relevant command. This varies significantly.
    ///
    /// Other notes:
    ///     * Batching is handled at submit time by comparing to the top of the stack.
    /// </summary>
    struct UndoObject
    {
        UndoObject(Undo::Action action, CursorPos cursor_pos, char data)
            : cursor_pos(cursor_pos), action(action), data(1, data) {}

        int count() const { return data.size(); }
        std::string dataAsStr();

        CursorPos       cursor_pos = {0, 0};
        Undo::Action    action;
        std::list<char> data;
    };


    std::stack<UndoObject> undoStack;
};

#endif // STUDENTUNDO_H_
