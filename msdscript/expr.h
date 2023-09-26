/**
 * @file expr.h
 * @brief contains Expr class
 *  contains NumExpr, AddExpr, MultExpr, VarExpr subclasses
 *
 *  abstract grammar of expressions is:
 *  <expr> = <number>
 *         | <boolean>
 *         | <expr> == <expr>
 *         | <expr> + <expr>
 *         | <expr> * <expr>
 *         | <expr> ( <expr> )
 *         | <variable>
 *         | _let <variable> = <expr> _in <expr>
 *         | _if <expr> _then <expr> _else <expr>
 *         | _fun ( <variable> ) <expr>
 *
 * @author Zidong Fan
 * @date 01/20/2023
 */

#ifndef expr_h
#define expr_h

#include <stdio.h>
#include <string>
#include <stdexcept>
#include <sstream>
#include "pointer.h"

//Hint 2: Here's the C++ way to declare a type precedence_t that has the possible values prec_none, prec_add, and prec_mult, in that order:
typedef enum {
    prec_none,      // = 0
    prec_let,       // = 1
    prec_add,       // = 2
    prec_mult       // = 3
} precedence_t;

class Val;
class Env;

/* ---------------------------------------- */
/*                  Expr                    */
/* ---------------------------------------- */
class Expr {
public:
    /**
     * virtual destructors
     */
    virtual ~Expr(){ };

    /**
     * compare two expressions for equality
     * @param e input expression for comparing
     * @return true if two expressions equal, false otherwise
     */
    virtual bool equals(PTR(Expr) e) = 0;

    /**
     * returns Val* for the value of an expression
     * @param env
     * @return Val*
     */
    virtual PTR(Val) interp(PTR(Env) env) = 0;

    /**
     * is the expression a variable or contains a variable
     * @return true if the expression is a variable or contains a variable, false otherwise
     */
    virtual bool has_variable() = 0;

    /**
     * everywhere that the expression (whose subst method is called) contains a variable matching the string, the result Expr* should have the given replacement, instead.
     * @param s a std::string is substituted
     * @param e a new expression
     * @return Expr
     */
    virtual PTR(Expr) subst(std::string s, PTR(Expr) e) = 0;

    /**
     * print the expression
     * @param ot ostream
     */
    virtual void print(std::ostream &ot) = 0;

    /**
     * should be implemented in all sub classes and should call the print method
     * @return st.str(), the string gonna be printed
     */
    std::string to_string();

    /**
     * virtual pretty_print:
     * likes print, print the expression.
     * unlike print, pretty_print includes a space around + or *, and it avoids unnecessary parentheses by relying the usual precedence rules for multiplication and addition. It can also avoid parentheses by assuming that operators associate to the right
     * @param ot ostream
     */
    virtual void pretty_print(std::ostream &ot) = 0;

    /**
     * for pretty_print, should be implemented in all sub classes and should call the print method
     * @return st.str() string gonna be printed
     */
    std::string to_pretty_string();

    //Hint: You will likely find it useful to “accumulate” a precedence level for parentheses as an argument to a pretty_print_at helper, where the precedence indicates which operations need parentheses: “none,” “addition or lower,” or “multiplication or lower.”
    /**
     * helper function for pretty print
     * @param ot
     * @param p
     * @param isLeftInside
     * @param isNested
     * @param occupy
     */
    virtual void pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy) = 0;
};

/* ---------------------------------------- */
/*               NumExpr                    */
/* ---------------------------------------- */
class NumExpr : public Expr {
public:
    int rep;        // the rep of NumExpr

    /**
     * constructor
     * @param rep int rep
     */
    NumExpr(int rep);

    /**
     * if input Expr is a Num and the val equal
     * @param e input expression for comparing
     * @return true if e is NumExpr and e->val equals to this->val, false otherwise
     */
    bool equals(PTR(Expr) e);

    /**
     * returns Val* for the value of a number is the number
     * @param env
     * @return int
     */
    PTR(Val) interp(PTR(Env) env);

    /**
     * the number expression is not contains a variable
     * @return false
     */
    bool has_variable();

