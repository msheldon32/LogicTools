#include <cstring>
#include <iostream>

#include "parser.h"
#include "logic_engine.h"

// constants
char const* WHITESPACE_CHARACTERS = " \n\t\r";

void RemoveChar(char* input_string, int loc) {
	/*
	 *  Removes a character from a string at a given location.
	 */
	for (int i = loc; i < (int) std::strlen(input_string); i++) {
		input_string[i] = input_string[i+1];
	}
}

bool HasSuperfluousParentheses(char* input_string) {
	/*
	 *  Check if a given string is surrounded by at least one pair of
	 *    superfluous parentheses. Assumes that any whitespace or other
	 *    irrelevant characters have already been removed.
	 */
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

void StripInput(char* input_string) {
	/*
	 *  Remove any whitespace or superfluous parenthesis from the given input string.
	 */
	int string_length = std::strlen(input_string);
	for (int i = 0; i < string_length; i++) {
		if (std::strchr(WHITESPACE_CHARACTERS, input_string[i]) != nullptr) {
			RemoveChar(input_string, i);
			i--;
			string_length--;
		}
	}

	while (HasSuperfluousParentheses(input_string)) {
		/*
		 *  Delete first and last characters (presumably parenthesis).
		 */
		RemoveChar(input_string, string_length-1);
		RemoveChar(input_string, 0);
		string_length-=2;
	}
}

void Regularize(char* input_string) {
	/*
	 *  Regularize the input string.
	 *
	 *  At the moment, this just calss strip_input.
	 */ 
	StripInput(input_string);
}

LogicNode* ParseString(char* input_string) {
	/*
	 *  Transforms input_string into a tree of LogicNodes
	 */
	int input_str_len = std::strlen(input_string);

	Regularize(input_string);

	LogicNode* out_node;

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
			out_node->SetLeft(ParseString(input_string));
			return out_node;
		}
	}

	char* left_string  = new char[pivot_location+1];
	char* right_string = new char[(input_str_len-pivot_location)];
	std::strncpy(left_string, input_string, pivot_location);
	std::strncpy(right_string, input_string+pivot_location+1, input_str_len-pivot_location-1);

	// null terminate
	left_string[pivot_location] = '\0';
	right_string[(input_str_len-pivot_location-1)] = '\0';

	out_node = new LogicNode(input_string[pivot_location]);
	out_node->SetLeft(ParseString(left_string));
	out_node->SetRight(ParseString(right_string));

	return out_node;
}

ElementaryProposition* GetPropositions(LogicNode* logic_tree) {
	/*
	 *  Extracts any symbols representing a proposition,
	 *    and creates ElementaryProposition objects from these.
	 */
	if (std::strchr("+*~", logic_tree->GetSymbol()) != nullptr) {
		ElementaryProposition* left_list  = GetPropositions(logic_tree->GetLeft());
		if (logic_tree->GetRight() != nullptr) {
			ElementaryProposition *right_left = GetPropositions(logic_tree->GetRight());
			left_list->MergeList(right_left);
		}
		return left_list;
	}

	return new ElementaryProposition(logic_tree->GetSymbol(), false);
}
