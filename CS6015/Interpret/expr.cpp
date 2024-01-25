//
// Created by Jakob West on 1/23/24.
//
#include "expr.hpp"
#include <stdexcept>
using namespace std;
AddExpr::AddExpr(Expr* lhs, Expr* rhs){
    this->lhs = lhs; //sending the pointer of Expr for lhs and directly assigning it to the addexpr lhs
    // heap space - heap space is faster
    this->rhs = rhs;
}
bool AddExpr::equals(Expr* e){
    //casting the AddExpr type to e
    //e is a subclass to expression if it can cast to AddExpr our
    // method is working properly, if it (addExpr) is null this is not possible
    AddExpr* addExpr = dynamic_cast<AddExpr*>(e);
    if(addExpr == NULL){
        //wont be an instance of expression, the object type AddExpr* isnt of type Expr
        return false;
    }else{
        //if member pointer is equivalent to object AddExpr pointer return addExpr
        //check that the member variables are the same that addExpr utilizes
        return this->lhs->equals(addExpr->lhs) && rhs->equals(addExpr->rhs);
    }
}
Expr* AddExpr::subst(const string& targetVarName, Expr* replacementVar){
    return new AddExpr(lhs->subst(targetVarName, replacementVar),rhs->subst(targetVarName, replacementVar));
}

int AddExpr::interp() {
    //return an int for the value of the expression
    // the value of addition expression is the sum of teh two subexpressions
    return lhs->interp() + rhs->interp();
}

bool AddExpr::has_variable() {
    return lhs->has_variable() || rhs->has_variable();
}


Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool Mult::equals(Expr *e){
    Mult *m = dynamic_cast<Mult*>(e);
    if(m ==NULL){
        return false;
    } else{
        return lhs->equals(m->lhs) && rhs->equals(m->rhs);
    }
}

int Mult::interp() {
    return lhs->interp() * rhs->interp();
}

bool Mult::has_variable() {
    //return true if the lhs-> or rhs-> is a variable
    return lhs->has_variable() || rhs->has_variable();
}

Expr* Mult::subst(const string& targetVarName, Expr* replacementVar) {
    //recursively look at the left hand and right hand side expression to see
    //if there is the target variable, if the target variable exists
    //on either the lhs or rhs of the multiplication expression, it can be replaced by
    //a new expression -- replacementVar
    return new Mult(lhs->subst(targetVarName, replacementVar), rhs->subst(targetVarName,replacementVar));
}

NumExpr::NumExpr(int val) {
    this->_val = val;
}
bool NumExpr::equals(Expr* e){
    NumExpr* num = dynamic_cast<NumExpr*>(e);
    if (num == NULL){
        return false;
    }else{
        return _val == num->_val;
    }

}

int NumExpr::interp() {
    //just return the member variable of the number expression when interpreting the number
    return _val;
}

bool NumExpr::has_variable() {
    return false;
}

Expr* NumExpr::subst(const string &targetmatch, Expr *e) {
    //you cannot substitute a string for a number
    //only a string for a string
    return new NumExpr(this->_val);
}
VarExpr::VarExpr(string val) {
    _val = val;
}
bool VarExpr::equals(Expr* e){
    auto* varexpr = dynamic_cast<VarExpr*>(e);
    if (varexpr == NULL){
        return false;
    }else{
        return _val == varexpr->_val;
    }

}

int VarExpr::interp()
{
     throw runtime_error("no value for variable");
}

bool VarExpr::has_variable() {
    return true;
}

Expr* VarExpr::subst(const string& targetvar, Expr* replacementVar) {
    //if we have expr pointer of type VarExpr, 'x', and our target from the original
    //object holds 'x' as it's value, we will change return a newVarName (holding 'y')
    //where our new Expr* result will point to replacement VarExpr (containing 'y')
    if(this->_val==targetvar){
        //return Expr that holds val->'y'
        return replacementVar;
    }else{

        //otherwise create a new variable expression object and give it the same original value
        //i.e., if 'x' is our og, and our targetvar isn't 'x' we can't replace the variable,
        //so we just create our new expression variable with this-> _val (string)
        // we already have stored
        return new VarExpr(this->_val);

    }
}
