/**
 * @file testExpr.cpp
 * @brief all Expr class functions tests
 *
 * @author Zidong Fan
 * @date 01/20/2023
 */

#include "catch.h"
#include "val.h"
#include "expr.h"
#include "parse.h"
#include "env.h"

TEST_CASE("equals") {
    // NumExpr equals Tests
    SECTION("num") {
        CHECK((NEW(NumExpr)(0))->equals(NEW(NumExpr)(0)) == true);
        CHECK((NEW(NumExpr)(1))->equals(NEW(NumExpr)(1)) == true);
        CHECK((NEW(NumExpr)(999))->equals(NEW(NumExpr)(999)) == true);
        CHECK((NEW(NumExpr)(-1))->equals(NEW(NumExpr)(-1)) == true);
        CHECK((NEW(NumExpr)(-999))->equals(NEW(NumExpr)(-999)) == true);
        CHECK((NEW(NumExpr)(0))->equals(NEW(NumExpr)(1)) == false);
        CHECK((NEW(NumExpr)(-1))->equals(NEW(NumExpr)(1)) == false);
        CHECK((NEW(NumExpr)(1))->equals(NEW(NumExpr)(2)) == false);
    }
        // AddExpr equals Tests
    SECTION("add") {
        CHECK((NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)))
                      ->equals(NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0))) == true);
        CHECK((NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(1)))
                      ->equals(NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(1))) == true);
        CHECK((NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)))
                      ->equals(NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))) == true);
        CHECK((NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(1)))
                      ->equals(NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2))) == false);
        CHECK((NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)))
                      ->equals(NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2))) == false);
        CHECK((NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2)))
                      ->equals(NEW(AddExpr)(NEW(NumExpr)(-2), NEW(NumExpr)(2))) == false);
        CHECK((NEW(AddExpr)(NEW(NumExpr)(-2), NEW(NumExpr)(-3)))
                      ->equals(NEW(AddExpr)(NEW(NumExpr)(-3), NEW(NumExpr)(-2))) == false);
    }
        // Mul equals Tests
    SECTION("mult") {
        CHECK((NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)))->equals(
                NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0))) == true);
        CHECK((NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(1)))->equals(
                NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(1))) == true);
        CHECK((NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)))->equals(
                NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))) == true);
        CHECK((NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)))->equals(
                NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2))) == false);
        CHECK((NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)))->equals(
                NEW(MultExpr)(NEW(NumExpr)(-2), NEW(NumExpr)(-3))) == false);
        CHECK((NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(1)))->equals(
                NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(2))) == false);
        CHECK((NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(1)))->equals(
                NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(-1))) == false);
    }
        // VarExpr equals Tests
    SECTION("var") {
        CHECK((NEW(VarExpr)("x"))->equals(NEW(VarExpr)("x")) == true);
        CHECK((NEW(VarExpr)("y"))->equals(NEW(VarExpr)("y")) == true);
        CHECK((NEW(VarExpr)("hello"))->equals(NEW(VarExpr)("hello")) == true);
        CHECK((NEW(VarExpr)("?"))->equals(NEW(VarExpr)("?")) == true);
        CHECK((NEW(VarExpr)("x"))->equals(NEW(VarExpr)("y")) == false);
        CHECK((NEW(VarExpr)("x"))->equals(NEW(VarExpr)("X")) == false);
        CHECK((NEW(VarExpr)("hello"))->equals(NEW(VarExpr)("Hello")) == false);
        CHECK((NEW(VarExpr)("x"))->equals(NEW(NumExpr)(1)) == false);
    }
        // LetExpr equals Tests
    SECTION("let") {
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                      ->equals(
                              (NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))) ==
              true);
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1))))
                      ->equals(
                              (NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1))))) ==
              true);
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                      ->equals(
                              (NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1))))) ==
              false);
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                      ->equals((NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))) == false);
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                      ->equals((NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))) == false);
    }
}

/*
 interp Test Examples:
 CHECK( (NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2)))
         ->interp()==6 );
 CHECK( (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(15)),NEW(AddExpr)(NEW(NumExpr)(20),NEW(NumExpr)(20))))
         ->interp()==65);
 */
