//
// Created by Jakob West on 1/20/24.
//

#include "mult.hpp"
Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool Mult::equals(Expr *e){
    Mult *m = dynamic_cast<Mult*>(e);
    if(m ==NULL){
        return false;
    } else{
        return m != nullptr && lhs->equals(m->lhs) && rhs->equals(m->rhs);
    }
}