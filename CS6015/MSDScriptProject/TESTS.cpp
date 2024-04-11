//
// Created by Jakob West on 1/23/24.
//

#include "TESTS.h"
#include "expr.hpp"
#include "cmdline.hpp"
#include "parse.hpp"
#include "Val.h"
#include "env.h"
#include <sstream>
#include <cstdlib>
TEST_CASE("TEST"){
printf("TESTS RAN");
    CHECK( (NEW(VarExpr)("x"))->equals(NEW(VarExpr)("x")) == true );
    CHECK( (NEW(VarExpr)("x"))->equals(NEW(VarExpr)("y")) == false );
    CHECK( (NEW(VarExpr)("!@#$%^&*()"))->equals(NEW(VarExpr)("!@#$%^&*()")) == true );
    CHECK( (NEW(NumExpr)(1))->equals(NEW(VarExpr)("x")) == false );
    CHECK( (NEW(NumExpr)(1))->equals(NEW (NumExpr)(1)) == true );
    CHECK( (NEW(AddExpr)(NEW (NumExpr)(2),NEW (NumExpr)(3)))->equals(NEW(AddExpr)(NEW (NumExpr)(2),NEW (NumExpr)(3)))==true );
    CHECK( (NEW (AddExpr)(NEW (NumExpr)(2),NEW (NumExpr)(3)))->equals(NEW (AddExpr)(NEW (NumExpr)(3),NEW (NumExpr)(2)))==false );
    PTR(AddExpr)addExpr =(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)));

    CHECK(addExpr->equals(NEW(NumExpr)(3)) == false);
    PTR(MultExpr) multExpr = NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));
    CHECK(multExpr->equals(NEW(NumExpr)(3)) == false);
    CHECK((NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))->to_string() == "(1*2)");
    CHECK(( NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2)))->equals(NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(1))) == false);
    CHECK(( NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2)))->equals(NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2))) == true);
    CHECK((NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")))->has_variable() == true);
//    PTR(Env) env = NEW(ExtendedEnv)("y", NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7)), Env::empty);
    PTR(Env) env = Env::empty;
    CHECK((NEW(NumExpr)(22))->interp(env)->equals(NEW(NumVal)(22)));


    PTR(Val)result1 = (NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2)))->interp(Env::empty);
    PTR(Val)result2 = (NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2)))->interp(Env::empty);
    CHECK(CAST(NumVal)(result1)->equals(CAST(NumVal)(result2)));

    PTR(Val)result3 = (NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2)))->interp(Env::empty);
    CHECK(CAST(NumVal)(result3)->equals(NEW(NumVal)(6)));

    PTR(Val)result4 = (NEW(MultExpr)(NEW(NumExpr)(-3), NEW(NumExpr)(-2)))->interp(Env::empty);
    CHECK(CAST(NumVal)(result4)->equals(NEW(NumVal)(6)));

    PTR(Val)result5 = (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(15)),NEW(AddExpr)(NEW(NumExpr)(20),NEW(NumExpr)(20))))->interp(Env::empty);
    CHECK(CAST(NumVal)(result5)->equals(NEW(NumVal)(65)));

    PTR(Val)result6 = (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(-10), NEW(NumExpr)(15)),NEW(AddExpr)(NEW(NumExpr)(20),NEW(NumExpr)(20))))->interp(Env::empty);
    CHECK(CAST(NumVal)(result6)->equals(NEW(NumVal)(45)));


    CHECK( (NEW(AddExpr)( NEW(NumExpr)(7),NEW(VarExpr)("x")))
                   ->subst("x", NEW(VarExpr)("y"))
                   ->equals(NEW(AddExpr)( NEW(NumExpr)(7),NEW(VarExpr)("y"))) );
    CHECK( (NEW(AddExpr)( NEW(NumExpr)(7),NEW(VarExpr)("x")))
                   ->subst("x", NEW(VarExpr)("y"))
                   ->equals(NEW(AddExpr)( NEW(VarExpr)("y"),NEW(NumExpr)(7))) == false);
    CHECK( (NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))
                   ->subst("x", NEW(VarExpr)("y"))
                   ->equals(NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))) );
    CHECK( (NEW(MultExpr)(NEW(NumExpr)(7), NEW(VarExpr)("x")))
                   ->subst("x", NEW(VarExpr)("y"))
                   ->equals(NEW(MultExpr)(NEW(NumExpr)(7), NEW(VarExpr)("y"))) );
    CHECK((NEW(MultExpr)(NEW(NumExpr)(7), NEW(VarExpr)("x")))
                   ->subst("x", NEW(VarExpr)("y"))
                   ->equals(NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))) == false);

    CHECK( (NEW(VarExpr)("x"))
                   ->subst("x", NEW(AddExpr)(NEW(VarExpr)("y"),NEW(NumExpr)(7)))
                   ->equals(NEW(AddExpr)(NEW(VarExpr)("y"),NEW(NumExpr)(7))) );

    CHECK( (NEW(VarExpr)("x"))
                   ->subst("x", NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7)))
                   ->equals(NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))) );
    CHECK((NEW(VarExpr)("x"))
                   ->subst("7", NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7)))
                   ->equals(NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))) == false );
    CHECK( (NEW(VarExpr)("x"))
                   ->subst("7", NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7)))
                   ->equals(NEW(VarExpr)("x")) );

    CHECK((NEW(NumExpr)(1))->has_variable() == false);
    CHECK((NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)))->has_variable() == false);
    CHECK((NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)))->has_variable() == true);
    CHECK( (NEW(NumExpr)(-1))->equals(NEW(NumExpr)(-1)) == true );
    CHECK( (NEW(NumExpr)(-1))->equals(NEW(NumExpr)(0)) == false );

    PTR(AddExpr) e = NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(3)), NEW(NumExpr)(2));
    std::stringstream ss;
    e->pretty_print(ss);
    CHECK(ss.str() == "(1 + 3) + 2");

    e = NEW(AddExpr)(NEW(NumExpr)(2), NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(3)));
    ss = std::stringstream();
    e->pretty_print(ss);
    CHECK(ss.str() == "2 + 1 + 3");

    // Expr - abstract type/interface/abstract base class
    // VarExpr - Concrete Type because it has no unimplemented methods
    // Expr e = NEW(VarExpr)()
    CHECK((NEW(NumExpr)(1))->to_string() == "1");
    CHECK((NEW(VarExpr)("x"))->to_string() == "x");
    CHECK((NEW(AddExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)))->to_string() == "(1+2)");

    CHECK ( ( NEW(MultExpr)(NEW(NumExpr)(2), NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(3))))
    ->to_pretty_string() == "2 * (1 + 3)");
    CHECK ( ( NEW(AddExpr)(NEW(NumExpr)(2), NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(3))))
    ->to_pretty_string() == "2 + 1 * 3");

    CHECK ( (NEW(MultExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))
    ->to_pretty_string() ==  "1 * (2 + 3)" );
    CHECK ( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(8), NEW(NumExpr)(1)), NEW(VarExpr)("y")))
    ->to_pretty_string() ==  "(8 * 1) * y" );
    CHECK ( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(8), NEW(NumExpr)(1)), NEW(VarExpr)("y")))
                    ->to_string() ==  "((8*1)*y)" );
    CHECK ( (NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(5)), NEW(MultExpr)(NEW(NumExpr)(6), NEW(NumExpr)(1))))
    ->to_pretty_string() ==  "(3 + 5) * 6 * 1" );
    CHECK ( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(7), NEW(NumExpr)(7)), NEW(AddExpr)(NEW(NumExpr)(9), NEW(NumExpr)(2))) )
    ->to_pretty_string() ==  "(7 * 7) * (9 + 2)" );
    CHECK ( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(7), NEW(NumExpr)(7)), NEW(AddExpr)(NEW(NumExpr)(9), NEW(NumExpr)(2))) )
                    ->to_string() ==  "((7*7)*(9+2))" );
    CHECK ( (NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(7), NEW(MultExpr)(NEW(NumExpr)(7), NEW(NumExpr)(9))), NEW(NumExpr)(2)))
                    ->to_string() ==  "((7*(7*9))+2)" );
    CHECK ( (NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(7), NEW(MultExpr)(NEW(NumExpr)(7), NEW(NumExpr)(9))), NEW(NumExpr)(2)))
                    ->to_pretty_string() ==  "7 * 7 * 9 + 2" );
    CHECK ( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(NumExpr)(2)), NEW(NumExpr)(8)))
    ->to_pretty_string() == "(5 * 2) * 8");
    CHECK ( (NEW(MultExpr)(NEW(NumExpr)(8), NEW(MultExpr)(NEW(NumExpr)(5), NEW(NumExpr)(2))))
    ->to_pretty_string() == "8 * 5 * 2");

    //---------------------------------------LetExpr Tests-----------------------------------------------------------------------------------------------//
    //CHECK ( (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x"))))->to_string() == "(_let x = 5 _in ((_let y = 3 _in (y+2))+x))");

    PTR(Expr) a = NEW(NumExpr)(1);
    PTR(Expr) b = NEW(NumExpr)(2);
    PTR(Expr) c = NEW(NumExpr)(2);
    PTR(Expr) d = NEW(NumExpr)(3);
    PTR(LetExpr) abx2 = NEW(LetExpr)("x", NEW(AddExpr)(a, b), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)));
    cout << abx2->to_string() <<"\n";
    PTR(LetExpr) bax2 = NEW(LetExpr)("x", NEW(AddExpr)(b,a), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)));
    cout << bax2->to_string() <<"\n";
    PTR(LetExpr) acx2 = NEW(LetExpr)("x", NEW(AddExpr)(a, c), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)));
    PTR(LetExpr) adx2 = NEW(LetExpr)("x", NEW(AddExpr)(a, d), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)));
    PTR(LetExpr) adx3= NEW(LetExpr)("x", NEW(AddExpr)(a, d), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)));
    PTR(LetExpr) ady2 = NEW(LetExpr)("x", NEW(AddExpr)(a, d), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2)));
    PTR(LetExpr) ady3 = NEW(LetExpr)("x", NEW(AddExpr)(a, d), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3)));
    PTR(LetExpr) ady3clone = NEW(LetExpr)("x", NEW(AddExpr)(a, d), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3)));
