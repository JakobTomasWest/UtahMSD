//
// Created by Jakob West on 3/11/24.
//
#include "Val.h"
#include "expr.hpp"
#include <string>
#include <sstream>
/**
 * \brief Constructor for NumVal class
 * @param num value we are giving it
 */
NumVal::NumVal(int num) {
    numV=num;
}
/**
 * \brief Attempts to cast the Val object to type numVal
 * @param otherVal the object we are trying to attempt to cast to type NumVal
 * @return
 */
bool NumVal::equals(PTR(Val) otherVal) {
    PTR(NumVal)  numVal = CAST(NumVal)(otherVal);
    if(numVal== nullptr){
        throw std::runtime_error("have to compare against another number");
    }
    //Compare the integer values of the current Val Object and numVal
    return numV == numVal->numV;
}
/**
 * \brief Creating an expression from the integer value of the NumVal Object
 * @return returns a Num expr
 */
PTR(Expr) NumVal::to_expr() {
    return NEW(NumExpr)(numV);
}
/**
 * \brief Adds the integer values of the current NumVal Object to another Val Object
 * @param val object to add to
 * @return added NumVal
 */
PTR(Val) NumVal::add_to(PTR(Val) val) {
    //ensure that val is an instance of NumVal so we can add
    PTR(NumVal)  numVal = CAST(NumVal)(val);
    if(numVal== nullptr){
        throw std::runtime_error("have to add to a number");
    }
    //add the integer value numV of the current object to the integer value of numVal
    return NEW(NumVal)((unsigned)numV+(unsigned)numVal->numV);
}
/**
 * \brief Multiplies the integer values of the current NumVal Object to another Val Object
 * @param val object to add to
 * @return multiplied NumVal
 */
PTR(Val) NumVal::mult_with(PTR(Val) val) {
    PTR(NumVal) numVal = CAST(NumVal)(val);
    if(numVal== nullptr){
        throw std::runtime_error("have to multiply to a number");
    }
    return NEW(NumVal)((unsigned)numV*(unsigned)numVal->numV);
}
/**
 * \brief  take numVal and print it the ostream in string format
 * @param out
 */
void NumVal::print(std::ostream &out) {
    out<<std::to_string(numV);
}


bool NumVal::is_true() {
    throw std::runtime_error("NumVal is not true/false");
}

PTR(Val) NumVal::call(PTR(Val) actual_arg) {
    throw std::runtime_error("NumVal cannot call");
}
/**
 * \brief convert current val object to string
 * @return returns resulting string of print method on stringstream
 */
std::string Val::to_string() {
    std::stringstream st(" ");
    print(st);
    return st.str();
}
/**
 * \brief Constructor for Boolean Value objects
 * @param val assign a boolean type to the object
 */
BoolVal::BoolVal(bool val) {
    _value=val;
}
/**
 * \brief Attempts to cast Val object to BoolVal type and evaluate whether the booleanValue is equivalent to our member Var Boolean value
 * @param val
 * @return returns true if the boolValue object and our member are the same
 */
bool BoolVal::equals(PTR(Val) val) {
    PTR(BoolVal) booleanValue = CAST(BoolVal)(val);
    if(booleanValue== nullptr){
        throw std::runtime_error("compare to non-boolean");
    }
    return _value==booleanValue->_value;
}
/**
 * \brief Converts the BoolVal into a BooleanExpr expression.
 * @return Returns a new BooleanExpr object initialized with the BoolVal's boolean value.
 */

PTR(Expr)BoolVal::to_expr() {
    return NEW(BooleanExpr) (_value);
}
/**
 * \brief Throws an error because addition operation is not applicable for boolean values.
 * @param val The value to add to the current BoolVal object.
 * @throw Throws a runtime_error indicating that addition operation is invalid for boolean types.
 */

PTR(Val) BoolVal::add_to(PTR(Val) val) {
    throw std::runtime_error("add doesn't work on boolean");
}
/**
 * \brief Checks if the BoolVal object's value is true.
 * @return Returns true if the BoolVal's value is true, otherwise returns false.
 */

bool BoolVal::is_true() {
    return _value;
}
/**
 * \brief Throws an error because multiplication operation is not applicable for boolean values.
 * @param val The value to multiply with the current BoolVal object.
 * @throw Throws a runtime_error indicating that multiplication operation is invalid for boolean types.
 */

PTR(Val) BoolVal::mult_with(PTR(Val) val) {
    throw std::runtime_error("mult doesn't work on boolean");
}

/**
 * \brief Prints the boolean value of the BoolVal object to the provided output stream.
 * @param out The output stream where the boolean value will be printed.
 */

void BoolVal::print(std::ostream &out) {
    out<<std::to_string(_value);
}

PTR(Val) BoolVal::call(PTR(Val) actual_arg) {
    throw std::runtime_error("BoolVal cannot call");
}

FunVal::FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env) {
    this->formal_arg = formal_arg;
    this->body = body;
    this->env = env;
}

PTR(Expr) FunVal::to_expr() {
    return NEW (FunExpr)(this->formal_arg, this->body);
}

bool FunVal::equals(PTR(Val) rhs) {
    PTR(FunVal) expr = CAST(FunVal) (rhs);
    if (expr == nullptr) {
        return false;
    }
    else {
        return this->formal_arg == expr->formal_arg && this->body->equals(expr->body);
    }

}

PTR(Val) FunVal::add_to(PTR(Val) rhs) {
    throw std::runtime_error("Addition cannot be performed on a function-value.");
}

PTR(Val) FunVal::mult_with(PTR(Val) rhs) {
    throw std::runtime_error("Multiplication cannot be performed on a function-value.");
}


PTR(Val) FunVal::call(PTR(Val) actual_arg) {
    return body->interp(NEW(ExtendedEnv)(formal_arg, actual_arg, env));

}
void FunVal::print(std::ostream& out){
    out << "(_fun (";
    out << this->formal_arg;
    out << ") ";
    this->body->print(out);
    out << ")";
}

bool FunVal::is_true() {
    throw std::runtime_error("A function cannot be used as a boolean value.");
}
