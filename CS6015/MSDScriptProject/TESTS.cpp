//
// Created by Jakob West on 1/23/24.
//

#include "TESTS.h"
#include "expr.hpp"
#include "cmdline.hpp"
#include "parse.hpp"
#include <sstream>
#include "stdlib.h"
TEST_CASE("TEST"){
printf("TESTS RAN");


    CHECK( (new VarExpr("x"))->equals(new VarExpr("x")) == true );
    CHECK( (new VarExpr("x"))->equals(new VarExpr("y")) == false );
    CHECK( (new VarExpr("!@#$%^&*()"))->equals(new VarExpr("!@#$%^&*()")) == true );
    CHECK( (new NumExpr(1))->equals(new VarExpr("x")) == false );
    CHECK( (new NumExpr(1))->equals(new NumExpr(1)) == true );
    CHECK( (new AddExpr(new NumExpr(2),new NumExpr(3)))->equals(new AddExpr(new NumExpr(2),new NumExpr(3)))==true );
    CHECK( (new AddExpr(new NumExpr(2),new NumExpr(3)))->equals(new AddExpr(new NumExpr(3),new NumExpr(2)))==false );
    Expr* addExpr =(new AddExpr(new NumExpr(1), new NumExpr(2)));
    Expr* numExpr = new NumExpr(3);
    CHECK(addExpr->equals(numExpr) == false);
    Expr* multExpr = new Mult(new NumExpr(1), new NumExpr(2));
    CHECK(multExpr->equals(numExpr) == false);
    CHECK((new Mult(new NumExpr(1),new NumExpr(2)))->to_string() == "(1*2)");
    CHECK(( new Mult( new NumExpr(2), new NumExpr(2)))->equals(new Mult(new NumExpr(2), new NumExpr(1)))==false);
    CHECK(( new Mult( new NumExpr(2), new NumExpr(2)))->equals(new Mult(new NumExpr(2), new NumExpr(2)))==true);
    CHECK((new Mult(new VarExpr("x"),new VarExpr("y")))->has_variable() ==true);
    CHECK( (new NumExpr(22))->subst("y", new Mult(new VarExpr("y"),new NumExpr(7)))->equals(new NumExpr(22)) );

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
    CHECK( (new Mult(new VarExpr("x"), new NumExpr(7)))
                   ->subst("x", new VarExpr("y"))
                   ->equals(new Mult(new VarExpr("y"), new NumExpr(7))) );
    CHECK( (new Mult( new NumExpr(7),new VarExpr("x")))
                   ->subst("x", new VarExpr("y"))
                   ->equals(new Mult( new NumExpr(7),new VarExpr("y"))) );
    CHECK( (new Mult( new NumExpr(7),new VarExpr("x")))
                   ->subst("x", new VarExpr("y"))
                   ->equals(new Mult( new VarExpr("y"),new NumExpr(7))) == false);

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

    AddExpr* e = new AddExpr(new AddExpr(new NumExpr(1), new NumExpr(3)), new NumExpr(2));
    std::stringstream ss;
    e->pretty_print(ss);
    CHECK(ss.str() == "(1 + 3) + 2");

    e = new AddExpr(new NumExpr(2), new AddExpr(new NumExpr(1), new NumExpr(3)));
    ss = std::stringstream();
    e->pretty_print(ss);
    CHECK(ss.str() == "2 + 1 + 3");

    // Expr - abstract type/interface/abstract base class
    // VarExpr - Concrete Type because it has no unimplemented methods
    // Expr e = new VarExpr()
    CHECK((new NumExpr(1))->to_string() == "1");
    CHECK((new VarExpr("x"))->to_string() == "x");
    CHECK((new AddExpr(new NumExpr(1),new NumExpr(2)))->to_string() == "(1+2)");

    CHECK ( ( new Mult( new NumExpr(2),new AddExpr(new NumExpr(1), new NumExpr(3))))
    ->to_pretty_string() == "2 * (1 + 3)");
    CHECK ( ( new AddExpr(new NumExpr(2), new Mult(new NumExpr(1), new NumExpr(3))))
    ->to_pretty_string() == "2 + 1 * 3");

    CHECK ( (new Mult(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3))))
    ->to_pretty_string() ==  "1 * (2 + 3)" );
    CHECK ( (new Mult(new Mult(new NumExpr(8), new NumExpr(1)), new VarExpr("y")))
    ->to_pretty_string() ==  "(8 * 1) * y" );
    CHECK ( (new Mult(new AddExpr(new NumExpr(3), new NumExpr(5)), new Mult(new NumExpr(6), new NumExpr(1))))
    ->to_pretty_string() ==  "(3 + 5) * 6 * 1" );
    CHECK ( (new Mult(new Mult(new NumExpr(7), new NumExpr(7)), new AddExpr(new NumExpr(9), new NumExpr(2))) )
    ->to_pretty_string() ==  "(7 * 7) * (9 + 2)" );
    CHECK ( (new Mult( new Mult(new NumExpr(5), new NumExpr(2)), new NumExpr(8)))
    ->to_pretty_string() == "(5 * 2) * 8");
    CHECK ( (new Mult(new NumExpr(8),new Mult(new NumExpr(5), new NumExpr(2))))
    ->to_pretty_string() == "8 * 5 * 2");

    //---------------------------------------LetExpr Tests-----------------------------------------------------------------------------------------------//
    CHECK ( (new LetExpr("x", new NumExpr(5), new AddExpr(new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x"))))->to_string() == "(_let x = 5 _in ((_let y = 3 _in (y+2))+x))");

    Expr* a = new NumExpr(1);
    Expr* b = new NumExpr(2);
    Expr* c = new NumExpr(2);
    Expr* d = new NumExpr(3);
    LetExpr* abx2 = new LetExpr("x", new AddExpr(a, b), new AddExpr(new VarExpr("x"), new NumExpr(2)));
    cout << abx2->to_string() <<"\n";
    LetExpr* bax2 = new LetExpr("x", new AddExpr(b,a), new AddExpr(new VarExpr("x"), new NumExpr(2)));
    cout << bax2->to_string() <<"\n";
    LetExpr* acx2 = new LetExpr("x", new AddExpr(a, c), new AddExpr(new VarExpr("x"), new NumExpr(2)));
    LetExpr* adx2 = new LetExpr("x", new AddExpr(a, d), new AddExpr(new VarExpr("x"), new NumExpr(2)));
    LetExpr* adx3= new LetExpr("x", new AddExpr(a, d), new AddExpr(new VarExpr("x"), new NumExpr(3)));
    LetExpr* ady2 = new LetExpr("x", new AddExpr(a, d), new AddExpr(new VarExpr("y"), new NumExpr(2)));
    LetExpr* ady3 = new LetExpr("x", new AddExpr(a, d), new AddExpr(new VarExpr("y"), new NumExpr(3)));
    LetExpr* ady3clone = new LetExpr("x", new AddExpr(a, d), new AddExpr(new VarExpr("y"), new NumExpr(3)));
    //bound
    CHECK(abx2->equals(acx2));
    CHECK(!abx2->equals(bax2)); // should return true if we arent using interp?
    CHECK(!abx2->equals(adx2));
    //body
    CHECK(!abx2->equals(adx2));
    CHECK(!abx2->equals(adx2));
    CHECK(!adx2->equals(ady2));
    CHECK(!adx3->equals(ady3));
    //clone
    CHECK(ady3->equals(ady3clone));
    //nest
    LetExpr* nested_abx2 = new LetExpr("y", new NumExpr(4), new LetExpr("x", new AddExpr(a, b), new AddExpr(new VarExpr("x"), new VarExpr("y"))));
    LetExpr* nested_acx2 = new LetExpr("y", new NumExpr(4), new LetExpr("x", new AddExpr(a, c), new AddExpr(new VarExpr("x"), new VarExpr("y"))));
    CHECK(nested_abx2->equals(nested_acx2));

    //Interp
    CHECK(abx2->interp()==5 );

    //Substitute
    //Dont sub x because lhs = valToSub
    CHECK( (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(5))))
    ->subst("x", new NumExpr(4))
                   ->equals( new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(5)))));
    //Don't sub x because valToSub is not contained
    CHECK( (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(5))))
    ->subst("y", new NumExpr(4))
                   ->equals( new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(5)))));

    CHECK((new LetExpr("y", new NumExpr(1),
                       new LetExpr("z", new AddExpr(new VarExpr("x"), new NumExpr(1)),
                                   new AddExpr(new VarExpr("z"), new NumExpr(1))))
                                   // _let y=1 _in ( _let z=x + 1 _in (z+1))
          )->subst("x", new NumExpr(2))
                                    //x =2
                  ->equals(new LetExpr("y", new NumExpr(1),
                                       new LetExpr("z", new AddExpr(new NumExpr(2), new NumExpr(1)),
                                                   new AddExpr(new VarExpr("z"), new NumExpr(1)))
                  )));
                            //_let y=1 _in (_let z=2+1 _ in (z+1)

    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("y"), new NumExpr(2)))
            //_let x=5 _in y + 2
          )->subst("y", new NumExpr(3))
                    // y =3
                  ->equals(new LetExpr("x", new NumExpr(5), new AddExpr(new NumExpr(3), new NumExpr(2)))
                  ));
    //_let x=5 _in 3 + 2

    CHECK((new LetExpr("x", new AddExpr(new VarExpr("x"),new NumExpr(1)), new AddExpr(new VarExpr("x"), new NumExpr(30))))
        ->subst("x", new NumExpr(300))
                ->equals((new LetExpr("x", new AddExpr(new NumExpr(300),new NumExpr(1)), new AddExpr(new VarExpr("x"), new NumExpr(30))))));

    //Sub if lhs != valToSub when valToSub is contained in rhs
    //Sub if lhs != valToSub when valToSub is contained in body
    delete abx2;
    delete bax2;
    delete acx2;
    delete adx2;
    delete adx3;
    delete ady2;
    delete ady3;
    delete nested_abx2;
    delete nested_acx2;
    delete a;
    delete b;
    delete c;
    delete d;

    CHECK((new Mult(new Mult(new NumExpr (2), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x") , new NumExpr(1)) )), new NumExpr(3) ))
    ->to_pretty_string() == "(2 * _let x = 5\n"
                           "      _in  x + 1) * 3");
//    //Let nested to the left in add expression which is nested to the right within a multiplication expression
//    CHECK((new Mult(new NumExpr(5), new AddExpr(new LetExpr("x", new NumExpr(5), new VarExpr("x")), new NumExpr(1))))->to_pretty_string() == "5 * ((_let x = 5\n"
//                                                                                                                         "      _in x) + 1)");
    CHECK((new Mult(new NumExpr(5), new AddExpr(new LetExpr("x", new NumExpr(5), new VarExpr("x")), new NumExpr(1))))->to_pretty_string() == "5 * ((_let x = 5\n"
                                                                                                                         "       _in  x) + 1)");
    //    //Let in lhs of add
    CHECK ( (new AddExpr(new LetExpr("x", new NumExpr(2), new AddExpr(new VarExpr("x"), new NumExpr(9))), new NumExpr(4)))->to_pretty_string() == "(_let x = 2\n"
                                                                                                                          "  _in  x + 9) + 4");
    //Let in lhs of multiplication expression
    CHECK((new Mult(new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(8))), new NumExpr(3)))->to_pretty_string() == "(_let x = 5\n"
                                                                                                                         "  _in  x + 8) * 3");
    //Let nest as right argument of un-parenthesized multiplication expression
    CHECK((new AddExpr (new Mult(new NumExpr(4), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))), new NumExpr(9)))->to_pretty_string() == "4 * (_let x = 5\n"
                                                                                                                                               "      _in  x + 1) + 9");
    CHECK((new Mult(new NumExpr(4), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))->to_pretty_string() == "4 * (_let x = 5\n"
                                                                                                                                                                           "      _in  x + 1)");
    CHECK((new Mult(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))->interp() == 30);


}
TEST_CASE("parse") {
    CHECK_THROWS_WITH( parseString("()"), "bad input" );

    CHECK( parseString("(1)")->equals(new NumExpr(1)) );
    CHECK( parseString("(((1)))")->equals(new NumExpr(1)) );

    CHECK_THROWS_WITH( parseString("(1"), "missing closing parenthesis" );

    CHECK( parseString("1")->equals(new NumExpr(1)) );
    CHECK( parseString("10")->equals(new NumExpr(10)) );
    CHECK( parseString("-3")->equals(new NumExpr(-3)) );
    CHECK( parseString("  \n 5  ")->equals(new NumExpr(5)) );
    CHECK_THROWS_WITH( parseString("-"), "no following digit after the negative sign, invalid input" );

    // This was some temporary debugging code:
    //  std::istringstream in("-");
    //  parse_num(in)->print(std::cout); std::cout << "\n";

    CHECK_THROWS_WITH( parseString(" -   5  "), "no following digit after the negative sign, invalid input" );

    CHECK( parseString("x")->equals(new VarExpr("x")) );
    CHECK( parseString("xyz")->equals(new VarExpr("xyz")) );
    CHECK( parseString("xYz")->equals(new VarExpr("xYz")) );
    CHECK_THROWS_WITH( parseString("x_z"), "invalid input: variable names cannot contain underscores" );

    CHECK( parseString("x + y")->equals(new AddExpr(new VarExpr("x"), new VarExpr("y"))) );

    CHECK( parseString("x * y")->equals(new Mult(new VarExpr("x"), new VarExpr("y"))) );

    CHECK( parseString("z * x + y")
                   ->equals(new AddExpr(new Mult(new VarExpr("z"), new VarExpr("x")),
                                    new VarExpr("y"))) );

    CHECK( parseString("z * (x + y)")
                   ->equals(new Mult(new VarExpr("z"),
                                     new AddExpr(new VarExpr("x"), new VarExpr("y"))) ));


    CHECK(parseString("_let x = 5\n_in  (_let y = 3\n      _in  y + 2) + x")
                   ->equals(new LetExpr("x", new NumExpr(5), new AddExpr(new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x")) )));

    CHECK( parseString("_let x = 5 _in x + 5")->equals(new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(5)))));
    CHECK( parseString("_let x = 5 _in ((_let x = 5 _in x + 1) + x)")->equals(new LetExpr("x", new NumExpr(5), new AddExpr(new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))), new VarExpr("x")))));
    CHECK( parseString("_let x = 5 _in ((_let x = 5 _in x * 1) * x)")->equals(new LetExpr("x", new NumExpr(5), new Mult(new LetExpr("x", new NumExpr(5), new Mult(new VarExpr("x"), new NumExpr(1))), new VarExpr("x")))));
    CHECK( parseString("x * ((_let x = 5 _in x * 1) * x)")->equals(new Mult(new VarExpr("x"), new Mult(new LetExpr("x", new NumExpr(5), new Mult(new VarExpr("x"), new NumExpr(1))), new VarExpr("x")))));

}
