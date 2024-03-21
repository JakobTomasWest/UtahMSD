#include <iostream>

class MyClass {
public:
    MyClass() {}
    
    void print_method() {
      std::cout << "Printing . . ." << std::endl;
    }
    
    ~MyClass() {}
};

int main() {
    MyClass* obj1 = new MyClass();
    obj1->print_method();
    obj1=nullptr;

    obj1->print_method();  // undefined behavior
    return 0;
}

