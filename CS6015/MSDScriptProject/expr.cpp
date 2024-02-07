//
// Created by Jakob West on 1/23/24.
//
#include "expr.hpp"
#include <stdexcept>
#include <sstream>


using namespace std;

typedef enum {
    prec_none,      // = 0
    prec_add,       // = 1
    prec_mult       // = 2
} precedence_t;
// precedence_t n = precedence_t::prec_none;

precedence_t exprToPrecedence(Expr* e ){
    if(dynamic_cast<AddExpr*>(e) != nullptr ){
        return prec_add;
    } else if (dynamic_cast<Mult*>(e)!= nullptr){
        return prec_mult;
    } else{
        return prec_none;
    }
}
/**
 * \brief Converts the expression to a standard string representation.
 *
 * \return A  string that represents the expression.
 */
std::string Expr::to_string() const {
    // explicit stringstream st = new stringstream("");
    stringstream st("");
    this->print(st);
    //take string stream and convert to a regular string .str()
    return st.str();
}
/**
 * \brief Converts the expression to pretty-printed string representation.
 *  \return A string that is pretty-printed representation of the expression.
 */
std::string Expr::to_pretty_string() const {
    // explicit stringstream st = new stringstream("");
    stringstream st("");
    this->pretty_print(st);
    //take string stream and convert to a regular string .str()
    return st.str();
}
/**
 * \brief Provides a basic form of pretty printing to fallback on and is overridden by the required derived/concrete mult and Addexpr
 *  pretty_prints
 *  \param out The output stream where the expression will be pretty-printed
 */
void Expr::pretty_print(std::ostream& out) const {
    this->print(out);
}

/**
 * \brief Contructs an addition expression with left hand and right hand operands
 * \param lhs Pointer to the left hand side expression.
 * \param rhs Pointer to the left hand side expression.
 */
AddExpr::AddExpr(Expr* lhs, Expr* rhs){
    this->lhs = lhs; //sending the pointer of Expr for lhs and directly assigning it to the addexpr lhs
    // heap space - heap space is faster
    this->rhs = rhs;
}

/**
 * \brief Check if this addtion expression equals another expression.
 * \param e Pointer to the expression to compare with.
 * return True if the expression are equal, return false if the expressions are not equal.
 */
bool AddExpr::equals(Expr* e){
    //casting the AddExpr type to e
    //e is a subclass to expression if it can cast to AddExpr our
    // method is working properly, if it (addExpr) is null this is not possible
    AddExpr* addExpr = dynamic_cast<AddExpr*>(e);
    if(addExpr == nullptr){
        //wont be an instance of expression, the object type AddExpr* isnt of type Expr
        return false;
    }else{
        //if member pointer is equivalent to object AddExpr pointer return addExpr
        //check that the member variables are the same that addExpr utilizes
        return this->lhs->equals(addExpr->lhs) && rhs->equals(addExpr->rhs);
    }
}
/**
 * \brief Substitutes a variable within the expression with a replacment expression.
 * \param targetVarName The name of the variable/string to be substituted.
 * \param replacementVar Pointer to the expression that will replace the variable/string.
 * \return A new expression with the variable substituted.
 */
Expr* AddExpr::subst(const string& targetVarName, Expr* replacementVar){
    return new AddExpr(lhs->subst(targetVarName, replacementVar),rhs->subst(targetVarName, replacementVar));
}
/**
 * \brief Evaluates the addition expression and return the sum
 * \return The integer result of the summed addition.
 */
int AddExpr::interp() {
    //return an int for the value of the expression
    // the value of addition expression is the sum of teh two subexpressions
    return lhs->interp() + rhs->interp();
}
/**
 *\brief Determines if the addition expression contains a variable
 *\return True if the expression contains a variable, false if it doesn't
 */
bool AddExpr::has_variable() {
    return lhs->has_variable() || rhs->has_variable();
}
/**
 * \brief Prints the addition expression
 * \param ot The output stream to print to.
 */
