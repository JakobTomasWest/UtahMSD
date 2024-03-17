
#ifndef parse_hpp
#include "expr.hpp"
#include <istream>
#include <string>
#include <sstream>
#define parse_hpp

#include <stdio.h>
Expr *parse_expr(std::istream &in);
Expr *parse_num(std::istream &in);
Expr *parse_addend(std::istream &inn);
Expr *parse_multicand (std::istream & inn);
//static void consume(std::istream  &in, int expect);
void skip_whitespace(std::istream &in);
Expr *parse_var(std::istream &inn);
Expr* parse_let(std::istream &in);
Expr* parse(std::istream &in);
Expr* parseString(const std::string &s);


#endif
