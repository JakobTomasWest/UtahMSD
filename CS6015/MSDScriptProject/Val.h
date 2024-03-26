//
// Created by Jakob West on 3/11/24.
//

#ifndef LAB03_VAL_H
#define LAB03_VAL_H
#include <iostream>
class Expr;
class Val {
public:
    virtual bool equals(Val* val) = 0;
    virtual Expr* to_expr() = 0;
    virtual Val* add_to(Val* val) = 0;
    virtual Val* mult_with(Val* val) = 0;
    virtual std::string to_string();
    virtual void print(std::ostream& out)=0;
    virtual bool is_true() =0;
    virtual Val *call(Val* actual_arg) = 0;
};

class NumVal:public Val{
public:
    int numV;
    explicit NumVal(int num);
    bool equals(Val* otherVal) override;
    Expr* to_expr() override;
    Val* add_to(Val* val) override;
    Val* mult_with(Val* val) override;
    void print(std::ostream& out) override;
    bool is_true() override;
    Val *call(Val* actual_arg) override;
};


class BoolVal:public Val{
public:
    bool _value;
    explicit BoolVal( bool val);
    bool equals(Val* val) override;
    Expr* to_expr() override;
    Val* add_to(Val* val) override;
    Val* mult_with(Val* val) override;
    void print(std::ostream& out) override;
    bool is_true() override;
    Val* call(Val* actual_arg) override;
};

class FunVal : public Val {
    std::string formal_arg;
    Expr* body;

public:
    FunVal(std::string formal_arg, Expr *body);
    Expr* to_expr() override;
    bool equals(Val* rhs) override;
    Val* add_to(Val* rhs)override;
    Val* mult_with(Val* rhs) override;
    void print(std::ostream& out) override;
    bool is_true() override;
    Val *call(Val* actual_arg) override;
};
#endif //LAB03_VAL_H
