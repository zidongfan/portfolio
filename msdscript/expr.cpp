/**
 * @file expr.cpp
 * @brief contains all constructions and methods in expr class
 *
 * @author Zidong Fan
 * @date 01/20/2023
 */

#include <iostream>
#include "expr.h"
#include "val.h"
#include "env.h"

/* ---------------------------------------- */
/*                  Expr                    */
/* ---------------------------------------- */
std::string Expr::to_string() {
    std::stringstream st("");
    this->print(st);
    return st.str();
}

std::string Expr::to_pretty_string() {
    std::stringstream st("");
    this->pretty_print(st);
    return st.str();
}

/* ---------------------------------------- */
/*               NumExpr                    */
/* ---------------------------------------- */
NumExpr::NumExpr(int rep) {
    this->rep = rep;
}

bool NumExpr::equals(PTR(Expr) e) {
    PTR(NumExpr) n = CAST(NumExpr)(e);
    if (n == NULL)
        return false;
    else
        return this->rep == n->rep;
}

PTR(Val)NumExpr::interp(PTR(Env) env) {
    return NEW(NumVal)(rep);
}

bool NumExpr::has_variable() {
    return false;
}

PTR(Expr)NumExpr::subst(std::string s, PTR(Expr) e) {
    return NEW(NumExpr)(this->rep);
}

void NumExpr::print(std::ostream &ot) {
    ot << rep;
}

void NumExpr::pretty_print(std::ostream &ot) {
    pretty_print_at(ot, prec_none, false, false, 0);
}

void NumExpr::pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy) {
    ot << rep;
}

/* ---------------------------------------- */
/*               AddExpr                    */
/* ---------------------------------------- */
AddExpr::AddExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool AddExpr::equals(PTR(Expr) e) {
    PTR(AddExpr) a = CAST(AddExpr)(e);
    if (a == NULL)
        return false;
    else
        return this->lhs->equals(a->lhs)
               && this->rhs->equals(a->rhs);
}

PTR(Val)AddExpr::interp(PTR(Env) env) {
    return this->lhs->interp(env)->add_to(this->rhs->interp(env));
}

bool AddExpr::has_variable() {
    return this->lhs->has_variable()
           || this->lhs->has_variable();
}

PTR(Expr)AddExpr::subst(std::string s, PTR(Expr) e) {
    return NEW(AddExpr)(this->lhs->subst(s, e), this->rhs->subst(s, e));
}

void AddExpr::print(std::ostream &ot) {
    ot << "("
       << lhs->to_string()
       << "+"
       << rhs->to_string()
       << ")";
}

void AddExpr::pretty_print(std::ostream &ot) {
    pretty_print_at(ot, prec_none, false, false, 0);
}

void AddExpr::pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy) {
    long begin = ot.tellp();
    if (p < 2 || (p == 2 && !isLeftInside)) {
        this->lhs->pretty_print_at(ot, prec_add, true, true, occupy);
        ot << " + ";
        long end1 = ot.tellp();
        this->rhs->pretty_print_at(ot, prec_add, false, isNested, occupy + (end1 - begin));
    } else {
        ot << "(";
        long end2 = ot.tellp();
        this->lhs->pretty_print_at(ot, prec_add, true, true, occupy + (int) (end2 - begin));
        ot << " + ";
        long end3 = ot.tellp();
        this->rhs->pretty_print_at(ot, prec_add, false, isNested, occupy + (int) (end3 - begin));
        ot << ")";
    }
}

/* ---------------------------------------- */
/*              MultExpr                    */
/* ---------------------------------------- */
MultExpr::MultExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
};

bool MultExpr::equals(PTR(Expr) e) {
    PTR(MultExpr) m = CAST(MultExpr)(e);
    if (m == NULL)
        return false;
    else
        return this->lhs->equals(m->lhs)
               && this->rhs->equals(m->rhs);
}

PTR(Val)MultExpr::interp(PTR(Env) env) {
    return this->lhs->interp(env)->mult_with(this->rhs->interp(env));
}

bool MultExpr::has_variable() {
    return this->lhs->has_variable()
           || this->lhs->has_variable();
}

PTR(Expr)MultExpr::subst(std::string s, PTR(Expr) e) {
    return NEW(MultExpr)(this->lhs->subst(s, e), this->rhs->subst(s, e));
}

void MultExpr::print(std::ostream &ot) {
    ot << "("
       << lhs->to_string()
       << "*"
       << rhs->to_string()
       << ")";
}

void MultExpr::pretty_print(std::ostream &ot) {
    pretty_print_at(ot, prec_none, false, false, 0);
}

void MultExpr::pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy) {
    //opposite to dfs we use true/false and accumulator to identify l/r side of the mult
    //from top to bottom -> get the right answer
    long begin = ot.tellp();
    if (isLeftInside && p == 3) {
        ot << "(";
        long end1 = ot.tellp();
        lhs->pretty_print_at(ot, prec_mult, true, true, occupy + (int) (end1 - begin));
        ot << " * ";
        long end2 = ot.tellp();
        rhs->pretty_print_at(ot, prec_mult, false, isNested, occupy + (int) (end2 - begin));
        ot << ")";
    } else {
        lhs->pretty_print_at(ot, prec_mult, true, true, occupy);
        ot << " * ";
        long end3 = ot.tellp();
        rhs->pretty_print_at(ot, prec_mult, false, isNested, occupy + (int) (end3 - begin));
    }
}