//    //bound
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
    PTR(LetExpr) nested_abx2 = NEW(LetExpr)("y", NEW(NumExpr)(4), NEW(LetExpr)("x", NEW(AddExpr)(a, b), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))));
    PTR(LetExpr) nested_acx2 = NEW(LetExpr)("y", NEW(NumExpr)(4), NEW(LetExpr)("x", NEW(AddExpr)(a, c), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))));
    CHECK(nested_abx2->equals(nested_acx2));


    //Substitute
    //Dont sub x because lhs = valToSub
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5))))
    ->subst("x", NEW(NumExpr)(4))
                   ->equals( NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)))));
    //Don't sub x because valToSub is not contained
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5))))
    ->subst("y", NEW(NumExpr)(4))
                   ->equals( NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)))));

    CHECK((NEW(LetExpr)("y", NEW(NumExpr)(1),
                       NEW(LetExpr)("z", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)),
                                   NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(1))))
                                   // _let y=1 _in ( _let z=x + 1 _in (z+1))
          )->subst("x", NEW(NumExpr)(2))
                                    //x =2
                  ->equals(NEW(LetExpr)("y", NEW(NumExpr)(1),
                                       NEW(LetExpr)("z", NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(1)),
                                                   NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(1)))
                  )));
                            //_let y=1 _in (_let z=2+1 _ in (z+1)

    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2)))
            //_let x=5 _in y + 2
          )->subst("y", NEW(NumExpr)(3))
                    // y =3
                  ->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2)))
                  ));
    //_let x=5 _in 3 + 2

    CHECK((NEW(LetExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"),NEW(NumExpr)(1)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(30))))
        ->subst("x", NEW(NumExpr)(300))
                ->equals((NEW(LetExpr)("x", NEW(AddExpr)(NEW(NumExpr)(300),NEW(NumExpr)(1)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(30))))));



    CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr) (2), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x") , NEW(NumExpr)(1)) )), NEW(NumExpr)(3) ))
    ->to_pretty_string() == "(2 * _let x = 5\n"
                           "      _in  x + 1) * 3");
