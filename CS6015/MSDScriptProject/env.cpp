//
// Created by Jakob West on 4/3/24.
//

#include "env.h"


PTR(Env) Env::empty = NEW(EmptyEnv)();
EmptyEnv::EmptyEnv() {

}

PTR(Val) EmptyEnv::lookup(std::string find_name) {
    throw std::runtime_error("free variable: "
                             + find_name);
}
ExtendedEnv::ExtendedEnv(string name, PTR(Val) val, PTR(Env) rest) {

    this-> name =  name;
    this-> val = val;
    this -> rest = rest;
}

PTR(Val) ExtendedEnv::lookup(std::string find_name) {
    if (this -> name == find_name)
        return this -> val;
    else
        return this -> rest->lookup(find_name);
}