TEST_CASE("interp") {
    // NumExpr interp Tests
    SECTION("num") {
        CHECK((NEW(NumExpr)(0))->interp(Env::empty)->equals(NEW(NumVal)(0)));
        CHECK((NEW(NumExpr)(1))->interp(Env::empty)->equals(NEW(NumVal)(1)));
        CHECK((NEW(NumExpr)(-1))->interp(Env::empty)->equals(NEW(NumVal)(-1)));
        CHECK((NEW(NumExpr)(999))->interp(Env::empty)->equals(NEW(NumVal)(999)));
        CHECK((NEW(NumExpr)(-999))->interp(Env::empty)->equals(NEW(NumVal)(-999)));
        CHECK((NEW(NumExpr)(1234))->interp(Env::empty)->equals(NEW(NumVal)(1234)));
    }
        // AddExpr interp Tests
    SECTION("add") {
        CHECK((NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(15)),
                            NEW(AddExpr)(NEW(NumExpr)(20), NEW(NumExpr)(20))))->interp(Env::empty)->equals(NEW(NumVal)(65)));
        CHECK((NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)))->interp(Env::empty)->equals(NEW(NumVal)(0)));
        CHECK((NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)))->interp(Env::empty)->equals(NEW(NumVal)(5)));
        CHECK((NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(-3)))->interp(Env::empty)->equals(NEW(NumVal)(-1)));
        CHECK((NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->interp(Env::empty)->equals(
                NEW(NumVal)(6)));
        CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->interp(Env::empty)->equals(
                NEW(NumVal)(5)));
        CHECK((NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)),
                            NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))->interp(Env::empty)->equals(NEW(NumVal)(10)));
        CHECK((NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)),
                            NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))->interp(Env::empty)->equals(NEW(NumVal)(15)));
        CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)),
                            NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))->interp(Env::empty)->equals(NEW(NumVal)(14)));
    }
        // Mul interp Tests
    SECTION("mult") {
        CHECK((NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2)))->interp(Env::empty)->equals(NEW(NumVal)(6)));
        CHECK((NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)))->interp(Env::empty)->equals(NEW(NumVal)(0)));
        CHECK((NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)))->interp(Env::empty)->equals(NEW(NumVal)(6)));
        CHECK((NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(-3)))->interp(Env::empty)->equals(NEW(NumVal)(-6)));
        CHECK((NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->interp(Env::empty)->equals(
                NEW(NumVal)(9)));
        CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->interp(Env::empty)->equals(
                NEW(NumVal)(6)));
        CHECK((NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)),
                             NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))->interp(Env::empty)->equals(NEW(NumVal)(21)));
        CHECK((NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)),
                             NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))->interp(Env::empty)->equals(NEW(NumVal)(36)));
        CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)),
                             NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))->interp(Env::empty));
    }
        // VarExpr interp Tests
    SECTION("var") {
        CHECK_THROWS_WITH((NEW(VarExpr)("x"))->interp(Env::empty), "free variable: x");
        CHECK_THROWS_WITH((NEW(VarExpr)("1"))->interp(Env::empty), "free variable: 1");
        CHECK_THROWS_WITH((NEW(VarExpr)("?"))->interp(Env::empty), "free variable: ?");
        CHECK_THROWS_WITH((NEW(VarExpr)("Hello"))->interp(Env::empty), "free variable: Hello");
        CHECK_THROWS_WITH((NEW(AddExpr)(NEW(NumExpr)(1), NEW(VarExpr)("x")))->interp(Env::empty), "free variable: x");
        CHECK_THROWS_WITH((NEW(MultExpr)(NEW(NumExpr)(1), NEW(VarExpr)("x")))->interp(Env::empty), "free variable: x" );
    }
        // LetExpr interp Tests
    SECTION("let") {
        // NEW(MultExpr)(NEW(NumExpr)(5),
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->interp(Env::empty)->equals(
                NEW(NumVal)(2)));
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->interp(Env::empty)->equals(
                NEW(NumVal)(6)));
        CHECK((NEW(LetExpr)("x", NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)),
                            NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3))))->interp(Env::empty)->equals(NEW(NumVal)(6)));
        CHECK((NEW(LetExpr)("hello", NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)),
                            NEW(AddExpr)(NEW(VarExpr)("hello"), NEW(NumExpr)(3))))->interp(Env::empty)->equals(NEW(NumVal)(5)));
        CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x"))),
                            NEW(NumExpr)(1)))->interp(Env::empty)->equals(NEW(NumVal)(26)));
        CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                              NEW(NumExpr)(
                                                                                                      1)))))->interp(Env::empty)->equals(
                NEW(NumVal)(30)));
        CHECK_THROWS_WITH((NEW(LetExpr)("x", NEW(AddExpr)(NEW(NumExpr)(1), NEW(VarExpr)("y")),
                                        NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3))))->interp(Env::empty),
                          "free variable: y");
    }
}

