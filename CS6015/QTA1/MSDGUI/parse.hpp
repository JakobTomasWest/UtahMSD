
#ifndef parse_hpp
#include "expr.hpp"
#include <istream>
#include <string>
#include <sstream>
#define parse_hpp
#include "pointer.hpp"
#include <stdio.h>
PTR(Expr) parse_expr(std::istream &in);
PTR(Expr) parse_num(std::istream &in);
PTR(Expr) parse_addend(std::istream &inn);
PTR(Expr) parse_multicand (std::istream & inn);
//static void consume(std::istream  &in, int expect);
void skip_whitespace(std::istream &in);
PTR(Expr) parse_var(std::istream &inn);
PTR(Expr) parse_let(std::istream &in);
PTR(Expr) parse_comparg(std::istream &in);
PTR(Expr) parse(std::istream &in);
PTR(Expr) parseInput();
PTR(Expr) parseString(const std::string &s);
PTR(Expr) parse_if(std::istream &in);
PTR(Expr) parse_inner(std::istream &in);
PTR(Expr) parse_fun(std::istream &in);

#endif
