//
// Created by Jakob West on 1/23/24.
//

#ifndef INTERPRET_CMDLINE_HPP
#define INTERPRET_CMDLINE_HPP

#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <iostream>
typedef enum {
    do_nothing,
    do_test,
    do_interp,
    do_print,
    do_pretty_print
} run_mode_t;

run_mode_t use_arguments(int argc, char **argv);

#endif //INTERPRET_CMDLINE_HPP
