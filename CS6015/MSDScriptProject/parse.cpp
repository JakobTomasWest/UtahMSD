//#include <iostream>
//#include "expr.hpp"
//
////Expr *parse_expr (std::istream & inn) {
////    skip_whitespace (inn);
////
////    int c = inn.peek();
////    if ((c == '-') || isdigit(c))
////      return parse_num(inn);
////    else if (c == '(') {
////      consume(inn, '(');
////      Expr *e = parse_expr(inn);
////      skip_whitespace(inn);
////      c = inn.get();
////      if (c != ')')
////        throw std::runtime_error("missing close parenthesis");
////      return e;
////    } else {
////      consume(inn, c);
////      throw std::runtime_error("invalid input");
////  }
////}
//
//Expr *parse_num(std::istream &inn) {
//    int n = 0;
//    bool negative = false;
//
//    if (inn.peek() == '-') {
//        negative = true;
//        consume (inn, '-');
//
//    }
//
//    while (1) {
//        int c = inn.peek();
//        if (isdigit(c))
//        {
//            consume(inn, c);
//            n = n*10 + (c - '0');
//        }
//        else
//            break;
//    }
//
//    if (negative)
//        n = -n;
//
//    return new NumExpr(n);
//}
//
//
//static void consume(std::istream &in, int expect) {
//    int c = in.get();
//    if (c!=expect) {
//        throw std::runtime_error("consume mismatch");
//    }
//}
//
//void skip_whitespace(std::istream &in) {
//  while (1) {
//    int c = in.peek();
//    if (!isspace(c))
//      break;
//    consume(in, c);
//      }
//}
//
//Expr *parse_addend(std::istream &in) {
//    skip_whitespace(in);
//    int c = in.peek();
//    if ((c == '-') || isdigit(c))
//        return parse_num(in);
//    else if (c == '(') {
//        consume(in, '(');
//        Expr *e = parse_expr(in);
//        skip_whitespace(in);
//        c = in.get();
//        if (c != ')')
//            throw std::runtime_error("missing close parenthesis");
//        return e;
//    } else {
//        consume(in, c);
//        throw std::runtime_error("invalid input");
//    }
//}
//
// static Expr *parse_expr(std::istream &in) {
//    Expr *e;
//    e = parse_addend(in);
//    skip_whitespace(in);
//    int c = in.peek();
//    if (c == '+') {
//        consume(in, '+');
//        Expr *rhs = parse_expr(in);
//        return new AddExpr(e, rhs);
//    } else
//        return e;
//}
//
////isAlpha() -->
////_--> read 3 characters --> should be followed by varName --> = --> (spaces --> )expr ---> (spaces -->) _in --> expr
//
////parse _var helper
//// parse_let jelper
////parse_keyword