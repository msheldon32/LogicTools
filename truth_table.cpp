#include <fstream>
#include <iostream>

#include "parser.h"
#include "logic_engine.h"

char TruthValueToChar(bool truth_value);

void PrintHeader(ElementaryProposition* props, LogicNode* parse_tree);
void PrintRow(ElementaryProposition* props, LogicNode* parse_tree);

int main(int num_args, char** args) {
	std::ifstream input_stream;
	input_stream.open(args[1]);

	char *logic_string = new char[256];
	
	while(input_stream.getline(logic_string, 256)) {
		std::cout << "Truth table for: " << logic_string << "\n";
		LogicNode *parse_tree = ParseString(logic_string);
	
		ElementaryProposition*  props = GetPropositions(parse_tree);
		ElementaryProposition** lattice = props->Lattice();
	
		PrintHeader(props, parse_tree);
	
		while (*lattice != nullptr) {
			PrintRow(*lattice, parse_tree);
			lattice++;
		}

		delete props;
		delete parse_tree;
	}

	input_stream.close();
}

char TruthValueToChar(bool truth_value) {
	if (truth_value) {
		return 'T';
	}
	return 'F';
}

void PrintHeader(ElementaryProposition* props, LogicNode* parse_tree) {
	/*
	 *  Displays the header of a truth table corresponding to props and parse_tree.
	 *
	 *  NOTE: Right now, parse_tree is irrelevant to the output - this might change in the future.
	 */
	while (props != nullptr) {
		std::cout << props->symbol << " | ";
		props = props->next;
	}
	std::cout << "Eval\n";
}

void PrintRow(ElementaryProposition* props, LogicNode* parse_tree) {
	/*
	 *  Displays a row of the truth table formed by the given parse_tree and propositions
	 */
	ElementaryProposition *props_cursor = props;
	while (props_cursor != nullptr) {
		std::cout << TruthValueToChar(props_cursor->truth_value) << " | ";
		props_cursor = props_cursor->next;
	}
	std::cout << parse_tree->Evaluate(props) << "\n";
}
