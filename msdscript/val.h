/**
 * @file Val.h
 * @brief
 *
 * <val> = <number>
 *       | <boolean>
 *
 * @author Zidong Fan
 * @date 03/10/2023
 */

#ifndef MSDSCRIPT_VAL_H
#define MSDSCRIPT_VAL_H


#include <string>
#include "pointer.h"

class Expr;
class Env;

/* ---------------------------------------- */
/*                  Val                     */
/* ---------------------------------------- */
class Val {
public:
    /**
     * virtual destructors
     */
    virtual ~Val(){ };

    /**
     * equals
     * @param input
     * @return true if equals, else return false
     */
    virtual bool equals(PTR(Val) other_val) = 0;

    /**
     * Val to Expr
     * @return Expr*
     */
    virtual PTR(Expr) to_expr() = 0;

    /**
     * to deal with adding values
     * @param input
     * @return Val*
     */
    virtual PTR(Val) add_to(PTR(Val) other_val) = 0;

    /**
     * to deal with multiplication values
     * @param input
     * @return Val*
     */
    virtual PTR(Val) mult_with(PTR(Val) other_val) = 0;

    /**
     * Val to string
     * @return std::string
     */
    virtual std::string to_string() = 0;

    /**
     * throws an exception for NumVal
     * @return bool
     */
    virtual bool is_true() = 0;

    /**
     * call function
     * @param actual_arg
     * @return
     */
    virtual PTR(Val) call(PTR(Val) actual_arg) = 0;
};

/* ---------------------------------------- */
/*                 NumVal                   */
/* ---------------------------------------- */
class NumVal : public Val {
public:
    int rep;        // the value of NumVal

    /**
     * constructor
     * @param rep
     */
    NumVal(int rep);

    /**
     * equals
     * @param other_val
     * @return true if equals, else return false
     */
    bool equals(PTR(Val) other_val);

    /**
     * NumVal to Expr
     * @return Expr*
     */
    PTR(Expr) to_expr();

    /**
     * to deal with adding number values
     * @param other_val
     * @return Val*
     */
    PTR(Val) add_to(PTR(Val) other_val);

    /**
     * to deal with multiplication number values
     * @param other_val
     * @return Val*
     */
    PTR(Val) mult_with(PTR(Val) other_val);

    /**
     * NumVal to string
     * @return std::string
     */
    std::string to_string();

    /**
    * throws an exception for NumVal
    * @return std::runtime_error
    */
    bool is_true();

    /**
     * call function
     * @param actual_arg
     * @return
     */
    PTR(Val) call(PTR(Val) actual_arg);
};

/* ---------------------------------------- */
/*                 BoolVal                  */
/* ---------------------------------------- */
class BoolVal : public Val {
public:
    bool _bool;     // the value of BoolVal

    /**
     * constructor
     * @param input
     */
    BoolVal(bool input);

    /**
     * equals
     * @param other_val
     * @return bool
     */
    bool equals(PTR(Val) other_val);

    /**
     * BoolVal to Expr
     * @return Expr*
     */
    PTR(Expr) to_expr();

    /**
     * to deal with adding bool values
     * @param other_val
     * @return std::runtime_error
     */
    PTR(Val) add_to(PTR(Val) other_val);

    /**
     * to deal with multiplication bool values
     * @param other_val
     * @return std::runtime_error
     */
    PTR(Val) mult_with(PTR(Val) other_val);

    /**
     * BoolVal to string
     * @return return _true if _bool is true, else return _false
     */
    std::string to_string();

    /**
     * bool is_true
     * @return true if _bool is true, else return false
     */
    bool is_true();

    /**
     * call function
     * @param actual_arg
     * @return
     */
    PTR(Val) call(PTR(Val) actual_arg);
};

/* ---------------------------------------- */
/*                 FunVal                   */
/* ---------------------------------------- */
class FunVal : public Val {
public:
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;

    /**
     * constructor
     * @param input
     */
    FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env);

    /**
     * equals
     * @param other_val
     * @return bool
     */
    bool equals(PTR(Val) other_val);

    /**
     * FunVal to Expr
     * @return Expr*
     */
    PTR(Expr) to_expr();

    /**
     * to deal with adding bool values
     * @param other_val
     * @return std::runtime_error
     */
    PTR(Val) add_to(PTR(Val) other_val);

    /**
     * to deal with multiplication bool values
     * @param other_val
     * @return std::runtime_error
     */
    PTR(Val) mult_with(PTR(Val) other_val);

    /**
     * FunVal to string
     * @return return _true if _bool is true, else return _false
     */
    std::string to_string();

    /**
    * bool is_true
    * @return true if _bool is true, else return false
    */
    bool is_true();

    /**
     * call function
     * @param actual_arg
     * @return
     */
    PTR(Val) call(PTR(Val) actual_arg);
};


#endif //MSDSCRIPT_VAL_H
