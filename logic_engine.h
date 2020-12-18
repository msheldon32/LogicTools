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

	ElementaryProposition(char init_symbol, bool init_truth_value, 
			      ElementaryProposition* init_next=nullptr);
	ElementaryProposition(const ElementaryProposition& other);
	~ElementaryProposition();

	ElementaryProposition& operator=(const ElementaryProposition& other);

	bool GetValueFromSymbol(char search_symbol);
	bool FindSymbol(char search_symbol);
	ElementaryProposition** Lattice();

	void MergeList(ElementaryProposition* new_list);

	bool operator==(ElementaryProposition comparison);
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
	LogicNode(const LogicNode& other);
	~LogicNode();

	LogicNode& operator= (const LogicNode& other);

	void SetLeft(LogicNode* init_left);
	void SetRight(LogicNode* init_right);

	LogicNode* GetLeft();
	LogicNode* GetRight();

	char GetSymbol();

	char* GetString();

	bool Evaluate(ElementaryProposition* first_prop);

	LogicNode* DemorganDual();
	LogicNode* Distribute(char dist_symbol, LogicNode* dist_left);
};

#endif
