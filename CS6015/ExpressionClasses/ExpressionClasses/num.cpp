//
// Created by Jakob West on 1/20/24.
//
#include "num.hpp"
NumExpr::NumExpr(int val) {
    this->val = val;
}
bool NumExpr::equals(Expr *e){
    NumExpr *num = dynamic_cast<NumExpr*>(e);
    if (num == NULL){
        return false;
    }else{
        return this->val == num->val;
    }

}