TEST_CASE("has_variable") {
    // NumExpr has_variable Tests
    SECTION("num") {
        CHECK((NEW(NumExpr)(0))->has_variable() == false);
        CHECK((NEW(NumExpr)(1))->has_variable() == false);
        CHECK((NEW(NumExpr)(-1))->has_variable() == false);
    }
        // AddExpr has_variable Tests
    SECTION("add") {
        CHECK((NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))->has_variable() == true);
        CHECK((NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")))->has_variable() == true);
        CHECK((NEW(AddExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), NEW(NumExpr)(1)))->has_variable() ==
              true);
        CHECK((NEW(AddExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), NEW(VarExpr)("x")))->has_variable() ==
              true);
        CHECK((NEW(AddExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)),
                            NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1))))->has_variable() == true);
        CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)))->has_variable() == false);
        CHECK((NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)),
                            NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1))))->has_variable() == false);
    }
        // Mul has_variable Tests
    SECTION("mult") {
        CHECK((NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))->has_variable() == true);
        CHECK((NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")))->has_variable() == true);
        CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), NEW(NumExpr)(1)))->has_variable() ==
              true);
        CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), NEW(VarExpr)("x")))->has_variable() ==
              true);
        CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)),
                             NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1))))->has_variable() == true);
        CHECK((NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)))->has_variable() == false);
        CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)),
                             NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1))))->has_variable() == false);
    }
        // VarExpr has_variable Tests
    SECTION("var") {
        CHECK((NEW(VarExpr)("x"))->has_variable() == true);
        CHECK((NEW(VarExpr)("hello"))->has_variable() == true);
        CHECK((NEW(VarExpr)("?"))->has_variable() == true);
    }
        // LetExpr has_variable Tests
    SECTION("let") {
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(NumExpr)(2)))->has_variable() == false);
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->has_variable() ==
              false);
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3))))->has_variable() ==
              true);
        CHECK((NEW(LetExpr)("x", NEW(VarExpr)("y"), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->has_variable() ==
              true);
        CHECK((NEW(LetExpr)("x", NEW(VarExpr)("y"),
                            NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3))))->has_variable() == true);
    }
}

/*
 subst Test Examples:
 CHECK( (NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))
        ->subst("x", NEW(VarExpr)("y"))
        ->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))) );
 CHECK( (NEW(VarExpr)("x"))
        ->subst("x", NEW(AddExpr)(NEW(VarExpr)("y"),NEW(NumExpr)(7)))
        ->equals(NEW(AddExpr)(NEW(VarExpr)("y"),NEW(NumExpr)(7))) );
 */
