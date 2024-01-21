//
// Created by Jakob West on 1/20/24.
//
#include "expr.hpp"
#ifndef EXPRESSIONCLASSES_ADD_HPP
#define EXPRESSIONCLASSES_ADD_HPP

class AddExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    AddExpr(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
};
#endif //EXPRESSIONCLASSES_ADD_HPP
