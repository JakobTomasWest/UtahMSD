//
// Created by Jakob West on 1/20/24.
//

#include "add.hpp"
AddExpr::AddExpr(Expr *lhs, Expr *rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}
bool AddExpr::equals(Expr *e){
    AddExpr *addExpr = dynamic_cast<AddExpr*>(e);
    if(addExpr == NULL){
        return false;
    }else{
        return addExpr != nullptr && lhs->equals(addExpr->lhs) && rhs->equals(addExpr->rhs);
    }
}