TEST_CASE("subst") {
    // NumExpr subst Tests
    SECTION("num") {
        CHECK((NEW(NumExpr)(0))->subst("x", NEW(VarExpr)("y"))->equals(NEW(NumExpr)(0)));
        CHECK((NEW(NumExpr)(1))->subst("x", NEW(VarExpr)("y"))->equals(NEW(NumExpr)(1)));
        CHECK((NEW(NumExpr)(1))->subst("0", NEW(VarExpr)("y"))->equals(NEW(NumExpr)(1)));
        CHECK((NEW(NumExpr)(1))->subst("0", NEW(NumExpr)(0))->equals(NEW(NumExpr)(1)));
        CHECK((NEW(NumExpr)(1))->subst("1", NEW(NumExpr)(0))->equals(NEW(NumExpr)(1)));
    }
        // AddExpr subst Tests
    SECTION("add") {
        CHECK((NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))->subst("x", NEW(VarExpr)("y"))->equals(
                NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))));
        CHECK((NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))->subst("y", NEW(VarExpr)("y"))->equals(
                NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7))));
        CHECK((NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))->subst("x", NEW(NumExpr)(1))->equals(
                NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(7))));
        CHECK((NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))->subst("y", NEW(NumExpr)(1))->equals(
                NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7))));
        CHECK((NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7)))->subst("y", NEW(AddExpr)(NEW(NumExpr)(1),
                                                                                          NEW(VarExpr)("x")))->equals(
                NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(VarExpr)("x")), NEW(NumExpr)(7))));
    }
        // Mul subst Tests
    SECTION("mult") {
        CHECK((NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))->subst("x", NEW(VarExpr)("y"))->equals(
                NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))));
        CHECK((NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))->subst("y", NEW(VarExpr)("y"))->equals(
                NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7))));
        CHECK((NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))->subst("x", NEW(NumExpr)(1))->equals(
                NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(7))));
        CHECK((NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))->subst("y", NEW(NumExpr)(1))->equals(
                NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7))));
        CHECK((NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7)))->subst("y", NEW(MultExpr)(NEW(NumExpr)(1),
                                                                                            NEW(VarExpr)("x")))->equals(
                NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(1), NEW(VarExpr)("x")), NEW(NumExpr)(7))));

    }
        // VarExpr subst Tests
    SECTION("var") {
        CHECK((NEW(VarExpr)("x"))->subst("x", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7)))->equals(
                NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))));
        CHECK((NEW(VarExpr)("x"))->subst("y", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7)))->equals(
                NEW(VarExpr)("x")));
        CHECK((NEW(VarExpr)("x"))->subst("x", NEW(NumExpr)(1))->equals(NEW(NumExpr)(1)));
        CHECK((NEW(VarExpr)("x"))->subst("y", NEW(NumExpr)(1))->equals(NEW(VarExpr)("x")));
        CHECK((NEW(VarExpr)("x"))->subst("x", NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7)))->equals(
                NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))));
    }
        // LetExpr subst Tests
    SECTION("let") {
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->subst("x",
                                                                                                            NEW(NumExpr)(
                                                                                                                    2))
                      ->equals(NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1))))
                      ->subst("y", NEW(VarExpr)("z"))
                      ->equals(NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(1)))));
        CHECK((NEW(LetExpr)("x", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1)),
                            NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                      ->subst("y", NEW(VarExpr)("z"))
                      ->equals(NEW(LetExpr)("x", NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(1)),
                                            NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));
        CHECK((NEW(LetExpr)("x", NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1)),
                            NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                      ->subst("y", NEW(VarExpr)("z"))
                      ->equals(NEW(LetExpr)("x", NEW(MultExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(1)),
                                            NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));
        CHECK((NEW(LetExpr)("x", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1)),
                            NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1))))
                      ->subst("y", NEW(VarExpr)("z"))
                      ->equals(NEW(LetExpr)("x", NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(1)),
                                            NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(1)))));
    }
}

/*
 DONOT test print() method as it will be less convenient setting up a std::ostream for every test. Test only  to_string method and that would be something like:

 print Test Examples:
 CHECK( (NEW(NumExpr)(10))->to_string() == "10" );
 CHECK( (NEW(AddExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_string() == "(1+(2+3))" );
 CHECK( (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->to_string() == "((1+2)+3)" );
 */
