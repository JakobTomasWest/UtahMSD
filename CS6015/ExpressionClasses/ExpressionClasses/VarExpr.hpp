//
// Created by Jakob West on 1/20/24.
//
#include "expr.hpp"
#ifndef EXPRESSIONCLASSES_VAREXPR_HPP
#define EXPRESSIONCLASSES_VAREXPR_HPP
class VarExpr : public Expr {
public:
    string val;
    VarExpr(string val);
    bool equals(Expr *e) override;

};
#endif //EXPRESSIONCLASSES_VAREXPR_HPP
