#include <iostream>
#include <time.h>
std::string random_bytes();
std::string random_variable();
std::string random_expr_string();

int main(int argc, char **argv) {
    srand (clock());
    for(int i = 0; i < 10; i++){
        std::string s =random_variable();
        std::string in = random_expr_string();
        std:: cout << "Add/Num expr: "<< in << "\n";
        std:: cout << "Variable; " << s << "\n";
//        std::string rand_str = random_bytes();
//        std::cout << "Output: "<< rand_str << "\n";
    }
    return 0;
}
std::string random_variable(){
    std::string s = "";
    for(int i=0; i < rand()%6;i++){
        s+=(rand()%26) + 'a';
    }
    return s;
}
std::string random_expr_string() {
    if ((rand() % 10) < 6)
        return std::to_string(rand());
    else
        return random_expr_string() + "+" + random_expr_string();
}

// Makes a string of up to 31 random bytes
std::string random_bytes() {
    std::string word = "";
    for (int i = rand() % 32; i-- > 0; )
        word += rand() % 256;
    return word;
}

//std::string random_expr_string() {
//    return std::to_string(rand());
//}
