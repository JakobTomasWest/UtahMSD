//
// Created by Jakob West on 1/23/24.
//
#include "expr.hpp"
#include "Val.h"
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
    } else if (dynamic_cast<MultExpr*>(e) != nullptr){
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
void Expr::pretty_print(std::ostream& ostream) const{
    std::streampos pos;
    pretty_printHelper(ostream,prec_none, false,0);
}
void Expr::pretty_printHelper(std::ostream &ostream, precedence_t precedence, bool needsParen, std::streampos spaces) const{
    print(ostream);
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
 * \brief Check if this addition expression equals another expression.
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
Val* AddExpr::interp() {
    //return an int for the value of the expression
    // the value of addition expression is the sum of teh two subexpressions
    return rhs->interp()->add_to( lhs->interp());
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
 * \brief Initiates the pretty-printing process for an AddExpr object.
 *
 * This method handles the initial call to pretty_printHelper with specific
 * settings for indentation level and the need for parentheses, determined
 * by the context of the expression.
 *
 * \param ostream The output stream where the expression will be pretty-printed.
 */
void AddExpr::pretty_print(std::ostream &ostream) const{
    std::streampos pos = ostream.tellp();
//    pretty_printHelper(ostream, prec_none, false,pos);
    pretty_printHelper(ostream, prec_add, false,pos);
}
/**
 * \brief Prints the addition expression in a pretty-printed format, using operation precedence.
 * This helper function recursively pretty-prints left and right subexpressions,
 * applying parentheses around subexpressions where necessary, based on the
 * provided precedence level.
 * \param ostream The output stream to print to.
 * \param precedence The current expressions precedence
 * \param needsParen  boolean flag indicating whether the current expression should
 * be enclosed in parentheses
 * \param spaces Indicates the current indentation level or position within the output, for aligning the expression properly
 * stream
 */
void AddExpr::pretty_printHelper(std::ostream &ostream, precedence_t precedence, bool needsParen, std::streampos spaces) const {
    // If the precedence we pass in is greater than add precedence we'll use needParen
    if (precedence > prec_add ) {
        ostream << "(";
    }
    //lhs will ensure parenthesis if nested in a prec_add (+1) / greater precedence
    this->lhs->pretty_printHelper(ostream, static_cast<precedence_t>(prec_add + 1),needsParen,spaces);
    ostream << " + ";
    this->rhs->pretty_printHelper(ostream, prec_add,needsParen,spaces);

    if (precedence > prec_add ) {
        ostream << ")";
    }

}

/**
 \brief Constructs a multiplication expression with left side and right wide multiplication operands.
 \param lhs Pointer to the left-hand side expression.
 \param rhs Pointer to the right-hand side expression.
 */
MultExpr::MultExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
/**
 *\brief Checks if this multiplication expression equals another expression.
 *\param e Pointer to the expression to compare with.
 *\return True if the expressions are equal, otherwise false.
 */
bool MultExpr::equals(Expr *e){
    MultExpr *m = dynamic_cast<MultExpr*>(e);
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
Val* MultExpr::interp() {
    return rhs->interp()->mult_with(lhs->interp());
}
/**
 *\brief Determines if the multiplication expression contains any variables.
 *\return True if the expression contains variables, otherwise false.
 */
bool MultExpr::has_variable() {
    //return true if the lhs-> or rhs-> is a variable
    return lhs->has_variable() || rhs->has_variable();
}
/**
 *\brief Substitutes a variable within the expression with another expression.
 *\param targetVarName The name of the variable to be substituted.
 *\param replacementVar Pointer to the expression that will replace the variable.
 *\return A new expression with the variable substituted.
 */
Expr* MultExpr::subst(const string& targetVarName, Expr* replacementVar) {
    //recursively look at the left hand and right hand side expression to see
    //if there is the target variable, if the target variable exists
    //on either the lhs or rhs of the multiplication expression, it can be replaced by
    //a new expression -- replacementVar
    return new MultExpr(lhs->subst(targetVarName, replacementVar), rhs->subst(targetVarName, replacementVar));
}
/**
 *\brief Prints the multiplication expression in a standard format.
 *\param ot The output stream to print to.
 */
void::MultExpr::print(std::ostream& ot)const {
    ot << "(" << lhs->to_string() << "*" << rhs->to_string() << ")";
}
/**
 * \brief Initiates the pretty-printing process for an MultExpr object.
 *
 * This method handles the initial call to pretty_printHelper with specific
 * settings for indentation level and the need for parentheses, determined
 * by the context of the expression.
 *
 * \param ostream The output stream where the expression will be pretty-printed.
 */
void MultExpr::pretty_print(std::ostream &ostream) const {
    std::streampos pos = ostream.tellp();
//    pretty_printHelper(ostream, prec_none, false,0);
    pretty_printHelper(ostream, prec_mult, false,pos);

}
/**
 * \brief Prints the multiplication expression in a pretty-printed format, using operation precedence.
 * This helper function recursively pretty-prints left and right subexpressions,
 * applying parentheses around subexpressions where necessary, based on the
 * provided precedence level.
 * \param ostream The output stream to print to.
 * \param precedence The current expressions precedence
 * \param needsParen  boolean flag indicating whether the current expression should
 * be enclosed in parentheses
 * \param spaces Indicates the current indentation level or position within the output, for aligning the expression properly
 * stream
 */
void MultExpr::pretty_printHelper(std::ostream &ostream, precedence_t precedence, bool needsParen, std::streampos spaces) const {

    //see if this multiplication method is nested within an operation of higher precedence
    if (precedence > prec_mult ) {
        needsParen = true;
        ostream << "(";
    }
    //ensure that if lhs is part of a nested multiplication or add expression that the is gets parenthesis
    this->lhs->pretty_printHelper(ostream, static_cast<precedence_t>(prec_mult + 1),needsParen,spaces);
    ostream << " * ";
    this->rhs->pretty_printHelper(ostream, prec_mult,needsParen,spaces);
    //close out if we opened paren
    if (precedence > prec_mult ) {
        ostream << ")";
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
Val* NumExpr::interp() {
    //just return the new numVal when interpreting the number
    return new NumVal(_val);
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
Expr* NumExpr::subst(const string& targetmatch, Expr *e) {
    //you cannot substitute a string for a number
    //only a string for a string
    return new NumExpr(_val);
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
Val* VarExpr::interp()
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
        //return Expr that holds Val->'y'
        return replacementVar;
    }else{

        //otherwise create a new variable expression object and give it the same original value
        //i.e., if 'x' is our og, and our targetvar isn't 'x' we can't replace the variable,
        //so we just create our new expression variable with this-> _val (string)
        // we already have stored
        return new VarExpr(_val);

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



LetExpr::LetExpr(const string& var, Expr *boundExpr, Expr *bodyExpr) {
    varName = var;
    _boundExpr = boundExpr;
    _bodyExpr = bodyExpr;
}

/**
 * \brief Evaluating whether this letExpr is equivalent to the otherLetExpression
 * \param otherLetExpression The LetExpression we want to compare with
 *
 */
bool LetExpr::equals(Expr *otherExpression) {
    //like other classes -- attempt to have base Expr pointer, point to derived instance of another
    // expression class -> LetExpr

    // dynamic_cast successfully converts the pointer
    // and returns the address of the object as type LetExpr*.
    LetExpr* otherLet = dynamic_cast<LetExpr*>(otherExpression);
    //or if dynamic cast fails, otherExpression cannot point/ is not a letExpr
    if(otherLet == nullptr){
        return false;
    }

    //Compare each member variable of this LetExpr to the otherLet Expr
    if(this->varName != otherLet->varName){
        return false;
    }
    //Check if boundExpr is the same exact as otherlets boundExpr or recursively evaluates
    // to the same expression components
    // i.e., (_let x = (a+b) _in (x)) equal (_let x = (a+c) _in (x)) !equal (_let x = (a+d) _in (x))
    // when a = new NumExpr(1), b = new NumExpr(2), c = New NumExpr(2), d = new NumExpr(3)
    if(!this->_boundExpr->equals(otherLet->_boundExpr)){
        return false;
    }
    if(!this->_bodyExpr->equals(otherLet->_bodyExpr)){
        return false;
    }
    return true;
}
/**
 * \brief Bind the boundExpr to the varName and substitutes the boundExpr into said VarName if the body
 * expression holds this variable
 *
 * @return the evaluated expression as an int
 */
Val* LetExpr::interp() {
    //let x=boundExpr, bodyExpr

//    int boundExprValue = this->_boundExpr->interp();
    //send boundExprValue into body for varName
//    Expr* substitutedBody = _bodyExpr->subst(varName,new NumExpr(boundExprValue));
    Expr* interpretedExpression = _bodyExpr->subst(varName,_boundExpr);
//    int interpretedExpression = substitutedBody->interp();


    return interpretedExpression->interp();
}
/**
 * \brief Determines whether the bodyExpression has a variable
 * @return true if the body has a variable
 */
bool LetExpr::has_variable() {
//    Expr* lhs = this->varName;
    Expr* rhs = this->_bodyExpr;
    if(rhs->has_variable() ){
        return true;
    }
    else
    {
        return false;
    }
}
/**
 * \brief Substitutes a given variable name within the LetExpr with a new expression.
 *
 * This method checks if the variable name matches the variable name within the `LetExpr`.
 * If it matches, it replaces the bound expression with the new expression. Otherwise,
 * it recursively substitutes within the bound and body expressions.
 *
 * \param givenVarName The name of the variable to be substituted.
 * \param newExpression The new expression to replace the variable with.
 * \return A new LetExpr instance with the variable substituted.
 */
Expr *LetExpr::subst(const string &givenVarName, Expr *newExpression) {
    //See if the variable is even correct for substitution for the boundExpr
    if(varName == givenVarName){
        //Now i want to replace my varNames corresponding boundExpression
        //with the new expression
//        _boundExpr= newExpression;
        return new LetExpr(varName,_boundExpr->subst(givenVarName,newExpression),_bodyExpr);
    }  else{
        //otherwise the varName is different so we will just create the same object and if body can substitute the varName it will
        return new LetExpr(varName,_boundExpr->subst(givenVarName,newExpression), _bodyExpr->subst(givenVarName,newExpression));
    }
}
//(_let x=5 _in ((_let y=3 _in (y+2))+x)

/**
 * \brief Prints the LetExpr to the output stream.
 *
 * This method formats the `LetExpr` as a string and outputs it to the given stream,
 * showing the structure of the let expression.
 *
 * \param out The output stream we will print the expression to.
 */
void LetExpr::print(ostream &out) const {
    out << "(_let " <<varName << " = " << _boundExpr->to_string() << " _in " << _bodyExpr->to_string() <<")";
}
/**
 * \brief Initiates the pretty-printing process for a LetExpr object.
 *
 * This method creates initial settings for the pretty-printing process, such as
 * indentation level and (no)need for parentheses, and delegates to pretty_printHelper
 * for the actual printing logic.
 *
 * \param ostream The output stream where the expression will be pretty-printed to.
 */
void LetExpr::pretty_print(std::ostream &ostream) const {
    std::streampos pos =  ostream.tellp();
    pretty_printHelper(ostream,prec_none, false,pos);
}
/**
 * \brief Pretty-prints the LetExpr expression, considering indentation and the need for parentheses.
 *
 * This helper function formats the LetExpr to show the variable binding and body expression
 * with proper indentation and parentheses, ensuring the necessary nested structure.
 *
 * @param ostream The output stream to print to.
 * @param precedence The operation precedence gives the LetExpr the precedence level that it is a part of.
 * @param needsParen Decides whether the expression needs to be in parentheses.
 * @param spaces The current indentation level, used for aligning nested expressions.
 */
void LetExpr::pretty_printHelper(std::ostream &ostream, precedence_t precedence, bool needsParen, std::streampos spaces) const {

    //always give letExpr ()
    if (!needsParen && precedence != prec_none) {
        ostream << "(";
    }

    std::streampos letPosition = ostream.tellp();
    std::streampos depth = letPosition - spaces;
    ostream<< "_let " << this->varName<<" = ";
    //print bound expression with passing difference
    this->_boundExpr->pretty_printHelper(ostream,prec_none, needsParen, depth);
    ostream<< "\n ";
    std::streampos nextPos = ostream.tellp();
    //start print with indentation
    for ( int i = 0; i < letPosition - spaces; i++ ) {
        ostream<< " ";
    }
    ostream<< "_in  ";
    this->_bodyExpr->pretty_printHelper(ostream, prec_none, needsParen, nextPos);
    if (!needsParen && precedence != prec_none) {
        ostream << ")";
    }

}
/**
 * \brief Constructor for the BooleanExpr class
 * This method Initializes a new BooleanExpr object with the boolean value provided in the val parameter
 * @param val is the true and false val
 */
BooleanExpr::BooleanExpr(bool val) {
    _boolean = val;
}
/** \brief Determines if the current BooleanExpr object is equivalent to another expression object pointed to by expr.
 *
 * @param e
 * @return if both are boolean objects it returns the same boolean value, otherwise false
 */

bool BooleanExpr::equals(Expr *e) {
    auto* val = dynamic_cast<BooleanExpr*>(e);
    if (val== nullptr){
        throw std::runtime_error("not a boolean");
    }
    return _boolean==val->_boolean;
}

/**
 * \brief Interprets the current BooleanExpr as a Boolean value
 * @return returns a pointer to a new BoolVal object that represents the boolean value of the expression
 */
Val* BooleanExpr::interp() {
    return new BoolVal(_boolean);
}
/**
 *  \brief boolean expressions do not return variables
 * @return false
 */
bool BooleanExpr::has_variable() {
    return false;
}
/**
 * \brief Cannot produce any substitution because BooleanExprs do not contain variables
 * @param givenVarName
 * @param e
 * @return null pointer
 */
 Expr* BooleanExpr::subst(const string& givenVarName, Expr* e){
    return nullptr;
}
/**
 * \brief Prints to the ostream the boolean expression values
 * @param ostream The output stream where the expression will be pritned too.
 */
void BooleanExpr::print(std::ostream &ostream) const {
    if (_boolean){
        ostream<<"_true";
    } else{
        ostream<<"_false";
    }
}

/**
 * \brief Constructs the If then else expression
 * @param cond if condition
 * @param thenTrue if condition is true then provide result
 * @param elseFalse else condition is true if condition is false providing secondary result
 */
IfExpr::IfExpr(Expr* cond, Expr *thenTrue, Expr *elseFalse) {
    _condition = cond;
    _true=thenTrue;
    _false=elseFalse;
}
/**
 * \brief IfExpr equals checks whether the provided expression is an instance of IfExpr and affirms that the variables are equivalent
 * This method compares the current IfExpr object with another object that's being pointed to.
 * @param expr is the object we are comparing this expression to
 * @return If the provided expression is an IfExpr, the method returns true if all the components are the same, otherwise it returns false
 */
bool IfExpr::equals(Expr* e) {
    auto* ifExpr = dynamic_cast<IfExpr*>(e);
    if (ifExpr== nullptr){
        throw std::runtime_error("not an if expression");
    }
    return _condition->equals(ifExpr->_condition)&&_true->equals(ifExpr->_true)&&_false->equals(ifExpr->_false);
}
/**
 * \brief Interprets whether teh condition is true by calling the is_true function on teh Val object
 * @return If the condition is true we interpret the then condition . If the condition was false we interpret the false condition branch
 */
Val *IfExpr::interp() {
    if (_condition->interp()->is_true()){
        return _true->interp();
    } else{
        return _false->interp();
    }
}
/**
 * \brief No part of the if expression is a variable
 * @return false
 */
bool IfExpr::has_variable() {
    return false;
}
/**
 * \brief If the condition is true substitute given a varible substitute expressions
 * @param givenVarName variable qualifier
 * @param e expression to substitute
 * @return return substituted expression
 */
Expr *IfExpr::subst(const string& givenVarName, Expr* e) {
    if (_condition->interp()->is_true()){
        return _true->subst(givenVarName,e);
    } else{
        return _false->subst(givenVarName,e);
    }
}
/**
 * \brief Prints IfExpression object to the output stream
 * @param ostream the output stream we are using to print the conditional expression
 */
void IfExpr::print(std::ostream &out) const{
    out<<"(_if ";
    _condition->print(out);
    out<<"_then ";
    _true->print(out);
    out<<"_else ";
    _false->print(out);
    out<<")";
}
/**
 * \brief Pretty print IfExpression object to output stream
 * @param out the ostream to pretty-print to
 */
void IfExpr::pretty_print(std::ostream& out) const {
    pretty_printHelper(out,prec_none, false,0);
}
/**
 * \brief actually handles the spacing of printing the IfExpression in the pretty format
 * @param out stream we are using to print
 * @param precedence to categorize how we will have to print different IfExprs
 * @param needsParen statement for parenthesis formatting
 * @param spaces number of spaces
 */
void IfExpr::pretty_printHelper(std::ostream &out, precedence_t precedence, bool needsParen, std::streampos spaces) const {
    if (!needsParen and precedence!= prec_none){
        out << "(";
        std::streampos sPosition  = out.tellp();
        std::streampos numSpaces = sPosition - spaces;
        out << "_if ";
        _condition->pretty_printHelper(out, prec_none, true, spaces);
        //separate condition from then portion
        out << "\n";
        std::streampos spacesBeforeNext = out.tellp();
        for (int i = 0; i < numSpaces; i++) {
            out << " ";
        }
        out << "_then ";
        _true->pretty_printHelper(out, prec_none, true, spacesBeforeNext);
        out << "\n";
        for (int i = 0; i < numSpaces; i++) {
            out << " ";
        }
        out << "_else ";
        _false->pretty_printHelper(out, prec_none, true, spacesBeforeNext);
        out<<")";
    } else {
        std::streampos sPosition  = out.tellp();
        std::streampos numSpaces = sPosition - spaces;
        out << "_if ";
        _condition->pretty_printHelper(out, prec_none, false, spaces);
        out << "\n";
        std::streampos spacesBeforeNext = out.tellp();
        for (int i = 0; i < numSpaces; i++) {
            out << " ";
        }
        out << "_then ";
        _true->pretty_printHelper(out, prec_none, false, spacesBeforeNext);
        out << "\n";
        for (int i = 0; i < numSpaces; i++) {
            out << " ";
        }
        out << "_else ";
        _false->pretty_printHelper(out, prec_none, false, spacesBeforeNext);
    }
}

/**
 * \brief Constructor for the Equals method
 * @param lhs expression on the left side of equals expression
 * @param rhs expression on the right side of equals expression
 */
EqualsExpr::EqualsExpr(Expr *lhs, Expr *rhs) {
    _lhs = lhs;
    _rhs = rhs;
}
/**
 * \brief Attempts to cast expression e to the EqualsExpression type
 * The equals() method of the resulting Val objects is then called to determine if the evaluated results are equivalent.
 * @param e expression for casting
 * @return true if results are equal, false otherwise
 */
bool EqualsExpr::equals(Expr *e) {
    auto equalsEx = dynamic_cast<EqualsExpr*>(e);
    if (equalsEx == nullptr){
        throw std::runtime_error("not a equal expression");
    }
    //Compare the interpreted values of the current EqualsExpr and the equalsEx
    return interp()->equals(equalsEx->interp());
}

/**
 *\brief Take left and ride expression objects and interpret them to Val objects, if they are equal construct a true bool val otherwise construct a false bool Val
 * @return new BoolVal object
 */
Val* EqualsExpr::interp() {
    return new BoolVal(_lhs->interp()->equals(_rhs->interp()));
}

bool EqualsExpr::has_variable() {
    return false;
}

Expr* EqualsExpr::subst(const string& givenVarName, Expr* e) {
    return nullptr;
}
/**
 * \brief Prints the EqualsExpression using the ostream
 * @param out
 */
void EqualsExpr::print(std::ostream& out) const{
    out<<"(";
    _lhs->print(out);
    out<<" == ";
    _rhs->print(out);
    out<<")";
}

void EqualsExpr::pretty_print(std::ostream& out) const {
    pretty_printHelper(out,prec_none, false,0);
}
/**
 * \brief Pretty prints the Equals Expression to the ostream
 * @param out ostream we are using
 * @param precedence for deciding how we need to print the expression
 * @param needsParen if prec is none we don't need another set of parenthesis
 * @param spaces
 */
void EqualsExpr::pretty_printHelper(std::ostream &out, precedence_t precedence, bool needsParen, std::streampos spaces) const {
    if (!needsParen and precedence!= prec_none){
        out << "(";
        _lhs->pretty_printHelper(out, prec_mult, true, spaces);
        out << " == ";
        _rhs->pretty_printHelper(out, prec_mult, true, spaces);
        out<<")";
    } else {
        _lhs->pretty_printHelper(out, prec_mult, false, spaces);
        out << " == ";
        _rhs->pretty_printHelper(out, prec_mult, false, spaces);
    }
}





