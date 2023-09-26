/**
 * @file parse.h
 * @brief
 *
 * <expr>      = <comparg>
 *             | <comparg> == <expr>
 * <comparg>   = <addend>
 *             | <addend> + <comparg>
 * <addend>    = <multicand>
 *             | <multicand> * <addend>
 * <multicand> = <inner>
 *             | <multicand> ( <expr> )
 * <inner> = <number>
 *             | ( <expr> )
 *             | <variable>
 *             | _let <variable> = <expr> _in <expr>
 *             | _true
 *             | _false
 *             | _if <expr> _then <expr> _else <expr>
 *             | _fun ( <variable> ) <expr>
 *
 * @author Zidong Fan
 * @date 02/16/2023
 */

#ifndef MSDSCRIPT_PARSE_H
#define MSDSCRIPT_PARSE_H

#include "expr.h"

/* ---------------------------------------- */
/*            parsing recursions            */
/* ---------------------------------------- */
/**
 * function to call from main
 * @param in
 * @return Expr*
 */
PTR(Expr)parse(std::istream &in);

 /**
  * parsing expression
  * @param in
  * @return Expr*
  */
PTR(Expr)parse_expr(std::istream &in);

/**
 * parsing comparg
 * @param in
 * @return Expr*
 */
PTR(Expr)parse_comparg(std::istream &in);

/**
 * parsing addition
 * @param in
 * @return Expr*
 */
PTR(Expr)parse_addend(std::istream &in);

/**
 * parsing multiplication
 * @param in
 * @return Expr*
 */
PTR(Expr)parse_multicand(std::istream &in);

/**
 * parsing inner
 * @param in
 * @return
 */
PTR(Expr)parse_inner(std::istream &in);



/* ---------------------------------------- */
/*              parsing helpers             */
/* ---------------------------------------- */
/**
 * parsing number
 * @param in
 * @return NumExpr
 */
PTR(Expr)parse_num(std::istream &in);

/**
 * parsing variable
 * @param in
 * @return VarExpr
 */
PTR(Expr)parse_var(std::istream &in);

/**
 * parsing let
 * @param in
 * @return LetExpr
 */
PTR(Expr)parse_let(std::istream &in);

/**
 * parsing if
 * @param in
 * @return IfExpr
 */
PTR(Expr)parse_if(std::istream &in);

/**
 * parsing fun
 * @param in
 * @return
 */
PTR(Expr)parse_fun(std::istream &in);

/**
 * parsing keyword
 * @param in
 * @return std::string
 */
std::string parse_keyword(std::istream &in);

/* ---------------------------------------- */
/*                utilities                 */
/* ---------------------------------------- */
/**
 * check and consume next char
 * if next char is expect consume it
 * else throw runtime_error
 * @param in
 * @param expect
 */
void consume(std::istream &in, int expect);

/**
 * ignoring whitespace
 * consume all white space char until next char is not white space
 * @param in
 */
void skip_whitespace(std::istream &in);

/**
 * wrapper for testing
 * @param s
 * @return
 */
PTR(Expr)parse_str(std::string s);

#endif //MSDSCRIPT_PARSE_H
