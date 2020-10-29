#include <fstream>
#include <iostream>

#include "parser.h"
#include "logic_engine.h"

char truth_value_to_char(bool truth_value);

void print_header(ElementaryProposition *props, LogicNode *parse_tree);
void print_row(ElementaryProposition *props, LogicNode *parse_tree);

int main(int num_args, char **args) {
	std::ifstream input_stream;
	input_stream.open(args[1]);

	char *logic_string = new char[256];
	
	while(input_stream.getline(logic_string, 256)) {
		std::cout << "Truth table for: " << logic_string << "\n";
		LogicNode *parse_tree = parse_string(logic_string);
	
		ElementaryProposition *props = get_propositions(parse_tree);
		ElementaryProposition **lattice = props->lattice();
	
		print_header(props, parse_tree);
	
		while (*lattice != nullptr) {
			print_row(*lattice, parse_tree);
			lattice++;
		}

		delete props;
		delete parse_tree;
	}

	input_stream.close();
}

char truth_value_to_char(bool truth_value) {
	if (truth_value) {
		return 'T';
	}
	return 'F';
}

void print_header(ElementaryProposition *props, LogicNode *parse_tree) {
	while (props != nullptr) {
		std::cout << props->symbol << " | ";
		props = props->next;
	}
	std::cout << "Eval\n";
}

void print_row(ElementaryProposition *props, LogicNode *parse_tree) {
	ElementaryProposition *props_cursor = props;
	while (props_cursor != nullptr) {
		std::cout << truth_value_to_char(props_cursor->truth_value) << " | ";
		props_cursor = props_cursor->next;
	}
	std::cout << parse_tree->evaluate(props) << "\n";
}
