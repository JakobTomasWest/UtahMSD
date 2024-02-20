/**
* \mainpage MSDScript
* \author Jakob Tomas West
* 
* \date 06-02-2024
*/


#include <iostream>
#include "cmdline.hpp"
#include "expr.hpp"
#include "parse.hpp"

int main(int argc, char **argv) {
    run_mode_t mode = use_arguments(argc,argv);
    if(mode!=do_nothing) {
        Expr *e = parse(cin);
        switch (mode) {
//            case do_nothing:
//                break;
            case do_interp:
                cout << e->interp() << "\n";
                break;
            case do_print:
                cout << e->to_string() << "\n";
                break;
            case do_pretty_print:
                cout << e->to_pretty_string() << "\n";
        }
    }

}
