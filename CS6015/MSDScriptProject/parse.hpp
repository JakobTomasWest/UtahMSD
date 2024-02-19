
#ifndef parse_hpp
#include "expr.hpp"
#define parse_hpp

#include <stdio.h>

Expr *parse_num(std::istream &in);
Expr *parse_addend(std::istream &inn);
Expr *parse_multicand (std::istream & inn);
//static void consume(std::istream  &in, int expect);
void skip_whitespace(std::istream &in);
#endif
