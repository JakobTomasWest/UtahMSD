//
// Created by Jakob West on 1/23/24.
//

#include "TESTS.h"
#include "expr.hpp"
#include "cmdline.hpp"
TEST_CASE("TEST"){
    printf("TEST Ran");
    CHECK( (new VarExpr("x"))->equals(new VarExpr("x")) == true );
    CHECK( (new VarExpr("x"))->equals(new VarExpr("y")) == false );
    CHECK( (new NumExpr(1))->equals(new VarExpr("x")) == false );
    CHECK( (new NumExpr(1))->equals(new NumExpr(1)) == true );
    REQUIRE (NumExpr(-10).equals(new NumExpr(-10)) == true);
    REQUIRE (NumExpr(-19).equals(new VarExpr("z")) == false);
    REQUIRE (NumExpr(-18134).equals(new VarExpr("1")) == false);
    CHECK( (new AddExpr(new NumExpr(2),new NumExpr(3)))->equals(new AddExpr(new NumExpr(2),new NumExpr(3)))==true );
    CHECK( (new AddExpr(new NumExpr(2),new NumExpr(3)))->equals(new AddExpr(new NumExpr(3),new NumExpr(2)))==false );
    CHECK(( new Mult( new NumExpr(2), new NumExpr(2)))->equals(new Mult(new NumExpr(2), new NumExpr(1)))==false);
    CHECK(( new Mult( new NumExpr(2), new NumExpr(2)))->equals(new Mult(new NumExpr(2), new NumExpr(2)))==true);
    REQUIRE ((new Mult(new NumExpr(2),new NumExpr(98)))->equals(new Mult(new NumExpr(2),new NumExpr(98))) == true);

    REQUIRE (VarExpr("-0").equals(new VarExpr("-1")) == false);
    REQUIRE (VarExpr("f").equals(new VarExpr("z")) == false);
    REQUIRE (VarExpr("b").equals(new NumExpr(10)) == false);
    REQUIRE (VarExpr("t").equals(new NumExpr(32)) == false);


}