/* ---------------------------------------- */
/*               VarExpr                    */
/* ---------------------------------------- */
VarExpr::VarExpr(std::string str) {
    this->str = str;
}

bool VarExpr::equals(PTR(Expr) e) {
    PTR(VarExpr) v = CAST(VarExpr)(e);
    if (v == NULL)
        return false;
    else
        return this->str == v->str;
}

PTR(Val)VarExpr::interp(PTR(Env) env) {
//    throw std::runtime_error("no value for variable");
    return env->lookup(str);
}

bool VarExpr::has_variable() {
    return true;
}

PTR(Expr)VarExpr::subst(std::string s, PTR(Expr) e) {
    if (s == this->str)
        return e;
    return NEW(VarExpr)(this->str);
}

void VarExpr::print(std::ostream &ot) {
    ot << str;
}

void VarExpr::pretty_print(std::ostream &ot) {
    pretty_print_at(ot, prec_none, false, false, 0);
}

void VarExpr::pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy) {
    ot << str;
}

/* ---------------------------------------- */
/*               LetExpr                    */
/* ---------------------------------------- */
LetExpr::LetExpr(std::string name, PTR(Expr) rhs, PTR(Expr) body) {
    this->name = name;
    this->rhs = rhs;
    this->body = body;
}

bool LetExpr::equals(PTR(Expr) e) {
    PTR(LetExpr) l = CAST(LetExpr)(e);
    if (l == NULL)
        return false;
    else
        return this->name == l->name
               && this->rhs->equals(l->rhs)
               && this->body->equals(l->body);
}

PTR(Val)LetExpr::interp(PTR(Env) env) {
    PTR(Val) rhs_val = rhs->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(name, rhs_val, env);
    return body->interp(new_env);
}

bool LetExpr::has_variable() {
    return this->rhs->has_variable()
           || this->body->has_variable();
}

PTR(Expr)LetExpr::subst(std::string s, PTR(Expr) e) {
    if (s != this->name)
        return NEW(LetExpr)(this->name, this->rhs->subst(s, e), this->body->subst(s, e));
    return NEW(LetExpr)(this->name, this->rhs->subst(s, e), this->body);
}

void LetExpr::print(std::ostream &ot) {
    ot << "(_let "
       << this->name
       << "="
       << rhs->to_string()
       << " _in "
       << this->body->to_string()
       << ")";
}

void LetExpr::pretty_print(std::ostream &ot) {
    pretty_print_at(ot, prec_let, false, false, 0);
}

void LetExpr::pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy) {
    PTR(AddExpr) a = CAST(AddExpr)(body);
    long begin_ = ot.tellp();

    if ((p > 1 && isLeftInside) || isNested) {

        ot << "(_let "
           << this->name
           << " = ";
        long end_p1 = ot.tellp();

        this->rhs->pretty_print_at(ot, p, false, false, occupy + (int) (end_p1 - begin_));

        ot << "\n";
        for (int i = 0; i < occupy + 1; i++) {
            ot << " ";
        }
        ot << "_in  ";

        if (a != NULL)
            this->body->pretty_print_at(ot, prec_add, false, false, occupy + 5);
        else
            this->body->pretty_print_at(ot, p, false, false, occupy + 5);

        ot << ")";
    } else {
        ot << "_let "
           << this->name
           << " = ";
        long end_p2 = ot.tellp();

        this->rhs->pretty_print_at(ot, p, false, false, occupy + (int) (end_p2 - begin_));

        ot << "\n";
        for (int i = 0; i < occupy; i++) {
            ot << " ";
        }
        ot << "_in  ";

        if (a != NULL)
            this->body->pretty_print_at(ot, prec_add, false, false, occupy + 4);
        else
            this->body->pretty_print_at(ot, p, false, false, occupy + 4);
    }
}

/* ---------------------------------------- */
/*               BoolExpr                   */
/* ---------------------------------------- */
BoolExpr::BoolExpr(bool var) {
    this->var = var;
}

bool BoolExpr::equals(PTR(Expr) e) {
    PTR(BoolExpr) b = CAST(BoolExpr)(e);
    if (b == nullptr)
        return false;
    else
        return b->var == this->var;
}

PTR(Val)BoolExpr::interp(PTR(Env) env) {
    return NEW(BoolVal)(this->var);
}

bool BoolExpr::has_variable() {
    return false;
}

PTR(Expr)BoolExpr::subst(std::string s, PTR(Expr) e) {
    PTR(Expr) res = NEW(BoolExpr)(this->var);
    return res;
}

void BoolExpr::print(std::ostream &ot) {
    if (var)
        ot << "_true";
    else
        ot << "_false";
}

