#include "shape.hpp"
#include <iostream>
using namespace std;

Rectangle::Rectangle(float width, float height):width(width),height(height) {}

Circle::Circle(float radius){
    this->radius=radius;
}

float Rectangle::getarea() {
    return width * height ;
}

float Circle::getarea() {
    return 3.14 * radius * radius ;
}

void printArea(Shape& shape) {
    cout << "Area: " << shape.getarea() << endl;
}

