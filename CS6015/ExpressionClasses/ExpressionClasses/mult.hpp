//
// Created by Jakob West on 1/20/24.
//
#include "expr.hpp"
#ifndef EXPRESSIONCLASSES_MULT_HPP
#define EXPRESSIONCLASSES_MULT_HPP
class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
};
#endif //EXPRESSIONCLASSES_MULT_HPP