//    //Let nested to the left in add expression which is nested to the right within a multiplication expression
//    CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(1))))->to_pretty_string() == "5 * ((_let x = 5\n"
//                                                                                                                         "      _in x) + 1)");
    CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(1))))->to_pretty_string() == "5 * ((_let x = 5\n"
                                                                                                                         "       _in  x) + 1)");
    //    //Let in lhs of add
    CHECK ( (NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(2), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(9))), NEW(NumExpr)(4)))->to_pretty_string() == "(_let x = 2\n"
                                                                                                                          "  _in  x + 9) + 4");
    //Let in lhs of multiplication expression
    CHECK((NEW(MultExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(8))), NEW(NumExpr)(3)))->to_pretty_string() == "(_let x = 5\n"
                                                                                                                         "  _in  x + 8) * 3");
    //Let nest as right argument of un-parenthesized multiplication expression
    CHECK((NEW(AddExpr) (NEW(MultExpr)(NEW(NumExpr)(4), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))), NEW(NumExpr)(9)))->to_pretty_string() == "4 * (_let x = 5\n"
                                                                                                                                               "      _in  x + 1) + 9");

}

TEST_CASE("parse") {
//    CHECK_THROWS_WITH( parseString("()"), "bad input" );

    CHECK( parseString("(1)")->equals(NEW(NumExpr)(1)) );
    CHECK( parseString("(((1)))")->equals(NEW(NumExpr)(1)) );

//    CHECK_THROWS_WITH( parseString("(1"), "missing closing parenthesis" );

    CHECK( parseString("1")->equals(NEW(NumExpr)(1)) );
    CHECK( parseString("10")->equals(NEW(NumExpr)(10)) );
    CHECK( parseString("-3")->equals(NEW(NumExpr)(-3)) );
    CHECK( parseString("  \n 5  ")->equals(NEW(NumExpr)(5)) );
//    CHECK_THROWS_WITH( parseString("-"), "no following digit after the negative sign, invalid input" );


//    CHECK_THROWS_WITH( parseString(" -   5  "), "no following digit after the negative sign, invalid input" );

    CHECK( parseString("x")->equals(NEW(VarExpr)("x")) );
    CHECK( parseString("xyz")->equals(NEW(VarExpr)("xyz")) );
    CHECK( parseString("xYz")->equals(NEW(VarExpr)("xYz")) );
//    CHECK_THROWS_WITH( parseString("x_z"), "invalid input in parse" );
    CHECK( parseString("x + y")->equals(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))) );

    CHECK( parseString("x * y")->equals(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))) );

    CHECK( parseString("z * x + y")
                   ->equals(NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("z"), NEW(VarExpr)("x")),
                                    NEW(VarExpr)("y"))) );

    CHECK( parseString("z * (x + y)")
                   ->equals(NEW(MultExpr)(NEW(VarExpr)("z"),
                                         NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))) ));


    CHECK(parseString("_let x = 5\n_in  (_let y = 3\n      _in  y + 2) + x")
                   ->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x")) )));

    CHECK( parseString("_let x = 5 _in x + 5")->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)))));
    CHECK( parseString("_let x = 5 _in ((_let x = 5 _in x + 1) + x)")->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), NEW(VarExpr)("x")))));
    CHECK( parseString("_let x = 5 _in ((_let x = 5 _in x * 1) * x)")->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), NEW(VarExpr)("x")))));
    CHECK( parseString("x * ((_let x = 5 _in x * 1) * x)")->equals(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), NEW(VarExpr)("x")))));

    printf("HERE");
    CHECK((NEW(BooleanExpr)(true))->to_string()=="_true");
    CHECK(parseString("_true ")->to_string()=="_true");
    CHECK(parseString("_false")->equals(NEW(BooleanExpr)(false)));
    CHECK(parseString("1 == 2")->equals(NEW(EqualsExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2))));
    CHECK(parseString("_if _true _then 10 _else 20")->equals(NEW(IfExpr)(NEW(BooleanExpr)(true), NEW(NumExpr)(10), NEW(NumExpr)(20))));
    CHECK(parseString("_let x = _true _in x")->equals(NEW(LetExpr)("x", NEW(BooleanExpr)(true), NEW(VarExpr)("x"))));

    CHECK((NEW(EqualsExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)))->interp(Env::empty)->equals(NEW(BoolVal)(true)));
    CHECK((NEW(EqualsExpr)(NEW(BooleanExpr)(true), NEW(BooleanExpr)(false)))->interp(Env::empty)->equals(NEW(BoolVal)(false)));
    CHECK((NEW(IfExpr)(NEW(BooleanExpr)(true), NEW(NumExpr)(10), NEW(NumExpr)(20)))->interp(Env::empty)->equals(NEW(NumVal)(10)));
    CHECK((NEW(IfExpr)(NEW(BooleanExpr)(false), NEW(NumExpr)(10), NEW(NumExpr)(20)))->interp(Env::empty)->equals(NEW(NumVal)(20)));
    CHECK_THROWS_WITH((NEW(AddExpr)(NEW(BooleanExpr)(true), NEW(NumExpr)(10)))->interp(Env::empty), "have to add to a number");

    CHECK(parseString("_true")->to_string() == "_true");
    CHECK(parseString("_false")->to_string() == "_false");
    CHECK(parseString("1 == 2")->to_string() == "(1==2)");

    CHECK(parseString("1 == (2 + 3)")->to_pretty_string() == "1 == (2 + 3)");

}
TEST_CASE("Nested Functions and Scope [Function]") {
//    CHECK(parseString("_let outer = _fun (x) _fun (y) x + y _in (outer(5))(3)")->interp(Env::empty)->to_string() == "8");
////
//    CHECK(parseString("_let x = 10 _in (_let f = _fun (x) x * 2 _in f(5))")->interp(Env::empty)->to_string() == "10");
//    CHECK((NEW(LetExpr)("factrl", NEW(FunExpr)("factrl", NEW(FunExpr)("x", NEW(IfExpr)(NEW(EqualsExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), NEW(NumExpr)(1), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(CallExpr)( NEW(CallExpr)( NEW(VarExpr)("factrl"), NEW(VarExpr)("factrl")),NEW(AddExpr)( NEW(VarExpr)("x"), NEW(NumExpr)(-1))))))), NEW(CallExpr)( NEW(CallExpr)( NEW(VarExpr)("factrl"), NEW(VarExpr)("factrl")), NEW(NumExpr)(10))))->interp(Env::empty)->equals(NEW(NumVal)(3628800)));
//    SECTION("FunExpr")
//    {
//        CHECK(parseString("_fun (x) x + 2")->equals(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))));
//
//        //FunExpr with AddExpr
//        CHECK(parseString("_fun (x)\n"
//                          "  2 + x")->equals(NEW(FunExpr)("x", NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")))));
//
////        FunExpr with nested LetExpr
//        CHECK(parseString("_fun (x)\n  x + _let y = 5\n      _in  y + 6")->equals(NEW(FunExpr)("x", NEW(AddExpr)(
//                NEW(VarExpr)("x"), NEW(LetExpr)("y", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(6)))))));
//
//        //FunExpr nested within AddExpr
//        CHECK(parseString("4 + _fun (x)\n5 * x")->equals(
//                NEW(AddExpr)(NEW(NumExpr)(4), NEW(FunExpr)("x", NEW(MultExpr)(NEW(NumExpr)(5), NEW(VarExpr)("x"))))));
//
//        //FunExpr nested within LetExpr
//        CHECK(parseString("_let x = 3\n_in  _fun (y)\ny + x")->equals(
//                NEW(LetExpr)("x", NEW(NumExpr)(3), NEW(FunExpr)("y", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("x"))))));
//
//        //FunExp nested within FunExpr
//        CHECK(parseString("_fun (x)\n      x * (_fun (y)\n  \n \n 4 + y)")->equals(NEW(FunExpr)("x", NEW(MultExpr)(
//                NEW(VarExpr)("x"), NEW(FunExpr)("y", NEW(AddExpr)(NEW(NumExpr)(4), NEW(VarExpr)("y")))))));
//    }

    SECTION("CallExpr")
    {
        //CallExpr with FunExpr and VarExpr
        CHECK(parseString("(_fun (x)\nx + 4)(x)")->equals(
                NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(4))), NEW(VarExpr)("x"))));

        //CallExpr with FunExpr and AddExpr
        CHECK(parseString("(_fun (x)\nx + 4)(3 + 8)")->equals(
                NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(4))),
                             NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(8)))));

        //CallExpr with FunExpr and MultExpr
        CHECK(parseString("(_fun (y)\n y + 7)(1 * 5)")->equals(
                NEW(CallExpr)(NEW(FunExpr)("y", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))),
                             NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(5)))));

        //CallExpr with FunExpr and LetExpr
        CHECK(parseString("(_fun (y)\n y + 7)(_let x = 9\n   \n  \n _in  x + 4)")->equals(
                NEW(CallExpr)(NEW(FunExpr)("y", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))),
                             NEW(LetExpr)("x", NEW(NumExpr)(9), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(4))))));

        //CallExpr with FunExpr and NumExpr
        CHECK(parseString("(_fun (y)\n   \n \n  \n \n \n y + 7)(0)")->equals(
                NEW(CallExpr)(NEW(FunExpr)("y", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))), NEW(NumExpr)(0))));