void AddExpr::print(std::ostream& ot)const{

    ot << "(" << lhs->to_string() << "+" << rhs->to_string() << ")";
}
/**
 * \brief Prints the addition expression in a pretty-printed format, using operation precedence.
 * \param out The output stream to print to.
 */
void AddExpr::pretty_print(std::ostream &out) const {
    //get precedence value
   precedence_t lhs_prec = exprToPrecedence(this->lhs);
   //precedence_t rhs_prec = exprToPrecedence(this->rhs);
   //case for mult expression inside AddExpr
   if (lhs_prec == prec_mult) {
       //if LHS has multi precedence, it's printed because multiplication has higher precedence than addition, so
       //no parentheses are needed.
       // i.e., new AddExpr(new Mult(new NumExpr(1), new NumExpr(2)), new NumExpr(3));  --> since Mult>add prec --> lhs doesnt need parenthesis
       // --> 1 * 2 + 3
       this->lhs->pretty_print(out);
       //if LHS is another addition expression we have explicitly group it
       // i.,e newAdd(new newAdd( nn(1), nn(3)), nn (2) --> (1 + 3) + 2
   } else if (lhs_prec == prec_add) {

       out << "(";
       //calling pretty print on the outer AddExpr triggers the pretty printing of the inner AddExpr --> (1 + 3)
       this->lhs->pretty_print(out);
       out << ")";
   } else {
       //print as is 3 + 4
       this->lhs->pretty_print(out);
   }
    // indicate that its an addition expression and print rhs as is
    // --> + 2
   out << " + ";
   this->rhs->pretty_print(out);
}

/**
 \brief Constructs a multiplication expression with left side and right wide multiplication operands.
 \param lhs Pointer to the left-hand side expression.
 \param rhs Pointer to the right-hand side expression.
 */
Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
/**
 *\brief Checks if this multiplication expression equals another expression.
 *\param e Pointer to the expression to compare with.
 *\return True if the expressions are equal, otherwise false.
 */
bool Mult::equals(Expr *e){
    Mult *m = dynamic_cast<Mult*>(e);
    if(m == nullptr){
        return false;
    } else{
        return lhs->equals(m->lhs) && rhs->equals(m->rhs);
    }
}
/**
 *\brief Evaluates the multiplication expression and returns its value.
 *\return The integer result of the multiplication.
 */
int Mult::interp() {
    return lhs->interp() * rhs->interp();
}
/**
 *\brief Determines if the multiplication expression contains any variables.
 *\return True if the expression contains variables, otherwise false.
 */
bool Mult::has_variable() {
    //return true if the lhs-> or rhs-> is a variable
    return lhs->has_variable() || rhs->has_variable();
}
/**
 *\brief Substitutes a variable within the expression with another expression.
 *\param targetVarName The name of the variable to be substituted.
 *\param replacementVar Pointer to the expression that will replace the variable.
 *\return A new expression with the variable substituted.
 */
Expr* Mult::subst(const string& targetVarName, Expr* replacementVar) {
    //recursively look at the left hand and right hand side expression to see
    //if there is the target variable, if the target variable exists
    //on either the lhs or rhs of the multiplication expression, it can be replaced by
    //a new expression -- replacementVar
    return new Mult(lhs->subst(targetVarName, replacementVar), rhs->subst(targetVarName,replacementVar));
}
/**
 *\brief Prints the multiplication expression in a standard format.
 *\param ot The output stream to print to.
 */
void::Mult::print(std::ostream& ot)const {
    ot << "(" << lhs->to_string() << "*" << rhs->to_string() << ")";
}
/**
 *\brief Prints the multiplication expression in a pretty-printed format, using precedence.
 *\param out The output stream to print to.
 */
