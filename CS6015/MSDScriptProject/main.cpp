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
#include "TESTS.h"
#include "Val.h"

int main(int argc, char **argv) {
    try {
        run_mode_t mode = use_arguments(argc, argv);
        if (mode != do_nothing) {
            Expr *e = parseInput();
            switch (mode) {
//            case do_nothing:
//                break;
                case do_interp:
                    std::cout << e->interp()->to_string() << "\n";
                    break;
                case do_print:
                    cout << e->interp()->to_string() << "\n";
                    break;
                case do_pretty_print:
                    cout << e->interp()->to_string() << "\n";
                case do_nothing:
                default:
                    // Print usage information or an error message if needed
                    std::cerr << "Invalid or missing flag. Usage: msdscript --flag\n";
                    return 1;
            }
            return 0; // Exit with 0 to indicate success
        }
    } catch (const std::exception &ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
            return 1; // Exit with 1 to indicate an error occurred
        }
    }