//        CHECK(parseString((
//                "_let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1) _in  factrl(factrl)(10)"))->interp(Env::empty)->equals(
//                NEW(NumVal)(3628800)));
    }
//369
    SECTION("FunExpr::Equals")
    {
//True check
        CHECK((NEW(FunExpr)("x", NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(3))))->equals(
                NEW(FunExpr)("x", NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(3)))));
//False check
        CHECK_FALSE((NEW(FunExpr)("y", NEW(MultExpr)(NEW(NumExpr)(9), NEW(NumExpr)(0))))->equals(
                NEW(FunExpr)("x", NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(3)))));
    }

    SECTION("FunExpr::Interp")
    {
//FunExpr with NumExpr body
        CHECK((NEW(FunExpr)("x", NEW(NumExpr)(5)))->interp(Env::empty)->equals(NEW(FunVal)("x", NEW(NumExpr)(5), Env::empty)));
//FunExpr with AddExpr body
        CHECK((NEW(FunExpr)("y", NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->interp(Env::empty)->equals(
                NEW(FunVal)("y", NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)),Env::empty)));
//FunExpr with MultExpr body
        CHECK((NEW(FunExpr)("z", NEW(MultExpr)(NEW(NumExpr)(8), NEW(NumExpr)(12))))->interp(Env::empty)->equals(
                NEW(FunVal)("z", NEW(MultExpr)(NEW(NumExpr)(8), NEW(NumExpr)(12)),Env::empty)));
//FunExpr with LetExpr body
        CHECK((NEW(FunExpr)("x", NEW(LetExpr)("f", NEW(NumExpr)(4),
                                            NEW(AddExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(8)))))->interp(Env::empty)->equals(
                NEW(FunVal)("x", NEW(LetExpr)("f", NEW(NumExpr)(4), NEW(AddExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(8))),Env::empty)));


    }

    SECTION("FunExpr::Substituion")
    {

//Unsuccessful substitution when valToSub is nested in FunExpr->body
        CHECK((NEW(FunExpr)("x", NEW(AddExpr)(NEW(NumExpr)(3), NEW(IfExpr)(NEW(BooleanExpr)(true),
                                                                       NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                   NEW(VarExpr)("y")),
                                                                       NEW(NumExpr)(4)))))
                      ->subst("x", NEW(NumExpr)(3))->equals
                (NEW(FunExpr)("x", NEW(AddExpr)(NEW(NumExpr)(3), NEW(IfExpr)(NEW(BooleanExpr)(true),
                                                                         NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                     NEW(VarExpr)("y")),
                                                                         NEW(NumExpr)(4))))));
