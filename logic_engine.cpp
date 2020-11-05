/*
 * logic_engine.cpp -
 *     Engine for parsing and evaluating propositional statements
 */

#include <cstring>
#include <iostream>

#include "logic_engine.h"


ElementaryProposition::ElementaryProposition(char init_symbol, bool init_truth_value, ElementaryProposition* init_next) {
	if (std::strchr("+*~", init_symbol) != nullptr) {
		throw "Reserved Character as Proposition!";
	}

	symbol = init_symbol;
	truth_value = init_truth_value;
	next = init_next;
}

ElementaryProposition::~ElementaryProposition() {
	delete next;
}

bool ElementaryProposition::GetValueFromSymbol(char search_symbol) {
	/*
	 *   Searches through the linked list to find the truth value
	 *   	corresponding to the given symbol.
	 */
	if (symbol == search_symbol) {
		return truth_value;
	}

	return next->GetValueFromSymbol(search_symbol);
}

bool ElementaryProposition::FindSymbol(char search_symbol) {
	/*
	 *   Searches through the linked list to see if the given symbol
	 *   	corresponds to a proposition.
	 */
	if (symbol == search_symbol) {
		return true;
	} else if (next == nullptr) {
		return false;
	}

	return next->FindSymbol(search_symbol);
}

ElementaryProposition* ElementaryProposition::Copy() {
	/*
	 *   Recursively copies every elementary proposition.
	 */
	if (next == nullptr) {
		return new ElementaryProposition(symbol, truth_value);
	}
	return new ElementaryProposition(symbol, truth_value, next->Copy());
}

ElementaryProposition** ElementaryProposition::Lattice() {
	/*
	 *   Returns an array of ElementaryProposition objects, 
	 * 	containing every possible truth value for every proposition.
	 */
	if (next == nullptr) {
		ElementaryProposition** out_array = new ElementaryProposition *[2];
		out_array[0] = new ElementaryProposition(symbol, false);
		out_array[1] = new ElementaryProposition(symbol, true);
		out_array[2] = nullptr;
		return out_array;
	}

	// get the lattice of the following element
	ElementaryProposition** next_lattice = next->Lattice();

	// find it's depth
	int cur_lattice_depth = 0;
	

	for (ElementaryProposition** nl_iter = next_lattice; (*nl_iter) != nullptr; nl_iter++) {
		cur_lattice_depth++;
	}

	// duplicate this lattice, one copy with the current proposition as false, and the other 
	//    with it as true
	ElementaryProposition** out_array = new ElementaryProposition* [(cur_lattice_depth*2)+1];
	out_array[(cur_lattice_depth*2)] = nullptr;
	int midpoint = cur_lattice_depth;
	for (int i = 0; i < cur_lattice_depth; i++) {
		out_array[i] = new ElementaryProposition(symbol, false, next_lattice[i]->Copy());
		out_array[midpoint+i] = new ElementaryProposition(symbol, true, next_lattice[i]->Copy());
	}

	return out_array;
}

void ElementaryProposition::MergeList(ElementaryProposition* new_list) {
	/*
	 *   Merge two ElementaryProposition lists. 
	 *   NOTE: ignores truth values, merges on symbols only.
	 */

	// ensure the first element doesn't correspond to itself...
	while (new_list->symbol == symbol) {
		new_list = new_list->next;
		if (new_list == nullptr) {
			return;
		}
	}

	// ...as well as any subsequent elements
	ElementaryProposition* cursor = new_list;

	while (cursor->next != nullptr) {
		if (cursor->next->symbol == symbol) {
			cursor->next = cursor->next->next;
		} else {
			cursor = cursor->next;
		}
	}

	// recursively merge
	if (next == nullptr) {
		next = new_list;
	} else {
		next->MergeList(new_list);
	}
}

LogicNode::LogicNode(char init_symbol) {
	left = nullptr;
	right = nullptr;
	symbol = init_symbol;
}

LogicNode::~LogicNode() {
	if (left != nullptr) {
		delete left;
	}
	if (right != nullptr) {
	       	delete right;
	}
}

void LogicNode::SetLeft(LogicNode* init_left) {
	left = init_left;
}

void LogicNode::SetRight(LogicNode* init_right) {
	right = init_right;
}

LogicNode* LogicNode::GetLeft() {
	return left;
}

LogicNode* LogicNode::GetRight() {
	return right;
}

char LogicNode::GetSymbol() {
	return symbol;
}

bool LogicNode::Evaluate(ElementaryProposition* first_prop) {
	/*
	 *   Recursively evaluate each node in the tree, accoridng to the truth values
	 *   	supplied by first_prop, and it's successors.
	 */

	if (first_prop->FindSymbol(symbol)) {
		// symbol represents an elementary proposition
		return first_prop->FindSymbol(symbol);
	}
	bool bleft  = left->Evaluate(first_prop);

	bool bright = false;
	if (right != nullptr) {
		bright = right->Evaluate(first_prop);
	}


	if (symbol == '+') {
		// or
		return bleft || bright;
	} else if (symbol == '*') {
		// and
		return bleft && bright;
	} else if (symbol == '~') {
		// not
		return !bleft;
	}

	// throw an exception. symbol is not recognized.
	throw "Unrecognized symbol";
}
