#include <iostream>
#include "parse.hpp"

static void consume(std::istream  &in, int expect);
void skip_whitespace(std::istream &in);
//Expr *parse_num(std::istream &inn);
//Expr *parse_addend(std::istream &inn);
//Expr *parse_multicand (std::istream & inn);

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

Expr* parse_var(std::istream &inn) {
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
            throw std::runtime_error("invalid input: variable names cannot contain underscores or alphanum");
        }
        varName += static_cast<char>(inn.get());
    }
    if (varName.empty()){
        throw std::runtime_error("Expected a variable name");
    }
    return new VarExpr(varName);
}
Expr* parse_fun(std::istream & in){
    skip_whitespace(in);
    consume(in,'(');
    Expr* formal_arg = parse_var(in);
    consume(in,')');
    Expr* body = parse_expr(in);
    return new FunExpr(formal_arg->to_string(),body);
}

std::string parse_keyword(std::istream &in){
    std::string keyword;
    skip_whitespace(in);
    int c = in.peek();
    //while the peek is a valid character append the characters to form the keyword
    while (c!=' ' and c!=-1){
        keyword +=(char)c;
        consume(in,c);
        c = in.peek();
    }
    return keyword;
}

Expr *parse_addend (std::istream &in) {
    Expr *e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    //if we see * we are gonna create a multiplication expression
    if (c == '*') {
        consume(in, '*');
        //parse rhs expression
        Expr *rhs = parse_addend(in);
        return new MultExpr(e, rhs);
    } else
        //otherwise return parsed multicand
        return e;
}

Expr* parse_inner(std::istream &in) {
    skip_whitespace(in);
    int c = in.peek();
    //if we have a negative parse as number
    if ((c == '-') || isdigit(c)) {
        return parse_num(in);
        //if we have alphabetic char parse as a variable
    } else if(isalpha(c)){
        return parse_var(in);
        //if we have _ indicator we will parse the keyword by appending each char
    } else if (c == '_'){
        std::string kw = parse_keyword(in);
        if (kw == "_let"){
            return parse_let(in);
        }else if (kw == "_true"){
            return new BooleanExpr(true);
        } else if(kw=="_false"){
            return new BooleanExpr(false);
        } else if (kw =="_if"){
            return parse_if(in);
        } else if (kw =="_fun"){
            return parse_fun(in);
        }
    } //otherwise if we have a parenthesis we have to run it all back with the new expression to parse denoted by (
    else if (c == '(') {
        consume(in, '(');
        Expr *e = parse_comparg(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')') {
            throw std::runtime_error("missing closing parenthesis");
        }
        return e;
    } else {
        consume(in, c);
        throw std::runtime_error("bad input");
    }
}
Expr* parse_multicand(std::istream &in) {
    Expr* e;
    Expr* actual_arg;
    e = parse_inner(in);
    while (in.peek() == '(') {
        consume(in, '(');
        actual_arg = parse_expr(in);
        consume(in, ')');
        e = new CallExpr(e, actual_arg);
    }
    return e;
}

Expr *parse_comparg(std::istream &in) {
    Expr *e;
    e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr *rhs = parse_comparg(in);
        return new AddExpr(e, rhs);
    }
    else{
        return  e;
    }
}

Expr* parse_expr(std::istream &in){
    Expr *e;
    e= parse_comparg(in);
    skip_whitespace(in);
    int c = in.peek();
    if(c=='='){
        //parse equality expression
        consume(in, '=');
        consume(in, '=');
        Expr* rhs = parse_expr(in);
        return new EqualsExpr(e,rhs);
    } else{
        return e;
    }
}
//TODO: fix
Expr* parse_let(std::istream &in){
    skip_whitespace(in);
    std::string variable;
    //take parsed variable expression and convert it to a string representation
    variable = parse_var(in)->to_string();
    skip_whitespace(in);
    int c = in.peek();
    if(c =='='){
        consume(in, '=');
    }
    //parse first expression and assign to the bound variable for the letExpr return
    Expr* bound = parse_expr(in);
    Expr* body;
    if (parse_keyword(in) == "_in"){
        //take the keyword and then parse the body expr
        body= parse_expr(in);
    } else{
        throw std::runtime_error("there needs to be an '_in' after _let");
    }
    return new LetExpr(variable,bound,body);
}

