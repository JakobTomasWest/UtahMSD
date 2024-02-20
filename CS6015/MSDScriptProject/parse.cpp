#include <iostream>
#include "parse.hpp"

static void consume(std::istream  &in, int expect);
void skip_whitespace(std::istream &in);
//Expr *parse_num(std::istream &inn);
//Expr *parse_addend(std::istream &inn);
//Expr *parse_multicand (std::istream & inn);


Expr *parse_expr(std::istream &in) {
    Expr *e;
    e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr *rhs = parse_expr(in);
        return new AddExpr(e, rhs);
    } else
        return e;
}

Expr *parse_addend (std::istream &in) {
    Expr *e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        Expr *rhs = parse_expr(in);
        return new Mult(e, rhs);
    } else
        return e;
}
Expr *parse_multicand(std::istream &in) {
    skip_whitespace(in);
    int c = in.peek();
    if ((c == '-') || isdigit(c))
        return parse_num(in);
    else if (c == '(') {
        consume(in, '(');
        Expr *e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')')
            throw std::runtime_error("missing close parenthesis");
        return e;
    } else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}
Expr *parse_num(std::istream &inn) {
    int n = 0;
    bool negative = false;

    if (inn.peek() == '-') {
        negative = true;
        consume (inn, '-');

    }

    while (1) {
        int c = inn.peek();
        if (isdigit(c))
        {
            consume(inn, c);
            n = n*10 + (c - '0');
        }
        else
            break;
    }

    if (negative)
        n = -n;

    return new NumExpr(n);
}


static void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c!=expect) {
        throw std::runtime_error("consume mismatch");
    }
}

void skip_whitespace(std::istream &in) {
  while (1) {
    int c = in.peek();
    if (!isspace(c))
      break;
    consume(in, c);
      }
}

Expr* parse(std::istream &in) {
    return parse_expr(in);
}

// Implement parseString
Expr* parseString(const std::string &s) {
    std::istringstream in(s);
    return parse_expr(in);
}



//
//isAlpha() -->
//_--> read 3 characters --> should be followed by varName --> = --> (spaces --> )expr ---> (spaces -->) _in --> expr
//
//parse _var helper
// parse_let jelper
//parse_keyword