    /**
     * everywhere that the expression (whose subst method is called) contains a variable matching the string, the result Expr* should have the given replacement, instead.
     * @param s a std::string is substituted
     * @param e a new expression
     * @return Expr
     */
    PTR(Expr) subst(std::string s, PTR(Expr) e);

    /**
     * the val of NumExpr expression
     * @param ot ostream
     */
    void print(std::ostream &ot);

    /**
     * the val of NumExpr expression
     * @param ot ostream
     */
    void pretty_print(std::ostream &ot);

    /**
      * helper function for pretty print
      * @param ot
      * @param p
      * @param isLeftInside
      * @param isNested
      * @param occupy
      */
    void pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy);
};

/* ---------------------------------------- */
/*               AddExpr                    */
/* ---------------------------------------- */
class AddExpr : public Expr {
public:
    PTR(Expr) lhs;      // left hand side expression of AddExpr
    PTR(Expr) rhs;      // right hand side expression of AddExpr

    /**
     * constructor
     * @param lhs left hand side expression
     * @param rhs right hand side expression
     */
    AddExpr(PTR(Expr) lhs, PTR(Expr) rhs);

    /**
     * if input Expr is a Add and the lhs and rhs equal
     * @param e input expression for comparing
     * @return true if e is AddExpr and e->lhs equals to this->lhs and e->rhs equals this->rhs, otherwise false
     */
    bool equals(PTR(Expr) e);

    /**
     * returns Val* for the value of an addition expression is the sum of the subexpression values
     * @param env
     * @return int
     */
    PTR(Val) interp(PTR(Env) env);

    /**
     * the add expression may contains a variable
     * @return true if AddExpr contains a variable, otherwise false
     */
    bool has_variable();

    /**
     * everywhere that the expression (whose subst method is called) contains a variable matching the string, the result Expr* should have the given replacement, instead.
     * @param s a std::string is substituted
     * @param e a new expression
     * @return Expr
     */
    PTR(Expr) subst(std::string s, PTR(Expr) e);

    /**
     * addition expressions with an infix +
     * @param ot ostream
     */
    void print(std::ostream &ot);

    /**
     * includes a space around + or *, and it avoids unnecessary parentheses
     * by relying the usual precedence rules for multiplication and addition
     * by assuming that operators associate to the right
     * @param ot ostream
     */
    void pretty_print(std::ostream &ot);

    /**
     * helper function for pretty print
     * @param ot
     * @param p
     * @param isLeftInside
     * @param isNested
     * @param occupy
     */
    void pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy);
};

/* ---------------------------------------- */
/*              MultExpr                    */
/* ---------------------------------------- */
class MultExpr : public Expr {
public:
    PTR(Expr) lhs;      // left hand side expression of MultExpr
    PTR(Expr) rhs;      // right hand side expression of MultExpr

    /**
     * constructor
     * @param lhs left hand side expression
     * @param rhs right hand side expression
     */
    MultExpr(PTR(Expr) lhs, PTR(Expr) rhs);

    /**
     * if input Expr is a Mult and the lhs and rhs equal
     * @param e input expression for comparing
     * @return true if e is MultExpr and e->lhs equals to this->lhs and e->rhs equals this->rhs, otherwise false
     */
    bool equals(PTR(Expr) e);

    /**
     * returns Val* for the value of a multiplication expression is the product of the subexpression values
     * @param env
     * @return Val*
     */
    PTR(Val) interp(PTR(Env) env);

    /**
     * the mult expression may contains a variable
     * @return ture if MultExpr contains a variable, otherwise false
     */
    bool has_variable();

    /**
     * everywhere that the expression (whose subst method is called) contains a variable matching the string, the result Expr* should have the given replacement, instead.
     * @param s a std::string is substituted
     * @param e a new expression
     * @return Expr*
     */
    PTR(Expr) subst(std::string s, PTR(Expr) e);

    /**
     * multiplication expressions with an infix *
     * @param ot ostream
     */
    void print(std::ostream &ot);

    /**
     * includes a space around + or *, and it avoids unnecessary parentheses,
     * by relying the usual precedence rules for multiplication and addition,
     * by assuming that operators associate to the right
     * @param ot ostream
     */
    void pretty_print(std::ostream &ot);

