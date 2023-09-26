/**
 * @file val.cpp
 * @brief
 *
 * @author Zidong Fan
 * @date 03/10/2023
 */

#include "val.h"
#include "expr.h"
#include "env.h"


/* ---------------------------------------- */
/*                 NumVal                   */
/* ---------------------------------------- */
NumVal::NumVal(int val) {
    this->rep = val;
}

bool NumVal::equals(PTR(Val) other_val) {
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if (other_num == NULL)
        return (NEW(BoolVal)(false))->_bool;
    else
        return (NEW(BoolVal)((this->rep == other_num->rep)))->_bool;
}

PTR(Expr) NumVal::to_expr() {
    return NEW(NumExpr)(rep);
}

PTR(Val) NumVal::add_to(PTR(Val) other_val) {
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if (other_num == NULL)
        throw std::runtime_error("add of non-number");
    else
        return NEW(NumVal)((unsigned) this->rep + (unsigned) other_num->rep);
}

PTR(Val) NumVal::mult_with(PTR(Val) other_val) {
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if (other_num == NULL)
        throw std::runtime_error("mult of non-number");
    else
        return NEW(NumVal)((unsigned) this->rep * (unsigned) other_num->rep);
}


std::string NumVal::to_string() {
    return std::to_string(rep);
}

bool NumVal::is_true() {
    throw std::runtime_error("is_true invalid");
}

PTR(Val) NumVal::call(PTR(Val) actual_arg) {
    throw std::runtime_error("call number invalid");
}

/* ---------------------------------------- */
/*                 BoolVal                  */
/* ---------------------------------------- */

BoolVal::BoolVal(bool input) {
    this->_bool = input;
}

bool BoolVal::equals(PTR(Val) val) {
    PTR(BoolVal) boolVal = CAST(BoolVal)(val);
    if (boolVal == NULL)
        return false;
    else
        return this->_bool == boolVal->_bool;
}

PTR(Expr) BoolVal::to_expr() {
    return nullptr;
}

PTR(Val) BoolVal::add_to(PTR(Val) other_val) {
    throw std::runtime_error("add of non-number");
}

PTR(Val) BoolVal::mult_with(PTR(Val) input) {
    throw std::runtime_error("mult of non-number");
}

std::string BoolVal::to_string() {
    if (_bool) return "_true";
    else return "_false";
}

bool BoolVal::is_true() {
    return this->_bool;
}

PTR(Val) BoolVal::call(PTR(Val) actual_arg) {
    throw std::runtime_error("call boolean invalid");
}

/* ---------------------------------------- */
/*                 FunVal                   */
/* ---------------------------------------- */
FunVal::FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env) {
    this->formal_arg = formal_arg;
    this->body = body;
    this->env = env;
}

bool FunVal::equals(PTR(Val) other_val) {
    PTR(FunVal) other_fun = CAST(FunVal)(other_val);
    if (other_fun == NULL)
        return false;
    else
        return (this->formal_arg == other_fun->formal_arg
                && this->body == other_fun->body);
}

PTR(Expr) FunVal::to_expr() {
    return NEW(FunExpr)(this->formal_arg, this->body);
}

PTR(Val) FunVal::add_to(PTR(Val) other_val) {
    throw std::runtime_error("add of non-number");
}

PTR(Val) FunVal::mult_with(PTR(Val) other_val) {
    throw std::runtime_error("mult of non-number");
}

std::string FunVal::to_string() {
    return "_fun (" + this->formal_arg + ") " + this->body->to_string();
}

bool FunVal::is_true() {
    throw std::runtime_error("is_true makes no sense");
}

PTR(Val) FunVal::call(PTR(Val) actual_arg) {
    return body->interp(NEW(ExtendedEnv)(formal_arg, actual_arg, env));
//    return this->body->subst(formal_arg, actual_arg->to_expr())->interp();
}
