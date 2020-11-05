#ifndef LOGIC_ENGINE_H
#define LOGIC_ENGINE_H

class ElementaryProposition {
	/*
	 *   ElementaryProposition -
	 *   	Linked-list representing individual propositional elements
	 *
	 *   Representation [Symbol]->[Truth value]
	 */
public:
	ElementaryProposition* next;
	bool truth_value;
	char symbol;

	ElementaryProposition(char init_symbol, bool init_truth_value, ElementaryProposition* init_next=nullptr);
	~ElementaryProposition();

	bool GetValueFromSymbol(char search_symbol);
	bool FindSymbol(char search_symbol);
	ElementaryProposition** Lattice();
	ElementaryProposition* Copy();

	void MergeList(ElementaryProposition* new_list);
};

class LogicNode {
	/*
	 *   LogicNode -
	 *   	Represents a node of the parse tree. Should be a wff.
	 */
private:
	char symbol;
	LogicNode* left;
	LogicNode* right;
public:
	LogicNode(char init_symbol);
	~LogicNode();

	void SetLeft(LogicNode* init_left);
	void SetRight(LogicNode* init_right);

	LogicNode* GetLeft();
	LogicNode* GetRight();

	char GetSymbol();

	bool Evaluate(ElementaryProposition* first_prop);
};

#endif