    /**
     * helper function for pretty print
     * @param ot
     * @param p
     * @param isLeftInside
     * @param isNested
     * @param occupy
     */
    void pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy);
};

/* ---------------------------------------- */
/*               VarExpr                    */
/* ---------------------------------------- */
class VarExpr : public Expr {
public:
    std::string str;        // the string value of VarExpr

    /**
     * constructor
     * @param val string value
     */
    VarExpr(std::string str);

    /**
     * if input Expr is a VarExpr and val equal
     * @param e input expression for comparing
     * @return true if e is VarExpr and e->val equals to this->val return true, otherwise false
     */
    bool equals(PTR(Expr) e);


    /**
     * A variable has no value, so interp for a variable should throw a std::runtime_error exception
     * @param env
     * @throw std::runtime_error
     */
    PTR(Val) interp(PTR(Env) env);

    /**
     * the var expression is a variable
     * @return true
     */
    bool has_variable();

    /**
     * everywhere that the expression (whose subst method is called) contains a variable matching the string, the result Expr* should have the given replacement, instead.
     * @param s a std::string is substituted
     * @param e a new expression
     * @return Expr
     */
    PTR(Expr) subst(std::string s, PTR(Expr) e);

    /**
     * the val of VarExpr expression
     * @param ot ostream
     */
    void print(std::ostream &ot);

    /**
     * the val of VarExpr expression
     * @param ot ostream
     */
    void pretty_print(std::ostream &ot);

    /**
     * helper function for pretty print
     * @param ot
     * @param p
     * @param isLeftInside
     * @param isNested
     * @param occupy
     */
    void pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy);
};

/* ---------------------------------------- */
/*               LetExpr                    */
/* ---------------------------------------- */
class LetExpr : public Expr {
public:
    std::string name;    // left-hand side name
    PTR(Expr) rhs;          // right-hand side expression
    PTR(Expr) body;         // body expression (_in <expr>)

    /**
     * constructor
     * @param lhs left hand side expression
     * @param rhs right hand side expression
     */
    LetExpr(std::string name, PTR(Expr) rhs, PTR(Expr) body);

    /**
     * if input Expr is a LetExpr and the name, rhs and body equal
     * @param e input expression for comparing
     * @return true if e is LetExpr and e->name equals to this->name, e->rhs equals this->rhs and e->body equals this->body, otherwise false
     */
    bool equals(PTR(Expr) e);

    /**
     * returns Val* for the value
     * Interpreting _let can use subst
     * Interpret binding RHS before subst
     * @param env
     * @return Val*
     */
    PTR(Val) interp(PTR(Env) env);

    /**
     * the _let expression may contains a variable
     * @return true only if the right-hand side or body expression contains a variable, otherwise false
     */
    bool has_variable();

    /**
     * everywhere that the expression (whose subst method is called) contains a variable matching the string, the result Expr* should have the given replacement, instead.
     * @param s a std::string is substituted
     * @param e a new expression
     * @return Expr
     */
    PTR(Expr) subst(std::string s, PTR(Expr) e);

    /**
     * For print, a _let expression should always print with parentheses, using "(" before the "_let" and ")" after the "body"
     * No spaces should be around "=", and only a single space should appear after "_let" and before/after "_in"
     *
     * Here's an example of a correctly printed expression:
     * (_let x=5 _in ((_let y=3 _in (y+2))+x))
     * @param ot ostream
     */
    void print(std::ostream &ot);

    /**
     * The "=" in "_let" should have one space on either side
     * A _let needs parentheses when it is nested immediately as the right argument of an unparenthesized *
     * where _let would have needed parentheses in the surrounding context
     * (that is, if the _let used in place of the whole * would need parentheses,
     * then it still needs parentheses within the right-hand size of *).
     *
     * Here's an example of a correctly pretty-printed expression:
     * _let x = 5
     * _in  (_let y = 3
     *       _in  y + 2) + x
     * @param ot ostream
     */
    void pretty_print(std::ostream &ot);

    /**
     * helper function for pretty print
     * @param ot
     * @param p
     * @param isLeftInside
     * @param isNested
     * @param occupy
     */
    void pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy);
};