TEST_CASE("print") {
    // NumExpr print Tests
    SECTION("num") {
        CHECK((NEW(NumExpr)(10))->to_string() == "10");
        CHECK((NEW(NumExpr)(0))->to_string() == "0");
        CHECK((NEW(NumExpr)(1))->to_string() == "1");
        CHECK((NEW(NumExpr)(-1))->to_string() == "-1");
        CHECK((NEW(NumExpr)(1234))->to_string() == "1234");
    }
        // AddExpr print Tests
    SECTION("add") {
        CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_string() ==
              "(1+(2+3))");
        CHECK((NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->to_string() ==
              "((1+2)+3)");
        CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_string() ==
              "(1+(2*3))");
        CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->to_string() ==
              "((1*2)+3)");
        CHECK((NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)),
                            NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))->to_string() == "((1+2)+(3+4))");
    }
        // Mul print Tests
    SECTION("mult") {
        CHECK((NEW(MultExpr)(NEW(NumExpr)(1), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_string() ==
              "(1*(2*3))");
        CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->to_string() ==
              "((1*2)*3)");
        CHECK((NEW(MultExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_string() ==
              "(1*(2+3))");
        CHECK((NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->to_string() ==
              "((1+2)*3)");
        CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)),
                             NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))->to_string() == "((1*2)*(3*4))");
    }
        // VarExpr print Tests
    SECTION("var") {
        CHECK((NEW(VarExpr)("x"))->to_string() == "x");
        CHECK((NEW(VarExpr)("y"))->to_string() == "y");
        CHECK((NEW(VarExpr)("z"))->to_string() == "z");
        CHECK((NEW(VarExpr)("0"))->to_string() == "0");
        CHECK((NEW(VarExpr)("?"))->to_string() == "?");
    }
        // LetExpr print Tests
    SECTION("let") {
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(6))))
                      ->to_string() == "(_let x=5 _in (x+6))");
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(6))))
                      ->to_string() == "(_let x=5 _in (x*6))");
        CHECK((NEW(LetExpr)("x", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3)),
                            NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(6))))
                      ->to_string() == "(_let x=(y+3) _in (x+6))");
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(
                NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))),
                NEW(VarExpr)("x"))))
                      ->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
        CHECK((NEW(LetExpr)("x", NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)), NEW(AddExpr)(
                NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))),
                NEW(VarExpr)("x"))))
                      ->to_string() == "(_let x=(2+3) _in ((_let y=3 _in (y+2))+x))");
    }
}

/*
 pretty_print Test Examples:

 CHECK( (NEW(AddExpr)(NEW(NumExpr)(1), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_string() == "1 + 2 * 3" );
 CHECK( (NEW(MultExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_string() == "1 * (2 + 3)" );
 CHECK( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)), NEW(NumExpr)(4)))->to_string() == "(2 * 3) * 4" );
 CHECK( (NEW(MultExpr)(NEW(NumExpr)(2), NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))->to_string() == "2 * 3 * 4" );
 
 */
