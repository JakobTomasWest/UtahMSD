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
bool NumVal::equals(Val* otherVal) {
    auto* numVal = dynamic_cast<NumVal*>(otherVal);
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
Expr* NumVal::to_expr() {
    return new NumExpr(numV);
}
/**
 * \brief Adds the integer values of the current NumVal Object to another Val Object
 * @param val object to add to
 * @return added NumVal
 */
Val* NumVal::add_to(Val* val) {
    //ensure that val is an instance of NumVal so we can add
    auto* numVal = dynamic_cast<NumVal*>(val);
    if(numVal== nullptr){
        throw std::runtime_error("have to add to a number");
    }
    //add the integer value numV of the current object to the integer value of numVal
    return new NumVal((unsigned)numV+(unsigned)numVal->numV);
}
/**
 * \brief Multiplies the integer values of the current NumVal Object to another Val Object
 * @param val object to add to
 * @return multiplied NumVal
 */
Val* NumVal::mult_with(Val *val) {
    auto* numVal = dynamic_cast<NumVal*>(val);
    if(numVal== nullptr){
        throw std::runtime_error("have to multiply to a number");
    }
    return new NumVal((unsigned)numV*(unsigned)numVal->numV);
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
bool BoolVal::equals(Val* val) {
    auto* booleanValue = dynamic_cast<BoolVal*>(val);
    if(booleanValue== nullptr){
        throw std::runtime_error("compare to non-boolean");
    }
    return _value==booleanValue->_value;
}
/**
 * \brief Converts the BoolVal into a BooleanExpr expression.
 * @return Returns a new BooleanExpr object initialized with the BoolVal's boolean value.
 */

Expr* BoolVal::to_expr() {
    return new BooleanExpr(_value);
}
/**
 * \brief Throws an error because addition operation is not applicable for boolean values.
 * @param val The value to add to the current BoolVal object.
 * @throw Throws a runtime_error indicating that addition operation is invalid for boolean types.
 */

Val* BoolVal::add_to(Val* val) {
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

Val* BoolVal::mult_with(Val *val) {
    throw std::runtime_error("mult doesn't work on boolean");
}
/**
 * \brief Prints the boolean value of the BoolVal object to the provided output stream.
 * @param out The output stream where the boolean value will be printed.
 */

void BoolVal::print(std::ostream &out) {
    out<<std::to_string(_value);
}
