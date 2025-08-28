//
// Created by Jakob West on 1/23/24.
//

#ifndef INTERPRET_EXPR_HPP
#define INTERPRET_EXPR_HPP
#include"pointer.hpp"
#include <stdio.h>
#include <string>
#include "env.h"
using namespace std;
class Val;
CLASS (Expr) {
public:
    typedef enum {
        prec_none,      // = 0
        prec_add,       // = 1
        prec_mult       // = 2
    } precedence_t;
    virtual ~Expr() {};
    virtual bool equals(PTR(Expr) e) = 0;
    virtual PTR(Val)interp(PTR(Env) env) = 0;
    virtual PTR(Expr) subst(const string& givenVarName, PTR(Expr) e) = 0;
    virtual void print(std::ostream& out) const = 0;
    virtual bool has_variable() = 0;
    virtual void pretty_print(std::ostream& out) const;
    virtual void pretty_printHelper(std::ostream &ostream, precedence_t precedence, bool needsParen,
                                    std::streampos spaces) const;
    string to_string() const;
    string to_pretty_string() const;
};
class AddExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    AddExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
    PTR(Expr) subst(const string& givenVarName, PTR(Expr) e) override;
    void print(ostream &ot) const override;
    void pretty_print(std::ostream &ostream) const override;
    void pretty_printHelper(std::ostream &ostream, precedence_t precedence, bool needsParen, std::streampos spaces) const override;

};


class MultExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    MultExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
    PTR(Expr) subst(const string& givenVarName, PTR(Expr) e) override;
    void print(ostream &ot) const override;
    void pretty_print(std::ostream &ostream) const override;
    void pretty_printHelper(std::ostream &ostream, precedence_t precedence, bool needsParen, std::streampos spaces) const override;
};


class NumExpr : public Expr {
public:
    int _val;
    NumExpr(int _val);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
    PTR(Expr) subst(const string& givenVarName, PTR(Expr) e) override;
    void print(ostream &ot) const override;
};


class VarExpr : public Expr {
public:
    string _val;
    VarExpr(string val);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
    PTR(Expr) subst(const string& givenVarName, PTR(Expr) e) override;
    void print(ostream &ot) const override;

};

class LetExpr : public Expr{
private:
    std::string varName;
    PTR(Expr)  _boundExpr;
    PTR(Expr)  _bodyExpr;

public:
    LetExpr(const string &var, PTR(Expr) boundExpr, PTR(Expr) bodyExpr);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
    PTR(Expr) subst(const string& givenVarName, PTR(Expr) e) override;
    void print(ostream &ot) const override;
    void pretty_print(std::ostream &ostream) const override;
    void pretty_printHelper(std::ostream &ostream, precedence_t precedence, bool needsParen, std::streampos spaces) const override;

};
class BooleanExpr:public Expr{
public:
    bool _boolean;
    explicit BooleanExpr(bool val);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
    PTR(Expr) subst(const string& givenVarName, PTR(Expr) e) override;
    void print(std::ostream& ostream) const override;
};

class IfExpr:public Expr{
public:
    PTR(Expr) _condition;
    PTR(Expr) _true;
    PTR(Expr) _false;
    IfExpr(PTR(Expr) cond,PTR(Expr) thenTrue, PTR(Expr) elseFalse);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
    PTR(Expr) subst(const string& givenVarName, PTR(Expr) e) override;
    void print(std::ostream& out) const override;
    virtual void pretty_print(std::ostream& out) const;
    void pretty_printHelper(std::ostream& out, precedence_t precedence, bool needsParen, std::streampos spaces) const override;
};

class EqualsExpr:public Expr{
public:
    PTR(Expr) _lhs;
    PTR(Expr) _rhs;
    EqualsExpr(PTR(Expr) lhs,PTR(Expr) rhs);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
    PTR(Expr)  subst(const string& givenVarName, PTR(Expr) e) override;
    void print(std::ostream& ostream) const override;
    virtual void pretty_print(std::ostream& out) const;
    void pretty_printHelper(std::ostream &ostream, precedence_t precedence, bool needsParen, std::streampos spaces) const override;
};

class FunExpr:public Expr {
public:
    string formal_arg;
    PTR(Expr) body;
    FunExpr(std::string formal_arg, PTR(Expr) body);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
    void print(std::ostream &out) const override;
    PTR(Expr)  subst(const string& givenVarName, PTR(Expr) e) override;
};

class CallExpr:public Expr {
public:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;
    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
    void print(std::ostream &out) const override;
    PTR(Expr)  subst(const string& givenVarName, PTR(Expr) e) override;
};





#endif //INTERPRET_EXPR_HPP
