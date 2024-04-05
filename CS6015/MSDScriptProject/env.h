//
// Created by Jakob West on 4/3/24.
//

#ifndef LAB03_ENV_H
#define LAB03_ENV_H
#include <stdio.h>
#include <string>
#include "pointer.hpp"
using namespace std;
class Val;

CLASS(Env){
        public:
        static PTR(Env) empty;
        virtual PTR(Val) lookup(string find_name) = 0;
};


class EmptyEnv : public Env {
public:
    EmptyEnv();
    PTR(Val) lookup(string find_name) override;
};

class ExtendedEnv : public Env {
public:
    std::string name;
    PTR(Val) val;
    PTR(Env) rest;

    ExtendedEnv(string name, PTR(Val) val, PTR(Env) rest);
    PTR(Val) lookup(string find_name) override;

};
#endif //LAB03_ENV_H
