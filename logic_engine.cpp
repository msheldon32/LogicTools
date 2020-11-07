/*
 * logic_engine.cpp -
 *     Engine for parsing and evaluating propositional statements
 */

#include <cstring>
#include <iostream>
#include <stdexcept>

#include "logic_engine.h"


ElementaryProposition::ElementaryProposition(char init_symbol, bool init_truth_value, ElementaryProposition* init_next) {
	if (std::strchr("+*~01", init_symbol) != nullptr) {
		throw std::invalid_argument("Reserved Symbol Used in Proposition");
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
	if (symbol == '0') {
		return false;
	} else if (symbol == '1') {
		return true;
	}


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

LogicNode* LogicNode::DemorganDual() {
	/*
	 *   Finds the demorgan dual of the current node, if one exists
	 */
	if (symbol == '~') {
		if (left->symbol == '+') {
			LogicNode* return_node = new LogicNode('*');
			LogicNode* return_node_left  = new LogicNode('~');
			LogicNode* return_node_right = new LogicNode('~');
			return_node_left->SetLeft(left->GetLeft());
			return_node_right->SetLeft(left->GetRight());
			return_node->SetLeft(return_node_left);
			return_node->SetRight(return_node_right);
			return return_node;
		} else if (left->symbol == '*') {
			LogicNode* return_node = new LogicNode('+');
			LogicNode* return_node_left  = new LogicNode('~');
			LogicNode* return_node_right = new LogicNode('~');
			return_node_left->SetLeft(left->GetLeft());
			return_node_right->SetLeft(left->GetRight());
			return_node->SetLeft(return_node_left);
			return_node->SetRight(return_node_right);
			return return_node;
		} else {
			return this;
		}
	} else if (symbol == '+') {
		if ((left->symbol == '~') && (right->symbol == '~')) {
			LogicNode* return_node = new LogicNode('~');
			LogicNode* return_node_neg = new LogicNode('*');
			return_node_neg->SetLeft(left->GetLeft());
			return_node_neg->SetRight(right->GetLeft());
			return_node->SetLeft(return_node_neg);
			return return_node;
		} else {
			return this;
		}
	} else if (symbol == '*') {
		if ((left->symbol == '~') && (right->symbol == '~')) {
			LogicNode* return_node = new LogicNode('~');
			LogicNode* return_node_neg = new LogicNode('+');
			return_node_neg->SetLeft(left->GetLeft());
			return_node_neg->SetRight(right->GetLeft());
			return_node->SetLeft(return_node_neg);
			return return_node;
		} else {
			return this;
		}
	} else {
		return this;
	}
}

LogicNode* LogicNode::Distribute(char dist_symbol, LogicNode* dist_left) {
	if (right != nullptr) {
		LogicNode* return_node = new LogicNode(symbol);
		LogicNode* return_left = new LogicNode(dist_symbol);
		LogicNode* return_right = new LogicNode(dist_symbol);
		return_left->SetLeft(dist_left);
		return_left->SetRight(left);
		return_right->SetLeft(dist_left);
		return_right->SetRight(right);
		return_node->SetLeft(return_left);
		return_node->SetRight(return_right);
		return return_node;
	} else {
		LogicNode* return_node = new LogicNode(dist_symbol);
		return_node->SetLeft(dist_left);
		return_node->SetRight(this);
		return return_node;
	}
}

char* LogicNode::GetString() {
	char* left_string  = new char[1];
	left_string[0] = '\0';
	char* right_string = new char[1];
	right_string[0] = '\0';

	if (left != nullptr) {
		left_string = left->GetString();
	}
	if (right != nullptr) {
		right_string = right->GetString();
	}

	char* output_string = new char[std::strlen(left_string)+std::strlen(right_string)+2];
	output_string[std::strlen(left_string)+std::strlen(right_string)+1] = '\0';
	output_string[std::strlen(left_string)] = symbol;
	std::strcpy(output_string, left_string);
	std::strcpy(output_string+std::strlen(left_string)+1, right_string);
	
	return output_string;
}
