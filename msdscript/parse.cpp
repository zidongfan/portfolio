/**
 * @file parse.cpp
 * @brief
 *
 * @author Zidong Fan
 * @date 02/16/2023
 */

#include "parse.h"

/* ---------------------------------------- */
/*            parsing recursions            */
/* ---------------------------------------- */
PTR(Expr)parse(std::istream &in) {
    PTR(Expr)e = parse_expr(in);
    skip_whitespace(in);

    char c = in.get();
    if (!in.eof())
        throw std::runtime_error((std::string) "end of file at " + c);
    return e;
}

PTR(Expr)parse_expr(std::istream &in) {
    PTR(Expr)e = parse_comparg(in);
    skip_whitespace(in);

    int c = in.peek();
    if (c == '=') {
        consume(in, '=');

        std::string str = "=";
        while (true) {
            char ch = in.peek();
            if (isalpha(ch) || isxdigit(ch) || isspace(ch))
                break;
            str += in.get();
        }

        if (str == "==") {
            PTR(Expr)res = parse_expr(in);
            e = NEW(EqExpr)(e, res);
        }
    }
    return e;
}

PTR(Expr)parse_comparg(std::istream &in) {
    PTR(Expr)e = parse_addend(in);
    skip_whitespace(in);

    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        PTR(Expr)rhs = parse_comparg(in);
        return NEW(AddExpr)(e, rhs);
    } else
        return e;
}

PTR(Expr)parse_addend(std::istream &in) {
    PTR(Expr)e = parse_multicand(in);
    skip_whitespace(in);

    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        PTR(Expr)rhs = parse_addend(in);
        return NEW(MultExpr)(e, rhs);
    } else
        return e;
}

PTR(Expr)parse_multicand(std::istream &in) {
    PTR(Expr)e = parse_inner(in);
    skip_whitespace(in);

    while (in.peek() == '(') {
        consume(in, '(');
        PTR(Expr)actual_arg = parse_expr(in);
        consume(in, ')');
        e = NEW(CallExpr)(e, actual_arg);
    }
    return e;
}

PTR(Expr)parse_inner(std::istream &in) {
    skip_whitespace(in);

    int c = in.peek();

    if ((c == '-') || isdigit(c))       // <number>
        return parse_num(in);
    else if (c == '(') {                // ( <expr> )
        consume(in, '(');
        PTR(Expr)e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')')
            throw std::runtime_error("missing close parenthesis");
        return e;
    } else if (isalpha(c))              // <variable>
        return parse_var(in);
    else if (c == '_') {                // _let <variable> = <expr> _in <expr>
        std::string kw = parse_keyword(in);
        if (kw == "_let")
            return parse_let(in);
        else if (kw == "_in")
            return parse_expr(in);
        else if (kw == "_true")        // _true
            return NEW(BoolExpr)(true);
        else if (kw == "_false")       // _false
            return NEW(BoolExpr)(false);
        else if (kw == "_if")          // _if <expr> _then <expr> _else <expr>
            return parse_if(in);
        else if (kw == "_fun")         // _fun ( <variable> ) <expr>
            return parse_fun(in);
        else
            throw std::runtime_error("invalid input");
    } else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}


/* ---------------------------------------- */
/*              parsing helpers             */
/* ---------------------------------------- */
PTR(Expr)parse_num(std::istream &in) {
    long double n = 0;
    bool negative = false;

    // in.peek(): like in.get(), but leaves character in stream
    if (in.peek() == '-') {
        negative = true;
        consume(in, '-'); // check and consume '-'
    }

    if (!isdigit(in.peek()))
        throw std::runtime_error("invalid input");

    while (1) {
        int c = in.peek();
        if (isdigit(c)) {
            consume(in, c);
            if (n <= INT_MAX && n >= INT_MIN)
                n = n * 10 + (c - '0');
            else
                throw std::runtime_error("input number constant is too big");
        } else
            break;
    }

    if (negative)
        n = -n;

    // General parsing strategy: peek to decide, then maybe consume
    return NEW(NumExpr)(n);
}

PTR(Expr)parse_var(std::istream &in) {
    std::string s = "";
    while (1) {
        int c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            s += c;
        } else
            break;
    }
    return NEW(VarExpr)(s);
}

std::string parse_keyword(std::istream &in) {
    consume(in, '_');
    std::string kw = "_";
    while (1) {
        int c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            kw += c;
        } else
            break;
    }
    return kw;
}

PTR(Expr)parse_let(std::istream &in) {
    skip_whitespace(in);                // _let: consumed by parse_keyword

    PTR(Expr)var = parse_var(in);          // <variable>
    std::string name = var->to_string();

    skip_whitespace(in);
    consume(in, '=');           // =
    skip_whitespace(in);

    PTR(Expr)rhs = parse_expr(in);        // <expr>
    PTR(Expr)body = parse_expr(in);       // _in <expr>: return PTR(Expr)e

    return NEW(LetExpr)(name, rhs, body);
}

PTR(Expr)parse_if(std::istream &in) {
    PTR(Expr)test_case = parse_expr(in);

    std::string keyword = parse_keyword(in);
    if (keyword != "_then")
        throw std::runtime_error("expected keyword _then");
    PTR(Expr)then_case = parse_expr(in);

    keyword = parse_keyword(in);
    if (keyword != "_else")
        throw std::runtime_error("expected keyword _else");
    PTR(Expr)else_case = parse_expr(in);

    return NEW(IfExpr)(test_case, then_case, else_case);
}

PTR(Expr)parse_fun(std::istream &in) {
    skip_whitespace(in);
    char c = in.peek();

    if (c != '(')
        throw std::runtime_error("expected an open parenthesis");
    consume(in, '(');

    std::string var = "";
    while (1) {
        c = in.peek();
        if (!isalpha(c))
            break;
        var += in.get();
    }

    skip_whitespace(in);
    c = in.peek();
    if (c !=')')
        throw std::runtime_error("expected a close parenthesis");

    consume(in, ')');
    PTR(Expr)e = parse_expr(in);
    return NEW(FunExpr)(var, e);
}

/* ---------------------------------------- */
/*                utilities                 */
/* ---------------------------------------- */
void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c != expect)
        throw std::runtime_error("consume mismatch");
}

void skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}

PTR(Expr)parse_str(std::string s) {
    std::istringstream in(s);
    return parse(in);
}


