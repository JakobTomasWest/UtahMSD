//
// Created by Jakob West on 1/23/24.
//

#include "TESTS.h"
#include "expr.hpp"
#include "cmdline.hpp"
TEST_CASE("TEST"){
printf("TESTS RAN");
    CHECK( (new VarExpr("x"))->equals(new VarExpr("x")) == true );
    CHECK( (new VarExpr("x"))->equals(new VarExpr("y")) == false );
    CHECK( (new VarExpr("!@#$%^&*()"))->equals(new VarExpr("!@#$%^&*()")) == true );
    CHECK( (new NumExpr(1))->equals(new VarExpr("x")) == false );
    CHECK( (new NumExpr(1))->equals(new NumExpr(1)) == true );
    CHECK( (new AddExpr(new NumExpr(2),new NumExpr(3)))->equals(new AddExpr(new NumExpr(2),new NumExpr(3)))==true );
    CHECK( (new AddExpr(new NumExpr(2),new NumExpr(3)))->equals(new AddExpr(new NumExpr(3),new NumExpr(2)))==false );
    CHECK(( new Mult( new NumExpr(2), new NumExpr(2)))->equals(new Mult(new NumExpr(2), new NumExpr(1)))==false);
    CHECK(( new Mult( new NumExpr(2), new NumExpr(2)))->equals(new Mult(new NumExpr(2), new NumExpr(2)))==true);
    int result1 = (new Mult(new NumExpr(2), new NumExpr(2)))->interp();
    int result2 = (new AddExpr(new NumExpr(2), new NumExpr(2)))->interp();
    CHECK(result1 == result2);

    CHECK( (new Mult(new NumExpr(3), new NumExpr(2)))
                   ->interp()==6 );
     CHECK( (new Mult(new NumExpr(-3), new NumExpr(-2)))
                   ->interp()==6 );
    CHECK( (new AddExpr(new AddExpr(new NumExpr(10), new NumExpr(15)),new AddExpr(new NumExpr(20),new NumExpr(20))))
                   ->interp()==65);
    CHECK( (new AddExpr(new AddExpr(new NumExpr(-10), new NumExpr(15)),new AddExpr(new NumExpr(20),new NumExpr(20))))
                   ->interp()==45);               

    CHECK_THROWS_WITH( (new VarExpr("x"))->interp(), "no value for variable" );

    CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))
                   ->subst("x", new VarExpr("y"))
                   ->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))) );
    CHECK( (new AddExpr( new NumExpr(7),new VarExpr("x")))
                   ->subst("x", new VarExpr("y"))
                   ->equals(new AddExpr( new NumExpr(7),new VarExpr("y"))) );
    CHECK( (new AddExpr( new NumExpr(7),new VarExpr("x")))
                   ->subst("x", new VarExpr("y"))
                   ->equals(new AddExpr( new VarExpr("y"),new NumExpr(7))) == false);

    CHECK( (new VarExpr("x"))
                   ->subst("x", new AddExpr(new VarExpr("y"),new NumExpr(7)))
                   ->equals(new AddExpr(new VarExpr("y"),new NumExpr(7))) );

    CHECK( (new VarExpr("x"))
                   ->subst("x", new Mult(new VarExpr("y"),new NumExpr(7)))
                   ->equals(new Mult(new VarExpr("y"),new NumExpr(7))) );
    CHECK( (new VarExpr("x"))
                   ->subst("7", new Mult(new VarExpr("y"),new NumExpr(7)))
                   ->equals(new Mult(new VarExpr("y"),new NumExpr(7))) ==false );
    CHECK( (new VarExpr("x"))
                   ->subst("7", new Mult(new VarExpr("y"),new NumExpr(7)))
                   ->equals(new VarExpr("x")) );

    CHECK((new NumExpr(1))->has_variable() == false);
    CHECK((new AddExpr(new NumExpr(2), new NumExpr(3)))->has_variable() == false);
    CHECK((new AddExpr(new VarExpr("x"), new NumExpr(3)))->has_variable() == true);
    CHECK( (new NumExpr(-1))->equals(new NumExpr(-1)) == true );
    CHECK( (new NumExpr(-1))->equals(new NumExpr(0)) == false );

}