Expr* parse_if(std::istream &in) {
    //The first parse just like the branches has to be an expression, a condition that we can branch from using true and false
    Expr* cond = parse_expr(in);
    skip_whitespace(in);
    if (parse_keyword(in) != "_then") {
        throw std::runtime_error("There needs to be a '_then' after the '_if' condition");
    }
    Expr* thenExpr = parse_expr(in);
    skip_whitespace(in);
    if (parse_keyword(in) != "_else") {
        throw std::runtime_error("There needs to be an '_else' statement after the '_then' expression");
    }
    Expr* elseExpr = parse_expr(in);

    return new IfExpr(cond, thenExpr, elseExpr);
}

Expr *parse(std::istream &in) {
    Expr* e;
    e = parse_comparg(in);
    skip_whitespace(in);
    int c = in.peek();
    //parse equality expression
    if(c =='='){
        consume(in,'=');
        consume(in,'=');
        Expr* rhs = parse_expr(in);
        return new EqualsExpr(e, rhs);
    } else {
        if (!in.eof()) {
            throw std::runtime_error("invalid input in parse");
        }
        return e;
    }
}
//Use parseString as a means to parse a stream of strings
Expr* parseString(const std::string &s) {
    std::istringstream in(s);
    return parse(in);
}

Expr* parseInput(){
    std::string input;
    getline( std::cin, input);
    std::stringstream ss(input);
    return parse(ss);
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
//
//
//
//#include <sstream>
//#include <iostream>
//#include "parse.hpp"
//
//
//void consume(std::istream &in, int expect) {
//    int c = in.get();
//    if (c!=expect) {
//        throw std::runtime_error("consume mismatch");
//    }
//}
//
//Expr *parse_num(std::istream &in) {
//    int n = 0;
//    bool negative = false;
//    if (in.peek() == '-') {
//        negative = true;
//        consume(in, '-');
//        if(!isdigit(in.peek())){
//            throw std::runtime_error("invalid input in parse num") ;
//        }
//    }
//    while (true) {
//        int c = in.peek();
//        if (isdigit(c))
//        {
//            consume(in, c);
//            n = n*10 + (c - '0');
//        }
//        else {
//            break;
//        }
//    }
//    if (negative){
//        n = n * -1;
//    }
//    return new NumExpr(n);
//}
//
//Expr *parse_var(std::istream &inn) {
//    std::string res;
//    if (!isalpha(inn.peek())) {
//        throw std::runtime_error("invalid input in parse var") ;
//    }
//    while (true) {
//        int c = inn.peek();
//        if (isalpha(c))
//        {
//            consume(inn, c);
//            res +=c;
//        }
//        else {
//            break;
//        }
//    }
//    return new VarExpr(res);
//}
//
//std::string parse_keyword(std::istream &in){
//    std::string returnString;
//    skip_whitespace(in);
//    int c = in.peek();
//    while (c!=' ' and c != -1 and c!='('){
//        returnString +=(char)c;
//        consume(in,c);
//        c = in.peek();
//    }
//    return returnString;
//}
//
//
//Expr *parse_let(std::istream &in){
//    skip_whitespace(in);
//    std::string var;
//    var = parse_var(in)->to_string();
//    skip_whitespace(in);
//    int c = in.peek();
//    if(c =='='){
//        consume(in, '=');
//    }
//
//    Expr* declare = parse_expr(in);
//    Expr* equation;
//    if (parse_keyword(in) == "_in"){
//        equation= parse_expr(in);
//    } else{
//        throw std::runtime_error("should be an _in after _let");
//    }
//    return new LetExpr(var,declare,equation);
//}
//
//void skip_whitespace(std::istream &in) {
//    while (true) {
//        int c = in.peek();
//        if (!isspace(c)){
//            break;
//        }
//        consume(in, c);
//    }
//}
//Expr* parse_expr(std::istream &in){
//    Expr *e;
//    e= parse_comparg(in);
//    skip_whitespace(in);
//    int c = in.peek();
//    if(c=='='){
//        consume(in, '=');
//        consume(in, '=');
//        Expr* rhs = parse_expr(in);
//        return new EqualsExpr(e,rhs);
//    } else{
//        return e;
//    }
//}
//
//Expr* parse_if(std::istream &in){
//    skip_whitespace(in);
//    Expr* ifState = parse_expr(in);
//    Expr* trueState;
//    Expr* falseState;
//    if (parse_keyword(in)=="_then"){
//        trueState = parse_expr(in);
//    } else{
//        throw std::runtime_error("_then should be follow after _if ");
//    }
//
//    if (parse_keyword(in)=="_else"){
//        falseState = parse_expr(in);
//    } else{
//        throw std::runtime_error("_else should be follow after _then ");
//    }
//    return new IfExpr(ifState,trueState,falseState);
//}
//
//Expr *parse_comparg(std::istream &in) {
//    Expr *e;
//    e = parse_addend(in);
//    skip_whitespace(in);
//    int c = in.peek();
//    if (c == '+') {
//        consume(in, '+');
//        Expr *rhs = parse_comparg(in);
//        return new AddExpr(e, rhs);
//    }
//    else{
//        return  e;
//    }
//}
//
//Expr* parse_fun(std::istream & in){
//    skip_whitespace(in);
//    consume(in,'(');
//    Expr* formal_arg = parse_var(in);
//    consume(in,')');
//    Expr* body = parse_expr(in);
//    return new FunExpr(formal_arg->to_string(),body);
//}
//Expr* parse_inner(std::istream &in){
//    skip_whitespace(in);
//    int c = in.peek();
//    if ((c == '-') || isdigit(c)) {
//        return parse_num(in);
//    } else if(isalpha(c)){
//        return parse_var(in);
//    } else if (c == '_'){
//        std::string kw = parse_keyword(in);
//        if (kw == "_let"){
//            return parse_let(in);
//        }else if (kw == "_true"){
//            return new BooleanExpr(true);
//        } else if(kw=="_false"){
//            return new BooleanExpr(false);
//        } else if (kw =="_if"){
//            return parse_if(in);
//        } else if(kw =="_fun"){
//            return parse_fun(in);
//        }
//
//    }
//    else if (c == '(') {
//        consume(in, '(');
//        Expr *e = parse_expr(in);
//        skip_whitespace(in);
//        c = in.get();
//        if (c != ')') {
//            throw std::runtime_error("missing close parenthesis");
//        }
//        return e;
//    } else {
//        consume(in, c);
//        throw std::runtime_error("invalid input in parse multicand");
//    }
//}
//Expr *parse_multicand(std::istream &in) {
//    Expr *e;
//    e = parse_inner(in);
//    skip_whitespace(in);
//    while (in.peek()=='('){
//        consume(in,'(');
//        Expr* actual_arg = parse_expr(in);
//        consume(in,')');
//        e = new CallExpr(e,actual_arg);
//    }
//    return e;
//}
//
//Expr *parse_addend(std::istream &in) {
//    Expr *e;
//    e = parse_multicand(in);
//    skip_whitespace(in);
//    int c = in.peek();
//    if (c == '*') {
//        consume(in, '*');
//        skip_whitespace(in) ;
//        Expr *rhs = parse_addend(in);
//        return new MultExpr(e, rhs);
//    }
//    else{
//        return e ;
//    }
//}
//
//
//Expr *parse(std::istream &in) {
//    Expr* e;
//    e = parse_comparg(in);
//    skip_whitespace(in);
//    int c = in.peek();
//    if(c =='='){
//        consume(in,'=');
//        consume(in,'=');
//        Expr* rhs = parse_expr(in);
//        return new EqualsExpr(e, rhs);
//    } else {
//        if (!in.eof()) {
//            throw std::runtime_error("invalid input in parse");
//        }
//        return e;
//    }
//}
//
//Expr* parseInput(){
//    std::string input;
//    getline( std::cin, input);
//    std::stringstream ss(input);
//    return parse(ss);
//}
//
//
//Expr* parseString(const std::string& s){
//    std::istringstream in(s);
//    return parse(in);
//}