void Mult::pretty_print(std::ostream &out) const {
    precedence_t lhs_prec = exprToPrecedence(this->lhs);
    precedence_t rhs_prec = exprToPrecedence(this->rhs);
    if (lhs_prec != prec_none) {
        out << "(";
        //recognize that there is an add or mult expression inside of the mult expression on the lhs and evaulate it first enclosing it with ()
        //use AddExpr/multExpr pretty print for the new nums
        this->lhs->pretty_print(out);
        out << ")";
    } else {
        // 1 ... 2
        this->lhs->pretty_print(out);
    }

    out << " * ";
    //if we have an add expression on the right hand side of * then we will print the parenthesis
    //and put the rhs expression between those
    if (rhs_prec == prec_add) {
        out << "(";
        //ensure ()s around lower precedence add --> 1 * ( 2 + 3)
        this->rhs->pretty_print(out);
        out << ")";
    } else {
        // 1 * 2 * 3 --> new Mult( new NumExpr(1), new Mult( new NumExpr(2), new Num Expr(3)); 3
        this->rhs->pretty_print(out);
    }
}
/**
 *\brief Constructs a numerical expression with a given int value.
 *\param val The integer value of the expression.
 */
NumExpr::NumExpr(int val) {
    this->_val = val;
}
/**
 *\brief Checks if this numerical expression equals expression another expression.
 *\param e Pointer to the expression to compare with.
 *\return True if the expressions are equal, otherwise return false.
 */
bool NumExpr::equals(Expr* e){
    NumExpr* num = dynamic_cast<NumExpr*>(e);
    if (num == NULL){
        return false;
    }else{
        return _val == num->_val;
    }

}
/**
 *\brief Evaluates the numerical expression and returns its value.
 *\return The int value of the numerical expression.
 */
int NumExpr::interp() {
    //just return the member variable of the number expression when interpreting the number
    return _val;
}
/**
 *\brief Determines if the numerical expression contains any variables.
 *\return False,  a numerical expression does not contain variables.
*/
bool NumExpr::has_variable() {
    return false;
}
/**
 *\brief Substitutes a variable within the expression with another expression. For NumExpr, it returns a copy of itself since it contains no variables.
 *\param targetVarName The name of the variable to be substituted.
 *\param replacementVar Pointer to the expression that would replace the variable.
 *\return A new NumExpr with the same value as this expression.
*/
Expr* NumExpr::subst(const string &targetmatch, Expr *e) {
    //you cannot substitute a string for a number
    //only a string for a string
    return new NumExpr(this->_val);
}
    //reference & is direct, use actual value
/**
 * \brief Prints the numerical expression to an output stream.
 * \param out The output stream where we will print to.
 */
void NumExpr::print(std::ostream& ot) const {
    // 4 --> '4' output it to the stream ot
    ot << std::to_string(_val);
}

/**
 *\brief Constructs a variable expression with a given variable name.
 *\param val The name of the variable.
*/
VarExpr::VarExpr(string val) {
    _val = val;
}

/**
 *\brief Checks if this variable expression equals another variable expression.
 *\param e Pointer to the expression to compare with
 *\return True if the expression are, otherwise false;
 */
bool VarExpr::equals(Expr* e){
    auto* varexpr = dynamic_cast<VarExpr*>(e);
    if (varexpr == NULL){
        return false;
    }else{
        return _val == varexpr->_val;
    }

}
/**
 *\brief Throws an exception as variable expressions cannot be directly evaluated without a context that assigns values to variables.
 *\throw runtime_error indicating there is no numerical value for the variable
*/
int VarExpr::interp()
{
     throw runtime_error("no value for variable");
}
/**
 * \brief Decides if the variable expression contains any variables.
 * \return True, always
 */
bool VarExpr::has_variable() {
    return true;
}
/**
 *\brief Substitutes a variable within the expression with another expression. If the variable name matches, replaces it with the provided replacement Var expression.
 *\param targetVarName The name of the variable to be substituted.
 *\param replacementVar Pointer to the expression that will replace the variable.
 *\return A new expression with the variable substituted, or "this" variable expression if the names do not match.
*/
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
/**
 *\brief Prints variable to an output stream.
 *\param ot The output stream that we print the variable expression to.
 */
void VarExpr::print(std::ostream& ot) const {
    // already a string just send this 'x' to outstream
    ot << this->_val;
}

