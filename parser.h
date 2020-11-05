#ifndef PARSER_H
#define PARSER_H

#include "logic_engine.h"

LogicNode* ParseString(char* input_string);
ElementaryProposition* GetPropositions(LogicNode* logic_tree);

#endif