//Unsuccessfuls substitution when FunExpr has valToSub in multiple locations
        CHECK((NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(IfExpr)(NEW(BooleanExpr)(true),
                                                                         NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                     NEW(VarExpr)("y")),
                                                                         NEW(NumExpr)(4)))))->subst("x", NEW(NumExpr)(
                5))->equals(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(IfExpr)(NEW(BooleanExpr)(true),
                                                                                      NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                                  NEW(VarExpr)("y")),
                                                                                      NEW(NumExpr)(4))))));
    }

    SECTION("FunExpr::Print")
    {
//Basic FunExpr
        CHECK((NEW(FunExpr)("x", NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x"))))->to_string() ==
              "(_fun (x) (2+x))");
//FunExpr with nested LetExpr
        CHECK((NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(LetExpr)("y", NEW(NumExpr)(5),
                                                                          NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(
                                                                                  6))))))->to_string() ==
              "(_fun (x) (x+(_let y=5 _in (y+6))))");
//FunExpr nested within AddExpr
        CHECK((NEW(AddExpr)(NEW(NumExpr)(4),
                           NEW(FunExpr)("x", NEW(MultExpr)(NEW(NumExpr)(5), NEW(VarExpr)("x")))))->to_string() ==
              "(4+(_fun (x) (5*x)))");
