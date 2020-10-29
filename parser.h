#ifndef PARSER_H
#define PARSER_H

#include "logic_engine.h"

LogicNode *parse_string(char *input_string);
ElementaryProposition *get_propositions(LogicNode *logic_tree);

#endif
