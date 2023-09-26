/**
 * @file cmdline.cpp
 * @brief contains use_arguments function
 *
 * @author Zidong Fan
 * @date 01/13/2023
 */

#include "cmdline.h"

run_mode_t use_arguments(int argc, const char *argv[]) {

    std::string help = "--help";
    std::string test = "--test";
    std::string interp = "--interp";
    std::string print = "--print";
    std::string prettyPrint = "--pretty-print";

    // print usages
    if (argc != 2 || argv[1] == help) {
        return do_nothing;
    }
    // run tests and exit with a 0 status if all pass, a non-zero status if there are any failures
    else if (argv[1] == test) {
        return do_test;
    }
    // accept a single expression on standard input, parse it, interp it, print the result to standard output, and print a newline, exiting with a 0 status
    else if (argv[1] == interp) {
        //Note that if you run your interpreter at the command line and type input, you'll need to send an “end of file” after your expression with Ctl-D (typically on a new line). Otherwise, your parser will probably be trying to skip whitespace and still waiting for more input
        return do_interp;
    }
    // accept a single expression on standard input and print it back out to standard output using the print method of Expr, print a newline, and exit with a 0 status
    else if (argv[1] == print) {
        return do_print;
    }
    // accept a single expression on standard input and print it back out to standard output using the pretty_print method of Expr, print a newline, and exit with a 0 status;
    // keep in mind that your pretty_print method may expect a string output port, so you might instead use something like a to_pretty_string method and then print the string to standard output
    else if (argv[1] == prettyPrint) {
        return do_pretty_print;
    }
    // You can choose what happens if multiple flags or additional flags provided, as long as your msdscript behaves as above for a single command-line argument
    else {
        std::cerr << "Invalid Command\n";
        return do_nothing;
    }

}
