//
//  expr.hpp
//  ExpressionClasses
//
//  Created by Jakob West on 1/19/24.
//

#ifndef expr_hpp
#define expr_hpp

#include <stdio.h>
#include <string>

using namespace std;
class Expr {
    public:
        virtual bool equals(Expr *e) = 0;
};
class AddExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    AddExpr(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
};
class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
};
class NumExpr : public Expr {
public:
    int _val;
    NumExpr(int _val);
    bool equals(Expr *e) override;

};
class VarExpr : public Expr {
public:
    string val;
    VarExpr(string val);
    bool equals(Expr *e) override;

};


#endif /* expr_hpp */
