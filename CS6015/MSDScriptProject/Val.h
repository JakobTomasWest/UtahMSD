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
    std::string to_string();
    virtual void print(std::ostream& ostream)=0;
    virtual bool is_true() =0;
};

class NumVal:public Val{
public:
    int numV;
    explicit NumVal(int num);
    bool equals(Val* otherVal) override;
    Expr* to_expr() override;
    Val* add_to(Val* val) override;
    Val* mult_with(Val* val) override;
    void print(std::ostream& ostream) override;
    bool is_true() override;
};


//class BoolVal:public Val{
//public:
//    bool _val;
//    explicit BoolVal( bool val);
//    bool equals(Val* val) override;
//    Expr* to_expr() override;
//    Val* add_to(Val* val) override;
//    Val* mult_with(Val* val) override;
//    void print(std::ostream& ostream) override;
//    bool is_true() override;
//};
#endif //LAB03_VAL_H
