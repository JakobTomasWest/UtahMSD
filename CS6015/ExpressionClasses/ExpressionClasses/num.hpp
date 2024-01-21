//
// Created by Jakob West on 1/20/24.
//
#include "expr.hpp"
#ifndef EXPRESSIONCLASSES_NUM_HPP
#define EXPRESSIONCLASSES_NUM_HPP
class NumExpr : public Expr {
public:
    int val;
    NumExpr(int val);
    bool equals(Expr *e) override;

};
#endif //EXPRESSIONCLASSES_NUM_HPP