//FunExpr nested within LetExpr
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(3),
                           NEW(FunExpr)("y", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("x")))))->to_string() ==
              "(_let x=3 _in (_fun (y) (y+x)))");
//FunExp nested within FunExpr
        CHECK((NEW(FunExpr)("x", NEW(MultExpr)(NEW(VarExpr)("x"), NEW(FunExpr)("y", NEW(AddExpr)(NEW(NumExpr)(4),
                                                                                            NEW(VarExpr)(
                                                                                                    "y"))))))->to_string() ==
              "(_fun (x) (x*(_fun (y) (4+y))))");
    }

    SECTION("CallExpr::Equals")
    {
//True check
        CHECK((NEW(CallExpr)(NEW(NumExpr)(5), NEW(NumExpr)(7)))->equals(NEW(CallExpr)(NEW(NumExpr)(5), NEW(NumExpr)(7))));
//False check
        CHECK(!(NEW(CallExpr)(NEW(VarExpr)("t"), NEW(NumExpr)(7)))->equals(
                NEW(CallExpr)(NEW(NumExpr)(5), NEW(NumExpr)(7))));
    }

    SECTION("CallExpr::Interp")
    {
//Error handle for NumVal
        CHECK_THROWS_WITH((NEW(CallExpr)(NEW(NumExpr)(5), NEW(NumExpr)(9)))->interp(Env::empty), "NumVal cannot call");
//Error handle for BoolVal
        CHECK_THROWS_WITH((NEW(CallExpr)(NEW(BooleanExpr)(true), NEW(NumExpr)(9)))->interp(Env::empty), "BoolVal cannot call");
//Interp on FunExpr when substituting to_be_called with a NumExpr
        CHECK((NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))),
                            NEW(NumExpr)(4)))->interp(Env::empty)->equals(NEW(NumVal)(5)));
