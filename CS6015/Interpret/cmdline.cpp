//
//  cmdline.cpp
//  MSDScript
//
//  Created by Jakob West on 1/15/24.
//
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "cmdline.hpp"
#include <iostream>

//argc is the number of arguments you type in
//argv is the array of arguments that you type in
void use_arguments(int argc, char* argv[]) {
    //Since argc is the number of arguments that will be the extent of typed arguments in the command line
    //argv[i] = particular arugment
    bool testflag = false;
    for(int i = 1; i < argc; i++){

        //string comparision
        //if ==0 then the string are the same
        if(strcmp(argv[i], "--help") == 0){
            //print help text and exit with 0
            std :: cout << "help !! msdScript arguments allowed: \n"
                        << "--help: show list of helpful arugments \n"
                        << "--test: see if tests pass! \n";
            //exit 0 is for success and exit 1 is for errors
            exit(0);

            //If the argument is "--test", then use_arguments should print "Tests passed" on its own line and continue, but only if a "--test" argument has not been seen already. If "--test" has been seen before, use_arguments should print an error to std::cerr and exit with exit(1).
        } else if(strcmp(argv[i], "--test") == 0){
            if(testflag == false){

                std :: cout << "test command processsed! \n"
                            << "All tests passed! \n" ;

                testflag = true;
                Catch::Session().run(1,argv);
            }else{
                std::cerr << "'--test' argument has been seen before\n";
                exit(1);
            }
        } else{
            std :: cerr << "Argument ineligible, use --help for available arguements";
            exit(1);
        }

    }
}

