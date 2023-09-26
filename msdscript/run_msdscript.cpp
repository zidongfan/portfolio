/**
 * @file test_msdscript.cpp
 * @brief
 *  When given a single argument, that argument is used as a path for an msdscript program
 *  and test_msdscript should test that msdscript implementation on randomly generated programs
 *
 *  When given two arguments, each argument is used as the path to an msdscript program
 *  and test_msdscript should try to find a program and mode (--interp, --print
 *  or --pretty-print) where the two programs disagree
 *
 * @author Zidong Fan
 * @date 02/22/2023
 */

#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "exec.h"

#define Itr 100

std::string random_expr_string();

int main(int argc, const char *argv[]) {

    if (argc == 2 || argc == 3) {

        const char *const interp_argv[] = {argv[1], "--interp"};
        const char *const print_argv[] = {argv[argc-1], "--print"};

        for (int i = 0; i < Itr; i++) {
            std::string in = random_expr_string();
            std::cout << "Trying " << in << "\n";

            ExecResult interp_result = exec_program(2, interp_argv, in);
            if (interp_result.exit_code != 0){
                std::cerr << argv[1] << " Interp Error: " << interp_result.err << "\n";
                exit(1);
            }

            ExecResult print_result = exec_program(2, print_argv, in);
            if (interp_result.exit_code != 0){
                std::cerr << argv[argc-1] << " Print Error: " << print_result.err << "\n";
                exit(1);
            }

            ExecResult interp_again_result = exec_program(2, interp_argv, print_result.out);
            if (interp_again_result.exit_code != 0){
                std::cerr << argv[argc-1] << " Interp Again Error: " << interp_again_result.err << "\n";
                exit(1);
            }

            if (interp_again_result.out != interp_result.out)
                throw std::runtime_error("different result for printed");
        }

    } else {
        std::cout << "Usage: " << argv[0] << " <executable>\n"
                  << "       " << argv[0] << " <executable> <executable>\n";
        exit(1);
    }

    return 0;
}

//std::string random_expr_string() {
//    srand(clock());
//    if ((rand() % 10) < 4)
//        return std::to_string(rand());
//    else if ((rand() % 10) == 4){
//        std::string str = "";
//        str += char('a' + rand() % 26);
//        return str;
//    }
//    else if ((rand() % 10) == 5)
//        return random_expr_string() + "+" + random_expr_string();
//    else if ((rand() % 10) == 6)
//        return random_expr_string() + "*" + random_expr_string();
//    else if ((rand() % 10) == 7)
//        return random_expr_string() + "==" + random_expr_string();
//    else if ((rand() % 10) == 8){
//        std::string str = "";
//        str += char('a' + rand() % 26);
//        return "_let " + str + " = " + random_expr_string() + " _in " + random_expr_string();
//    }
//    else
//        return "_if " + random_expr_string() + " _then " + random_expr_string() + " _else " + random_expr_string();
//}

std::string random_expr_string() {
    srand(clock());
    if ((rand() % 10) < 5)
        return std::to_string(rand());
//    else if ((rand() % 10) == 4){
//        std::string str = "";
//        str += char('a' + rand() % 26);
//        return str;
//    }
    else if ((rand() % 10) == 5)
        return std::to_string(rand())+ "+" + std::to_string(rand());
    else if ((rand() % 10) == 6)
        return std::to_string(rand()) + "*" + std::to_string(rand());
    else if ((rand() % 10) == 7)
        return std::to_string(rand()) + "==" + std::to_string(rand());
    else if ((rand() % 10) == 8){
        std::string str = "";
        str += char('a' + rand() % 26);
        return "_let " + str + " = " + std::to_string(rand()) + " _in " + str + " + " + std::to_string(rand());
    }
    else
        return "_if " + std::to_string(rand()) + "==" + std::to_string(rand()) + " _then " + std::to_string(rand()) + " _else " + std::to_string(rand());
}