//Interp on FunExpr when substituting to_be_called with a AddExpr
        CHECK((NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(9))),
                            NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(7))))->interp(Env::empty)->equals(NEW(NumVal)(19)));
//Interp on FunExpr when substituting to_be_called with a MultExpr
        CHECK((NEW(CallExpr)(NEW(FunExpr)("x", NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3))),
                            NEW(MultExpr)(NEW(NumExpr)(6), NEW(NumExpr)(2))))->interp(Env::empty)->equals(NEW(NumVal)(36)));

    }

    SECTION("CallExpr::Substituion")
    {
//Successful substitution on basic CallExpr
        CHECK((NEW(CallExpr)(NEW(NumExpr)(3), NEW(VarExpr)("x")))->subst("x", NEW(NumExpr)(7))->equals(
                NEW(CallExpr)(NEW(NumExpr)(3), NEW(NumExpr)(7))));
//Unsuccessful substitution on basic CallExpr
        CHECK((NEW(CallExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4)))->subst("x", NEW(NumExpr)(7))->equals(
                NEW(CallExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))));
//Successful substitution when valToSub is nested in to_be_called expression
        CHECK((NEW(CallExpr)(NEW(AddExpr)(NEW(NumExpr)(4), NEW(VarExpr)("x")), NEW(NumExpr)(5)))->subst("x", NEW(NumExpr)(
                8))->equals(NEW(CallExpr)(NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(8)), NEW(NumExpr)(5))));
