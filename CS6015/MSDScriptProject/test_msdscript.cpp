////
//// Created by Jakob West on 2/21/24.
////
#include <iostream>
#include <stdlib.h>
#include "exec.h"
#include <time.h>
std::string random_bytes();
std::string random_variable();
std::string random_expr_string();



int main(int argc, char **argv) {
srand(clock());
    if(argc > 2) {
        const char *const file1 = argv[1];
        const char *const file2 = argv[2];

        const char *const interp_argv[] = {file1, "--interp"};
        const char *const print_argv[] = {file1, "--print"};
        const char *const pprint_argv[] = {file1, "--pretty-print"};

        const char *const interp2_argv[] = {file2, "--interp"};
        const char *const print2_argv[] = {file2, "--print"};
        const char *const pprint2_argv[] = {file2, "--pretty-print"};
        for (int i = 0; i < 10; i++) {
            std::string in = random_expr_string();
            std::cout << "trying to print: " << in << "\n";


            ExecResult interp_result = exec_program(2, interp_argv, in);
            ExecResult print_result = exec_program(2, print_argv, in);
            ExecResult pprint_result = exec_program(2,pprint_argv,in);
            std::cout << "My originalprint   : " << print_result.out << "";
            std::cout << "My prettyprint   : " << pprint_result.out << "";

            ExecResult interp_again_result = exec_program(2, interp_argv, print_result.out);
            std::cout << "My print interpreted :" << interp_again_result.out << "";
            ExecResult interpp_again_result = exec_program(2, interp_argv, pprint_result.out);
            std::cout << "My prettyprint interpreted :" << interpp_again_result.out << "\n";



            ExecResult interp2_result = exec_program(2, interp2_argv, in);
            ExecResult print2_result = exec_program(2, print2_argv, in);
            ExecResult pprint2_result = exec_program(2,pprint2_argv,in);
            std::cout << "Their original print: " << print2_result.out << "";
            std::cout << "Their prettyprint: " << pprint2_result.out << "";


            ExecResult interp2_again_result = exec_program(2, interp2_argv, print2_result.out);
            std::cout << "Their print interpreted :" << interp2_again_result.out << "";
            ExecResult interpp2_again_result = exec_program(2, interp2_argv, pprint2_result.out);
            std::cout << "Their prettyprint interpreted :" << interpp2_again_result.out << "\n";



            if(print2_result.out!=print_result.out)
                throw std::runtime_error("different print");

            if(pprint2_result.out!=pprint_result.out)
                throw std::runtime_error("different pretty print");

//            if (interp_again_result.out != interp_result.out)
//                throw std::runtime_error("different result for printed");

            if(interp2_again_result.out != interp_again_result.out)
                throw std::runtime_error("different interpreted results");
            if(interpp2_again_result.out != interpp_again_result.out)
                throw std::runtime_error("different interpreted results");


        }

    }else {
        const char *const file1 = argv[1];
        const char *const interp_argv[] = {file1, "--interp"};
        const char *const print_argv[] = {file1, "--print"};
        const char *const pprint_argv[] = {file1, "--pretty-print"};
        for (int i = 0; i < 10; i++) {
            std::string in = random_expr_string();
            std::cout << "trying to print: " << in << "\n";


            ExecResult interp_result = exec_program(2, interp_argv, in);
            ExecResult print_result = exec_program(2, print_argv, in);
            ExecResult pprint_result = exec_program(2, pprint_argv, in);
            std::cout << "My prettyprint   : " << pprint_result.out << "";

            ExecResult interp_again_result = exec_program(2, interp_argv, print_result.out);
            std::cout << "My print interpreted :" << interp_again_result.out << "";
            ExecResult interpp_again_result = exec_program(2, interp_argv, pprint_result.out);
            std::cout << "My prettyprint interpreted :" << interpp_again_result.out << "";
            std::cout << "My result   : " << print_result.out << "\n";

            if (interp_again_result.out != interp_result.out)
                throw std::runtime_error("different result for printed");
        }
    }
    return 0;
}

std::string random_expr_string() {
//
//    if((rand() % 10) <6) {
//        return std::to_string(rand()%1000);

    if ((rand() % 10) == 6 || (rand() % 10) == 5 || (rand() % 10) == 4) {
        return std::to_string(rand()%1000);
    }else if ((rand() % 10) <= 3){
        return "-" + std::to_string(rand()%1000);
    }else if ((rand() % 10) == 7 ){
        return "_let " + random_variable() + " = " + random_expr_string() + " _in " + random_expr_string();
    } else if ((rand() % 10) == 8 ) {
        return random_expr_string() + "*" + random_expr_string();
    } else if ((rand() % 10) == 9 ){
        return random_variable();
    } else {
        return random_expr_string() + "+" + random_expr_string();
    }
}

std::string random_variable(){
    std::string s = "";
    for(int i=0; i < rand()%6;i++){
        s+=(rand()%26) + 'a';
    }
    return s;
}
//std::string random_expr_string() {
//    if ((rand() % 10) < 6)
//        return std::to_string(rand());
//    else
//        return random_expr_string() + "+" + random_expr_string();
//}

// Makes a string of up to 31 random bytes
std::string random_bytes() {
    std::string word = "";
    for (int i = rand() % 32; i-- > 0; )
        word += rand() % 256;
    return word;
}