void BoolExpr::pretty_print(std::ostream &ot) {
    if (var)
        ot << "_true";
    else
        ot << "_false";
}

void BoolExpr::pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy) {

}

/* ---------------------------------------- */
/*                EqExpr                    */
/* ---------------------------------------- */
EqExpr::EqExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqExpr::equals(PTR(Expr) e) {
    PTR(EqExpr) eq = CAST(EqExpr)(e);
    if (eq == nullptr)
        return false;
    else
        return (this->lhs->equals(eq->lhs) &&
                this->rhs->equals(eq->rhs));
}

PTR(Val)EqExpr::interp(PTR(Env) env) {
    PTR(Val) val_lhs = lhs->interp(env);
    PTR(Val) val_rhs = rhs->interp(env);
    return NEW(BoolVal)(val_lhs->equals(val_rhs));
}

bool EqExpr::has_variable() {
    return (this->lhs->has_variable() ||
            this->rhs->has_variable());;
}

PTR(Expr)EqExpr::subst(std::string s, PTR(Expr) e) {
    return NEW(EqExpr)(this->lhs->subst(s, e), this->rhs->subst(s, e));
}

void EqExpr::print(std::ostream &ot) {
    ot << "("
       << lhs->to_string()
       << "=="
       << rhs->to_string()
       << ")";
}

void EqExpr::pretty_print(std::ostream &ot) {

}

void EqExpr::pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy) {

}

/* ---------------------------------------- */
/*                IfExpr                    */
/* ---------------------------------------- */
IfExpr::IfExpr(PTR(Expr) test_part, PTR(Expr) then_part, PTR(Expr) else_part) {
    this->test_part = test_part;
    this->then_part = then_part;
    this->else_part = else_part;
}

bool IfExpr::equals(PTR(Expr) e) {
    PTR(IfExpr) ife = CAST(IfExpr)(e);
    if (ife == nullptr)
        return false;
    else
        return (this->test_part->equals(ife->test_part) &&
                this->then_part->equals(ife->then_part) &&
                this->else_part->equals(ife->else_part));
}

PTR(Val)IfExpr::interp(PTR(Env) env) {
    if (this->test_part->interp(env)->is_true())
        return this->then_part->interp(env);
    else
        return this->else_part->interp(env);
}

bool IfExpr::has_variable() {
    return false;
}

PTR(Expr)IfExpr::subst(std::string s, PTR(Expr) e) {
    return NEW(IfExpr)(this->test_part->subst(s, e), this->then_part->subst(s, e), this->else_part->subst(s, e));
}

void IfExpr::print(std::ostream &ot) {
    ot << "(_if " << this->test_part->to_string()
       << " _then " << this->then_part->to_string()
       << " _else " << this->else_part->to_string() << ")";
}

void IfExpr::pretty_print(std::ostream &ot) {

}

void IfExpr::pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy) {

}

/* ---------------------------------------- */
/*                FunExpr                   */
/* ---------------------------------------- */
FunExpr::FunExpr(std::string formal_arg, PTR(Expr) body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals(PTR(Expr) e) {
    PTR(FunExpr) f = CAST(FunExpr)(e);
    if (f == NULL)
        return false;
    else
        return this->formal_arg == f->formal_arg
               && this->body->equals(f->body);
}

PTR(Val)FunExpr::interp(PTR(Env) env) {
    return NEW(FunVal)(formal_arg, body, env);
}

bool FunExpr::has_variable() {
    return true;
}

PTR(Expr)FunExpr::subst(std::string s, PTR(Expr) e) {
    if (s == this->formal_arg)
        return NEW(FunExpr)(formal_arg, body);
    return NEW(FunExpr)(formal_arg, body->subst(s, e));
}

void FunExpr::print(std::ostream &ot) {
    ot << "(_fun(" << this->formal_arg
       << ") "
       << this->body->to_string()
       << ")";
}

void FunExpr::pretty_print(std::ostream &ot) {

}

void FunExpr::pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy) {

}

/* ---------------------------------------- */
/*                CallExpr                  */
/* ---------------------------------------- */

CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

bool CallExpr::equals(PTR(Expr) e) {
    PTR(CallExpr) c = CAST(CallExpr)(e);
    if (c == NULL)
        return false;
    else
        return this->to_be_called->equals(c->to_be_called)
               && this->actual_arg->equals(c->actual_arg);
}

PTR(Val)CallExpr::interp(PTR(Env) env) {
    return to_be_called->interp(env)->call(actual_arg->interp(env));
}

bool CallExpr::has_variable() {
    return true;
}

PTR(Expr) CallExpr::subst(std::string s, PTR(Expr) e) {
    return NEW(CallExpr)(to_be_called->subst(s, e), actual_arg->subst(s, e));
}

void CallExpr::print(std::ostream &ot) {
    ot << to_be_called->to_string()
       << "("
       << actual_arg->to_string()
       << ")";
}

void CallExpr::pretty_print(std::ostream &ot) {

}

void CallExpr::pretty_print_at(std::ostream &ot, precedence_t p, bool isLeftInside, bool isNested, int occupy) {

}