TEST_CASE("pretty_print") {
    // NumExpr pretty_print Tests
    SECTION("num") {
        CHECK((NEW(NumExpr)(10))->to_pretty_string() == "10");
        CHECK((NEW(NumExpr)(0))->to_pretty_string() == "0");
        CHECK((NEW(NumExpr)(1))->to_pretty_string() == "1");
        CHECK((NEW(NumExpr)(-1))->to_pretty_string() == "-1");
        CHECK((NEW(NumExpr)(1234))->to_pretty_string() == "1234");
    }
        // AddExpr pretty_print Tests
    SECTION("add") {
        CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_pretty_string() ==
              "1 + 2 + 3");
        CHECK((NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->to_pretty_string() ==
              "(1 + 2) + 3");
        CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_pretty_string() ==
              "1 + 2 * 3");
        CHECK((NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)),
                            NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))->to_pretty_string() == "(2 + 3) + 3 + 4");
        CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)),
                            NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))->to_pretty_string() == "2 * 3 + 3 * 4");
    }
        // Mul pretty_print Tests
    SECTION("mult") {
        CHECK((NEW(MultExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_pretty_string() ==
              "1 * (2 + 3)");
        CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)), NEW(NumExpr)(4)))->to_pretty_string() ==
              "(2 * 3) * 4");
        CHECK((NEW(MultExpr)(NEW(NumExpr)(2), NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))->to_pretty_string() ==
              "2 * 3 * 4");
        CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)),
                             NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))->to_pretty_string() ==
              "(2 * 3) * 3 * 4");
        CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10),
                                           NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)),
                                                         NEW(NumExpr)(10))),
                             NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10))))->to_pretty_string() ==
              "(10 * (10 * 10) * 10) * 10 * 10");
    }
        // VarExpr pretty_print Tests
    SECTION("var") {
        CHECK((NEW(VarExpr)("x"))->to_pretty_string() == "x");
        CHECK((NEW(VarExpr)("y"))->to_pretty_string() == "y");
        CHECK((NEW(VarExpr)("z"))->to_pretty_string() == "z");
        CHECK((NEW(VarExpr)("0"))->to_pretty_string() == "0");
        CHECK((NEW(VarExpr)("?"))->to_pretty_string() == "?");
    }
        // LetExpr pretty_print Tests
    SECTION("let") {
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5),
                            NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3),
                                                      NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))),
                                         NEW(VarExpr)("x"))))
                      ->to_pretty_string() == "_let x = 5\n"
                                              "_in  (_let y = 3\n"
                                              "     _in  y + 2) + x");
        CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x"))),
                            NEW(NumExpr)(1)))
                      ->to_pretty_string() == "5 * (_let x = 5\n"
                                              "     _in  x) + 1");
        CHECK((NEW(MultExpr)(NEW(NumExpr)(5),
                             NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))
                      ->to_pretty_string() == "5 * _let x = 5\n"
                                              "    _in  x + 1");
        CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(LetExpr)("x", NEW(NumExpr)(5),
                                                                         NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                      NEW(NumExpr)(1)))),
                             NEW(NumExpr)(3)))
                      ->to_pretty_string() == "(2 * (_let x = 5\n"
                                              "      _in  x + 1)) * 3");
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                      ->to_pretty_string() == "_let x = 5\n"
                                              "_in  x + 1");
        CHECK((NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(1)))
                      ->to_pretty_string() == "(_let x = 5\n"
                                              " _in  x) + 1");
        CHECK((NEW(LetExpr)("x", NEW(LetExpr)("y", NEW(NumExpr)(6), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))),
                            NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                      ->to_pretty_string() == "_let x = _let y = 6\n"
                                              "         _in  y * 2\n"
                                              "_in  x + 1");
        CHECK((NEW(MultExpr)(NEW(NumExpr)(2),
                             NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))
                      ->to_pretty_string() == "2 * _let x = 5\n"
                                              "    _in  x + 1");
        CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5),
                                                                        NEW(MultExpr)(NEW(VarExpr)("x"),
                                                                                      NEW(NumExpr)(2)))),
                            NEW(NumExpr)(1)))
                      ->to_pretty_string() == "5 * (_let x = 5\n"
                                              "     _in  x * 2) + 1");
        CHECK((NEW(MultExpr)((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5),
                                                                                       NEW(MultExpr)(NEW(VarExpr)("x"),
                                                                                                     NEW(NumExpr)(2)))),
                                           NEW(NumExpr)(1))), NEW(NumExpr)(7)))
                      ->to_pretty_string() == "(5 * (_let x = 5\n"
                                              "      _in  x * 2) + 1) * 7");
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(10), NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(
                                                                         NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(NumExpr)(10))),
                                                                 NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))))
                      ->to_pretty_string() == "_let x = 10\n"
                                              "_in  (x * (10 * 10) * 10) * 10 * 10");
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(
                                                                        NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(VarExpr)("x"))),
                                                                NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))))
                      ->to_pretty_string() == "_let x = 1\n"
                                              "_in  (x * (10 * 10) * x) * 10 * 10");
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(
                                                                        NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(VarExpr)("x"))),
                                                                NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(10)))))
                      ->to_pretty_string() == "_let x = 1\n"
                                              "_in  (x * (10 * 10) * x) * y * 10");
    }
}

