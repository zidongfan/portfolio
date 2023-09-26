/**
 * @file main.cpp
 * @brief int main() to run the catch tests
 * @mainpage MSDScript
 * @author Zidong Fan
 * @date 01/20/2023
 */

#define CATCH_CONFIG_RUNNER

#include <iostream>
#include "catch.h"
#include "cmdline.h"

int main(int argc, const char *argv[]) {

    switch (use_arguments(argc, argv)) {

        case do_nothing:
            std::cout << "Usage: " << argv[0] << " --test\n"
                      << "       " << argv[0] << " --interp: print the result\n"
                      << "       " << argv[0] << " --print\n"
                      << "       " << argv[0] << " --pretty-print\n";
            break;

        case do_test:
            if (Catch::Session().run(1, argv) != 0)
                return 1;
            break;

        case do_interp:
            try {
                PTR(Expr)e = parse(std::cin);
                std::cout << e->interp(Env::empty)->to_string() << "\n";
                break;
            } catch (std::runtime_error exn) {
                std::cerr << exn.what() << "\n";
                return 1;
            }

        case do_print:
            try {
                PTR(Expr)e = parse(std::cin);
                std::cout << e->to_string() << "\n";
                break;
            } catch (std::runtime_error exn) {
                std::cerr << exn.what() << "\n";
                return 1;
            }

        case do_pretty_print:
            try {
                PTR(Expr)e = parse(std::cin);
                std::cout << e->to_pretty_string() << "\n";
                break;
            } catch (std::runtime_error exn) {
                std::cerr << exn.what() << "\n";
                return 1;
            }

        default:
            std::cerr << "INVALID INPUT\n";
            return 1;

    }

    return 0;
}


