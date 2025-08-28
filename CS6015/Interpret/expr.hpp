//
// Created by Jakob West on 1/23/24.
//

#ifndef INTERPRET_EXPR_HPP
#define INTERPRET_EXPR_HPP

#include <stdio.h>
#include <string>

using namespace std;
class Expr {
public:
    virtual bool equals(Expr *e) = 0;
    virtual bool has_variable() =0;
    virtual int interp() = 0;
    virtual Expr* subst(const string& givenVarName, Expr* e) = 0;

};


class AddExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    AddExpr(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(const string& givenVarName, Expr* e) override;
};


class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(const string& givenVarName, Expr* e) override;
};


class NumExpr : public Expr {
public:
    int _val;
    NumExpr(int _val);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(const string& givenVarName, Expr* e) override;
};


class VarExpr : public Expr {
public:
    string _val;
    VarExpr(string val);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(const string& givenVarName, Expr* e) override;
};





#endif //INTERPRET_EXPR_HPP