TEST_CASE("parse") {
    CHECK_THROWS_WITH(parse_str("()"), "invalid input");

    CHECK(parse_str("(1)")->equals(NEW(NumExpr)(1)));
    CHECK(parse_str("(((1)))")->equals(NEW(NumExpr)(1)));

    CHECK_THROWS_WITH(parse_str("(1"), "missing close parenthesis");

    CHECK(parse_str("1")->equals(NEW(NumExpr)(1)));
    CHECK(parse_str("10")->equals(NEW(NumExpr)(10)));
    CHECK(parse_str("-3")->equals(NEW(NumExpr)(-3)));
    CHECK(parse_str("  \n 5  ")->equals(NEW(NumExpr)(5)));
    CHECK_THROWS_WITH(parse_str("-"), "invalid input");

    // This was some temporary debugging code:
    //  std::istringstream in("-");
    //  parse_num(in)->print(std::cout); std::cout << "\n";

    CHECK_THROWS_WITH(parse_str(" -   5  "), "invalid input");

    CHECK(parse_str("x")->equals(NEW(VarExpr)("x")));
    CHECK(parse_str("xyz")->equals(NEW(VarExpr)("xyz")));
    CHECK(parse_str("xYz")->equals(NEW(VarExpr)("xYz")));
    CHECK_THROWS_WITH(parse_str("x_z"), "end of file at _");

    CHECK(parse_str("x + y")->equals(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))));

    CHECK(parse_str("x * y")->equals(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))));

    CHECK(parse_str("z * x + y")
                  ->equals(NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("z"), NEW(VarExpr)("x")),
                                        NEW(VarExpr)("y"))));

    CHECK(parse_str("z * (x + y)")
                  ->equals(NEW(MultExpr)(NEW(VarExpr)("z"),
                                         NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")))));

    CHECK(parse_str("_let x = 5   _in  x + 1")
                  ->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));
    CHECK(parse_str("5 * (_let x = 5   _in  x * 2) + 1")
                  ->equals(NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5),
                                                                                    NEW(MultExpr)(NEW(VarExpr)("x"),
                                                                                                  NEW(NumExpr)(2)))),
                                        NEW(NumExpr)(1))));
}

