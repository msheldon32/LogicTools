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

	ElementaryProposition(char init_symbol, bool init_truth_value, ElementaryProposition *init_next=nullptr);
	~ElementaryProposition();

	bool get_value_from_symbol(char search_symbol);
	bool find_symbol_in_list(char search_symbol);
	ElementaryProposition **lattice();
	ElementaryProposition *copy();

	void merge_list(ElementaryProposition *new_list);
};

class LogicNode {
	/*
	 *   LogicNode -
	 *   	Represents a node of the parse tree. Should be a wff.
	 */
private:
	char symbol;
	LogicNode *left, *right;
public:
	LogicNode(char init_symbol);
	~LogicNode();

	void set_left(LogicNode *init_left);
	void set_right(LogicNode *init_right);

	bool evaluate(ElementaryProposition *first_prop);

	char get_symbol();

	LogicNode *get_left();
	LogicNode *get_right();
};

#endif
