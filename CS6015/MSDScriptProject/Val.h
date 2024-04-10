//
// Created by Jakob West on 3/11/24.
//

#ifndef LAB03_VAL_H
#define LAB03_VAL_H
#include <iostream>
#include "pointer.hpp"
#include "env.h"
class Expr;
CLASS(Val) {
public:
    virtual ~Val() {};
    virtual bool equals(PTR(Val) val) = 0;
    virtual PTR(Expr)  to_expr() = 0;
    virtual PTR(Val) add_to(PTR(Val) val) = 0;
    virtual PTR(Val) mult_with(PTR(Val) val) = 0;
    virtual std::string to_string();
    virtual void print(std::ostream& out)=0;
    virtual bool is_true() =0;
    virtual PTR(Val) call(PTR(Val) actual_arg) = 0;
};

class NumVal:public Val{
public:
    int numV;
    explicit NumVal(int num);
    bool equals(PTR(Val) otherVal) override;
    PTR(Expr)  to_expr() override;
    PTR(Val) add_to(PTR(Val) al) override;
    PTR(Val) mult_with(PTR(Val) val) override;
    void print(std::ostream& out) override;
    bool is_true() override;
    PTR(Val) call(PTR(Val) actual_arg) override;
};


class BoolVal:public Val{
public:
    bool _value;
    explicit BoolVal( bool val);
    bool equals(PTR(Val) val) override;
    PTR(Expr)  to_expr() override;
    PTR(Val) add_to(PTR(Val) val) override;
    PTR(Val) mult_with(PTR(Val) val) override;
    void print(std::ostream& out) override;
    bool is_true() override;
    PTR(Val) call(PTR(Val) actual_arg) override;
};

class FunVal : public Val {
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;

public:

    FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env);
    PTR(Expr)  to_expr() override;
    bool equals(PTR(Val)  rhs) override;
    PTR(Val)  add_to(PTR(Val)  rhs)override;
    PTR(Val)  mult_with(PTR(Val) rhs) override;
    void print(std::ostream& out) override;
    bool is_true() override;
    PTR(Val) call(PTR(Val) actual_arg) override;
};
#endif //LAB03_VAL_H