TEST_CASE("_fun") {
    CHECK(parse_str("_let f = _fun (x) x+ 1 \n"
                    "_in f(5) ")->interp(Env::empty)->equals(NEW(NumVal)(6)));
    CHECK(parse_str("_let f = _fun (x)\n"
                    "           7\n"
                    "_in f(5)")->interp(Env::empty)->equals(NEW(NumVal)(7)));
    CHECK(parse_str("_let f = _fun (x)\n"
                    "           _true\n"
                    "_in f(5) ")->interp(Env::empty)->equals(NEW(BoolVal)(true)));
    CHECK_THROWS_WITH(parse_str("_let f = _fun (x)\n"
                                "           x + _true\n"
                                "_in f(5) ")->interp(Env::empty), "add of non-number");
    CHECK(parse_str("_let f = _fun (x)\n"
                    "           x + _true\n"
                    "_in 5 + 1 ")->interp(Env::empty)->equals(NEW(NumVal)(6)));
    CHECK_THROWS_WITH(parse_str("_let f = _fun (x)\n"
                                "           7\n"
                                "_in  f(5 + _true)")->interp(Env::empty), "add of non-number");
    CHECK_THROWS_WITH(parse_str("_let f = _fun (x) x+ 1\n"
                                "_in f + 5")->interp(Env::empty), "add of non-number");
    CHECK(parse_str("_let f = _fun (x) x+ 1 \n"
                    "_in _if _false\n"
                    "    _then f(5)\n"
                    "    _else f(6)")->interp(Env::empty)->equals(NEW(NumVal)(7)));
    CHECK(parse_str("_let f = _fun (x) x+ 1 \n"
                    "_in _let g = _fun (y) y+ 2 \n"
                    "_in _if _true\n"
                    "    _then f(5)\n"
                    "    _else g(5)")->interp(Env::empty)->equals(NEW(NumVal)(6)));
    CHECK(parse_str("_let f = _fun (x) x+ 1 \n"
                    "_in _let g = _fun (y) y+ 2 \n"
                    "_in f(g(5)) ")->interp(Env::empty)->equals(NEW(NumVal)(8)));
    CHECK(parse_str("_let f = _fun (x) x+ 1 \n"
                    "_in _let g = _fun (y)\n"
                    "              f(y + 2)\n"
                    "_in g(5) ")->interp(Env::empty)->equals(NEW(NumVal)(8)));
    CHECK(parse_str("_let f = _fun (x) x+ 1 \n"
                    "_in _let g = _fun (x)\n"
                    "              f(2) + x\n"
                    "_in g(5) ")->interp(Env::empty)->equals(NEW(NumVal)(8)));
    CHECK_THROWS_WITH(parse_str("_let f = _fun (x) x+ 1 \n"
                                "_in f 5 ")->interp(Env::empty), "end of file at 5");
    CHECK(parse_str("_let f = _fun (x) x+ 1 \n"
                    "_in (f)(5) ")->interp(Env::empty)->equals(NEW(NumVal)(6)));
    CHECK(parse_str("(_fun (x)\n"
                    "   x + 1)(5)")->interp(Env::empty)->equals(NEW(NumVal)(6)));
    CHECK(parse_str("_let f = _if _false\n"
                    "            _then _fun (x)  \n"
                    "                        x+ 1 \n"
                    "           _else _fun (x)\n"
                    "                       x+ 2\n"
                    "_in f(5)")->interp(Env::empty)->equals(NEW(NumVal)(7)));
    CHECK(parse_str("(_if _false \n"
                    "  _then _fun (x)\n"
                    "            x+ 1\n"
                    "   _else _fun (x)\n"
                    "                x + 2)(5)")->interp(Env::empty)->equals(NEW(NumVal)(7)));
    CHECK(parse_str("_let f = _fun (g)\n"
                    "           g(5)\n"
                    "_in _let g = _fun (y)  \n"
                    "             y + 2\n"
                    "_in f(g) ")->interp(Env::empty)->equals(NEW(NumVal)(7)));
    CHECK(parse_str("_let f = _fun (g)\n"
                    "           g(5)\n"
                    "_in f(_fun (y)\n"
                    "        y + 2)")->interp(Env::empty)->equals(NEW(NumVal)(7)));
    CHECK(parse_str("_let f = _fun (x)\n"
                    "           _fun (y)\n"
                    "x+ y _in (f(5))(1) ")->interp(Env::empty)->equals(NEW(NumVal)(6)));
    CHECK(parse_str("_let f = _fun (x)\n"
                    "           _fun (y)\n"
                    "x+ y _in f(5)(1) ")->interp(Env::empty)->equals(NEW(NumVal)(6)));
    CHECK(parse_str("_let f = _fun (x)\n"
                    "           _fun (g)\n"
                    "             g(x + 1)\n"
                    "_in _let g = _fun (y)\n"
                    "              y+ 2 \n"
                    "_in (f(5))(g) ")->interp(Env::empty)->equals(NEW(NumVal)(8)));
    CHECK(parse_str("_let f = _fun (x)\n"
                    "           _fun (g)\n"
                    "             g(x + 1)\n"
                    "_in _let g = _fun (y)\n"
                    "y+ 2 _in f(5)(g)")->interp(Env::empty)->equals(NEW(NumVal)(8)));
    CHECK(parse_str("_let f = _fun (f)\n"
                    "           _fun (x)\n"
                    "             _if x == 0\n"
                    "             _then 0\n"
                    "             _else x + f(f)(x + -1)\n"
                    "_in f(f)(3)")->interp(Env::empty)->equals(NEW(NumVal)(6)));

    PTR(Expr) factrl_expr = NEW(LetExpr)(
            "factrl",
            NEW(FunExpr)(
                    "factrl",
                    NEW(FunExpr)(
                            "x",
                            NEW(IfExpr)(
                                    NEW(EqExpr)(
                                            NEW(VarExpr)("x"),
                                            NEW(NumExpr)(1)
                                    ),
                                    NEW(NumExpr)(1),
                                    NEW(MultExpr)(
                                            NEW(VarExpr)("x"),
                                            NEW(CallExpr)(
                                                    NEW(CallExpr)(
                                                            NEW(VarExpr)("factrl"),
                                                            NEW(VarExpr)("factrl")
                                                    ),
                                                    NEW(AddExpr)(
                                                            NEW(VarExpr)("x"),
                                                            NEW(NumExpr)(-1)
                                                    )
                                            )
                                    )
                            )
                    )
            ),
            NEW(CallExpr)(
                    NEW(CallExpr)(
                            NEW(VarExpr)("factrl"),
                            NEW(VarExpr)("factrl")
                    ),
                    NEW(NumExpr)(10)
            )
    );
    CHECK(factrl_expr->interp(Env::empty)->equals(NEW(NumVal)(3628800)));
}




