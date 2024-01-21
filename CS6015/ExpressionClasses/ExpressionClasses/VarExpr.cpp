//
// Created by Jakob West on 1/20/24.
//
#include "VarExpr.hpp"
VarExpr::VarExpr(string val) {
    this->val = val;
}
bool VarExpr::equals(Expr *e){
    VarExpr *n = dynamic_cast<VarExpr*>(e);
    if (n == NULL){
        return false;
    }else{
        return this->val == n->val;
    }

}