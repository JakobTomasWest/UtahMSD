//
// Created by Jakob West on 3/11/24.
//
#include "Val.h"
#include "expr.hpp"
#include <string>
#include <sstream>

NumVal::NumVal(int num) {
    numV=num;
}

bool NumVal::equals(Val *otherVal) {
    auto* numVal = dynamic_cast<NumVal*>(otherVal);
    if(numVal== nullptr){
        throw std::runtime_error("have to compare against another number");
    }
    return numV == numVal->numV;
}

Expr *NumVal::to_expr() {
    return new NumExpr(numV);
}

Val *NumVal::add_to(Val *val) {
    auto* numVal = dynamic_cast<NumVal*>(val);
    if(numVal== nullptr){
        throw std::runtime_error("have to add to a number");
    }
    return new NumVal(numV+numVal->numV);
}

Val *NumVal::mult_with(Val *val) {
    auto* numVal = dynamic_cast<NumVal*>(val);
    if(numVal== nullptr){
        throw std::runtime_error("have to multiply to a number");
    }
    return new NumVal(numV*numVal->numV);
}

void NumVal::print(std::ostream &ostream) {
    ostream<<std::to_string(numV);
}

bool NumVal::is_true() {
    throw std::runtime_error("NumVal is not true/false");
}

std::string Val::to_string() {
    std::stringstream st(" ");
    print(st);
    return st.str();
}