//Successful substitution when valToSub nested in actual_arg expression
        CHECK((NEW(CallExpr)(NEW(NumExpr)(4), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(0))))->subst("y",
                                                                                                    NEW(NumExpr)(
                                                                                                            9))->equals(
                NEW(CallExpr)(NEW(NumExpr)(4), NEW(MultExpr)(NEW(NumExpr)(9), NEW(NumExpr)(0)))));
//Unsuccessful substitution when valToSub is not in ntested actual_arg expression
        CHECK((NEW(CallExpr)(NEW(NumExpr)(4), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(0))))->subst("z",
                                                                                                    NEW(NumExpr)(
                                                                                                            9))->equals(
                NEW(CallExpr)(NEW(NumExpr)(4), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(0)))));
    }

    SECTION("CallExpr::Print")
    {
//FunExpr with VarExpr
        CHECK((NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(4))),
                            NEW(VarExpr)("x")))->to_string() ==
              "(_fun (x) (x+4)) x");
//FunExpr with AddExpr
        CHECK((NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(4))),
                            NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(8))))->to_string() ==
              "(_fun (x) (x+4)) (3+8)");
//FunExpr with MultExpr
        CHECK((NEW(CallExpr)(NEW(FunExpr)("y", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))),
                            NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(5))))->to_string() ==
              "(_fun (y) (y+7)) (1*5)");
//FunExpr with LetExpr
        CHECK((NEW(CallExpr)(NEW(FunExpr)("y", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))),
                            NEW(LetExpr)("x", NEW(NumExpr)(9),
                                        NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(4)))))->to_string() ==
              "(_fun (y) (y+7)) (_let x=9 _in (x+4))");
//FunExpr with NumExpr
        CHECK((NEW(CallExpr)(NEW(FunExpr)("y", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))),
                            NEW(NumExpr)(0)))->to_string() ==
              "(_fun (y) (y+7)) 0");
    }
}


