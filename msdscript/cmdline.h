/**
 * @file cmdline.h
 * @brief contains use_arguments function
 *
 * @author Zidong Fan
 * @date 01/13/2023
 */

#ifndef cmdline_h
#define cmdline_h


#include "expr.h"
#include "parse.h"
#include "val.h"
#include "env.h"

#include <stdio.h>
#include <iostream>
#include <string>

typedef enum {

    do_nothing,         // print help msg
    do_test,            // do test
    do_interp,          // input and interp
    do_print,           // input and print
    do_pretty_print    // input and pretty print

} run_mode_t;

run_mode_t use_arguments(int argc, const char * argv[]);

#endif /* cmdline_h */