/* ---------------------------------------- */
/*               BoolExpr                   */
/* ---------------------------------------- */
class BoolExpr : public Expr {
public:
    bool var;

    /**
     * constructor
     * @param var
     */
    BoolExpr(bool var);

    /**
     * if input Expr is a BoolExpr and the var equal
     * @param e
     * @return true if e is BoolExpr and e->var equals to this->var, otherwise false
     */
    bool equals(PTR(Expr) e);

    /**
     * returns BoolVal for the value of a boolean expression is the var
     * @param env
     * @return Val*
     */
    PTR(Val) interp(PTR(Env) env);

    /**
     * the boolean expression has not variable
     * @return false
     */
    bool has_variable();

    /**
     * return this as Expr*
     * @param s
     * @param e
     * @return Expr*
     */
    PTR(Expr) subst(std::string s, PTR(Expr) e);

    /**
     * if true print "_true", else print "_false"
     * The print method should always put parentheses around a == or _if..._then..._else form, and never around _true or _false
     * @param ot
     */
    void print(std::ostream &ot);

    /**
     * if true print "_true", else print "_false"
     * @param ot
     */
    void pretty_print(std::ostream &ot);

    /**
     *
     * @param ot
     * @param p
     * @param isLeftInside
     * @param isNested
     * @param occupy
     */
    void pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy);
};

/* ---------------------------------------- */
/*                EqExpr                    */
/* ---------------------------------------- */
class EqExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;

    /**
     * constructor
     * @param lhs
     * @param rhs
     */
    EqExpr(PTR(Expr) lhs, PTR(Expr) rhs);

    /**
     * if input Expr is a EqExpr and lhs, rhs equal
     * @param e
     * @return true if e is EqExpr and e->lhs equals to this->lhs, e->rhs equals this->rhs, otherwise false
     */
    bool equals(PTR(Expr) e);

    /**
     * returns BoolVal for the value of a equality expression is the bool of lhs == rhs
     * @param env
     * @return Val*
     */
    PTR(Val) interp(PTR(Env) env);

    /**
     * the equality expression may contains a variable
     * @return ture if EqExpr contains a variable, otherwise false
     */
    bool has_variable();

    /**
     * everywhere that the expression (whose subst method is called) contains a variable matching the string, the result Expr* should have the given replacement, instead.
     * @param s
     * @param e
     * @return Expr*
     */
    PTR(Expr) subst(std::string s, PTR(Expr) e);

    /**
     * equality expressions with an infix ==
     * The print method should always put parentheses around a == or _if..._then..._else form, and never around _true or _false
     * @param ot
     */
    void print(std::ostream &ot);

    /**
     *
     * @param ot
     */
    void pretty_print(std::ostream &ot);

    /**
     *
     * @param ot
     * @param p
     * @param isLeftInside
     * @param isNested
     * @param occupy
     */
    void pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy);
};

/* ---------------------------------------- */
/*                IfExpr                    */
/* ---------------------------------------- */
class IfExpr : public Expr {
public:
    PTR(Expr) test_part;
    PTR(Expr) then_part;
    PTR(Expr) else_part;

    /**
     * constructor
     * @param if_e
     * @param then_e
     * @param else_e
     */
    IfExpr(PTR(Expr) test_part, PTR(Expr) then_part, PTR(Expr) else_part);

    /**
     * if input Expr is a IfExpr and test_part, then_part and else_part equal
     * @param e
     * @return true if e is IfExpr and e->test_part equals to this->test_part, e->then_part equals to this->then_part and e->else_part equals to this->else_part, otherwise false
     */
    bool equals(PTR(Expr) e);

    /**
     * returns Val for the value of a conditional expression
     * if test_part->is_true() true, return then_part->interp(), else return else_part->interp()
     * @param env
     * @return Val*
     */
    PTR(Val) interp(PTR(Env) env);

    /**
     * the conditional expression may contains a variable
     * @return ture if IfExpr contains a variable, otherwise false
     */
    bool has_variable();

    /**
     * everywhere that the expression (whose subst method is called) contains a variable matching the string, the result Expr* should have the given replacement, instead.
     * @param s
     * @param e
     * @return
     */
    PTR(Expr) subst(std::string s, PTR(Expr) e);

