#include <cstring>
#include <iostream>

#include "parser.h"
#include "logic_engine.h"

// constants
char const *WHITESPACE_CHARACTERS = " \n\t\r";

void remove_char(char *input_string, int loc) {
	for (int i = loc; i < (int) std::strlen(input_string); i++) {
		input_string[i] = input_string[i+1];
	}
}

bool has_superfluous_parentheses(char *input_string) {
	int string_length = std::strlen(input_string);
	if (input_string[0] != '(') {
		return false;
	} else if (input_string[string_length-1] != ')') {
		return false;
	}

	int depth = 0;
	for (int str_iter = 0; str_iter < string_length; str_iter++) {
		if (input_string[str_iter] == '(') {
			depth++;
		} else if (input_string[str_iter] == ')') {
			depth--;
		} else if (depth == 0) {
			return false;
		}
	}

	return true;
}

void strip_input(char *input_string) {
	// remove any whitespace...
	int string_length = std::strlen(input_string);
	for (int i = 0; i < string_length; i++) {
		if (std::strchr(WHITESPACE_CHARACTERS, input_string[i]) != nullptr) {
			remove_char(input_string, i);
			i--;
			string_length--;
		}
	}

	// ... and superfluous parentheses
	while (has_superfluous_parentheses(input_string)) {
		remove_char(input_string, string_length-1);
		remove_char(input_string, 0);
		string_length-=2;
	}
}

void regularize(char *input_string) {
	strip_input(input_string);
}

LogicNode *parse_string(char *input_string) {
	int input_str_len = std::strlen(input_string);

	regularize(input_string);

	LogicNode *out_node;

	// check if we have an elementary proposition
	if (input_str_len == 1) {
		// assume we have an elementary proposition if there's one element
		// the constructor will fail otherwise, as it should, if the character is invalid
		out_node = new LogicNode(input_string[0]);
		return out_node;
	}

	// find a pivot point, and parse each side.
	int parentheses_depth = 0;
	int pivot_location = -1;
	char current_char;

	for (int i = 0; i <= input_str_len; i++) {
		current_char = input_string[i];
		if (current_char == '(') {
			parentheses_depth++;
		} else if (current_char == ')') {
			parentheses_depth--;
		} else if (parentheses_depth == 0) {
			if (current_char == '+') {
				// lowest priority - split here for sure.
				pivot_location = i;
				break;
			} else if (current_char == '*') {
				if (pivot_location == -1) {
					pivot_location = i;
				}
			}
		}
	}

	// check for a negative
	if (pivot_location == -1) {
		if (input_string[0] == '~') {
			out_node = new LogicNode('~');
			input_string++;
			out_node->set_left(parse_string(input_string));
			return out_node;
		}
	}

	char *left_string  = new char[pivot_location+1];
	char *right_string = new char[(input_str_len-pivot_location)];
	std::strncpy(left_string, input_string, pivot_location);
	std::strncpy(right_string, input_string+pivot_location+1, input_str_len-pivot_location-1);
	// null terminate
	left_string[pivot_location] = '\0';
	right_string[(input_str_len-pivot_location-1)] = '\0';

	out_node = new LogicNode(input_string[pivot_location]);
	out_node->set_left(parse_string(left_string));
	out_node->set_right(parse_string(right_string));

	return out_node;
}

ElementaryProposition *get_propositions(LogicNode *logic_tree) {
	if (std::strchr("+*~", logic_tree->get_symbol()) != nullptr) {
		ElementaryProposition *left_list  = get_propositions(logic_tree->get_left());
		if (logic_tree->get_right() != nullptr) {
			ElementaryProposition *right_left = get_propositions(logic_tree->get_right());
			left_list->merge_list(right_left);
		}
		return left_list;
	}

	return new ElementaryProposition(logic_tree->get_symbol(), false);
}
