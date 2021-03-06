#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include <string>
#include <list>

class Undo;

class StudentTextEditor {
public:

	StudentTextEditor(Undo* undo);
	StudentTextEditor() {};
	~StudentTextEditor();
	bool load(std::string file);
	bool save(std::string file);


    static std::string cleanString(std::string string);
private:

    std::list<std::string> linesList;
};

#endif // STUDENTTEXTEDITOR_H_
