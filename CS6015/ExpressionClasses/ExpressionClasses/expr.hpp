//
//  expr.hpp
//  ExpressionClasses
//
//  Created by Jakob West on 1/19/24.
//

#ifndef expr_hpp
#define expr_hpp

#include <stdio.h>
#include <string>

using namespace std;
class Expr {
    public:
        virtual bool equals(Expr *e) = 0;
};

#endif /* expr_hpp */