    /**
     * The print method should always put parentheses around a == or _if..._then..._else form, and never around _true or _false
     * @param ot
     */
    void print(std::ostream &ot);

    /**
     *
     * @param ot
     */
    void pretty_print(std::ostream &ot);

    /**
     *
     * @param ot
     * @param p
     * @param isLeftInside
     * @param isNested
     * @param occupy
     */
    void pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy);
};

/* ---------------------------------------- */
/*                FunExpr                   */
/* ---------------------------------------- */
class FunExpr : public Expr {
public:
    std::string formal_arg;
    PTR(Expr) body;

    /**
     * constructor
     * @param formal_arg variable
     * @param body an Expression inside a Value
     */
    FunExpr(std::string formal_arg, PTR(Expr) body);

    /**
     * if input Expr is a FunExpr and the formal_arg and body equal
     * @param e input expression for comparing
     * @return true if e is FunExpr and e->formal_arg equals to this->formal_arg, e->body equals this->body, otherwise false
     */
    bool equals(PTR(Expr) e);

    /**
     * returns FunVal* for the value
     * @param env
     * @return Val*
     */
    PTR(Val) interp(PTR(Env) env);

    /**
     * the _fun expression contains a variable
     * @return true
     */
    bool has_variable();

    /**
     * everywhere that the expression (whose subst method is called) contains a variable matching the string, the result Expr* should have the given replacement, instead.
     * @param s a std::string is substituted
     * @param e a new expression
     * @return Expr
     */
    PTR(Expr) subst(std::string s, PTR(Expr) e);

    /**
     * For print, always put a _fun expression in parentheses,
     * include exactly one space around the outside of the parentheses for the argument,
     * and include no space inside the parentheses,
     *
     * @param ot ostream
     */
    void print(std::ostream &ot);

    /**
     * For pretty_print, if you have it,
     * _fun expressions need parentheses in the same contexts where _let would need parentheses.
     * Add a newline after the argument parentheses for _fun and indent the body two spaces more than the column where _fun starts.
     * A function call never needs extra parentheses. As an example, the implementation of factorial below is correctly pretty-printed.
     *
     * Here's an example of a correctly pretty-printed expression:
     * _let y = 8
     * _in _let f = _fun (x) x*y
     *     _in f(2)
     * @param ot ostream
     */
    void pretty_print(std::ostream &ot);

    /**
     * helper function for pretty print
     * @param ot
     * @param p
     * @param isLeftInside
     * @param isNested
     * @param occupy
     */
    void pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy);
};

/* ---------------------------------------- */
/*                CallExpr                  */
/* ---------------------------------------- */

class CallExpr : public Expr {
public:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;

    /**
     * constructor
     * @param lhs left hand side expression
     * @param rhs right hand side expression
     */
    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);

    /**
     * if input Expr is a CallExpr and the to_be_called, rhs and actual_arg equal
     * @param e input expression for comparing
     * @return true if e is CallExpr and e->to_be_called equals to this->to_be_called, e->actual_arg equals this->actual_arg, otherwise false
     */
    bool equals(PTR(Expr) e);

    /**
     * returns Val* for the value
     * @param env
     * @return Val*
     */
    PTR(Val) interp(PTR(Env) env);

    /**
     * the call expression may contains a variable
     * @return true
     */
    bool has_variable();

    /**
     * everywhere that the expression (whose subst method is called) contains a variable matching the string, the result Expr* should have the given replacement, instead.
     * @param s a std::string is substituted
     * @param e a new expression
     * @return Expr
     */
    PTR(Expr) subst(std::string s, PTR(Expr) e);

    /**
     * Never print a pair of parentheses around a function call (because it turns out that's not necessary),
     * and include no space on the outside or inside edges of the parentheses around the argument in a function call.
     * @param ot ostream
     */
    void print(std::ostream &ot);

    /**
     *
     * @param ot ostream
     */
    void pretty_print(std::ostream &ot);

    /**
     * helper function for pretty print
     * @param ot
     * @param p
     * @param isLeftInside
     * @param isNested
     * @param occupy
     */
    void pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy);
};

#endif /* expr_h */
