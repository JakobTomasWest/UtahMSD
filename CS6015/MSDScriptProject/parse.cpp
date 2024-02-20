#include <iostream>
#include "parse.hpp"

static void consume(std::istream  &in, int expect);
void skip_whitespace(std::istream &in);
//Expr *parse_num(std::istream &inn);
//Expr *parse_addend(std::istream &inn);
//Expr *parse_multicand (std::istream & inn);


Expr *parse_expr(std::istream &in) {
    //if you find the beginning of let expression send it
    skip_whitespace(in);
    if(in.peek() == '_'){
        return parse_let(in);
    }
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
        Expr *rhs = parse_multicand(in);
        return new Mult(e, rhs);
    } else
        return e;
}
Expr *parse_multicand(std::istream &in) {
    skip_whitespace(in);
    int c = in.peek();
    //slide 75 - look for (negative)number first
    if ((c == '-') || isdigit(c)){
        return parse_num(in);
    }else if(isalpha(c)){
        return parse_var(in);

        //if we find parenthesis we'll consume opener --> parse expression inside --> consume closing
    }else if (c == '_') {
        return parse_let(in);
    }else if (c == '(') {
            consume(in, '(');
            if(in.peek() == ')'){
                throw std::runtime_error("bad input");
            }
            Expr *e = parse_expr(in);
            skip_whitespace(in);
            c = in.get();
            if (c != ')')
                throw std::runtime_error("missing closing parenthesis");
            return e;

    } else {
        consume(in, c);
        throw std::runtime_error("invalid input in multicand");
    }
}
Expr *parse_num(std::istream &inn) {
    int n = 0;
    bool negative = false;
    if (inn.peek() == '-') {
        negative = true;
        consume (inn, '-');
        if(!isdigit(inn.peek())){
            throw std::runtime_error("no following digit after the negative sign, invalid input");
        }
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

Expr* parse(std::istream &in) {
    return parse_expr(in);
}

Expr* parseString(const std::string &s) {
    std::istringstream in(s);
    return parse(in);
}

Expr *parse_var(std::istream &inn) {
    std::string varName;
    skip_whitespace(inn);
    //while we have alphabetic entries take the stream in and concatenate each char to the string
    if(std::isalpha(inn.peek())){
        varName +=static_cast<char>(inn.get());
    } else {
        throw std::runtime_error("Expected a variable name to start with a letter");
    }
    //variables can have numbers or other strings after the starting alphabet char
    while (std::isalpha(inn.peek()) || inn.peek() == '_') {
        if (inn.peek() == '_') {
            throw std::runtime_error("invalid input: variable names cannot contain underscores");
        }
        varName += static_cast<char>(inn.get());
    }
    if (varName.empty()){
        throw std::runtime_error("Expected a variable name");
    }
    return new VarExpr(varName);
}


Expr* parse_let(std::istream &in){
    //first *consume* _let
    string letPart;
    for(int i =0; i <4; i++){
        letPart += static_cast<char>(in.get());
    }

    if(letPart != "_let"){
        throw std::runtime_error("expected '_let'");
    }
    skip_whitespace(in);
    //parse variable name -- not as a new var expr
    string varName;
    while (isalpha(in.peek())){
        varName += static_cast<char>(in.get());
    }
    if (varName.empty()) {
        throw std::runtime_error("Expected a variable name after '_let'");
    }
    skip_whitespace(in);
    consume(in, '=');
    skip_whitespace(in);

    //parse boundExpr
    Expr* boundExpr = parse_expr(in);
    skip_whitespace(in);

    //*consume* _in
    string inPart;
    for(int i =0; i < 3; i++){
        inPart += static_cast<char>(in.get());
    }
    if(inPart != "_in"){
        throw std::runtime_error("Expected '_in'");
    }
    skip_whitespace(in);
    //parse bodyExpr
    Expr* bodyExpr = parse_expr(in);

    return new LetExpr(varName, boundExpr, bodyExpr);
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
