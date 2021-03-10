#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include <list>

#include "TextEditor.h"

class Undo;


class StudentTextEditor : public TextEditor
{
public:

    StudentTextEditor(Undo* undo);
    virtual ~StudentTextEditor();
    bool    load(std::string file) override;
    bool    save(std::string file) override;
    void    reset() override;
    void    move(Dir dir) override;
    void    del() override;
    void    backspace() override;
    void    insert(char ch) override;
    void    enter() override;
    void    getPos(int& row, int& col) const override;
    int     getLines(int startRow, int numRows, std::vector<std::string>& lines) const override;
    void    undo() override;

private:
    bool reEvaluateColPos();
    bool isAtLineEnd() const;

    void move_up();
    void move_down();
    void move_left();
    void move_right();
    void move_home();
    int  move_end();

    void raw_del_end_of_line();
    char raw_del_middle();
    void raw_backspace_start_of_line();
    char raw_backspace_middle();
    void raw_insert(char ch);
    void raw_enter();

    static std::string cleanString(std::string str);

    std::list<std::string>           m_linesList;
    std::list<std::string>::iterator m_lineIterator;

    int m_line;
    int m_col;
};

#endif // STUDENTTEXTEDITOR_H_
