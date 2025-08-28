class Shape {
    
public:
    virtual float getarea()=0; // equals to -, means no implementation,
    // now called pure virtual method - the class is now abstract class
    //must now be implemented in all the subclasses
};

class Circle: public Shape {
public:
    float radius;
    Circle(float radius);
    float getarea();
};

class Rectangle: public Shape {
    public:
        float width;
        float height;
        Rectangle(float width, float height);
        float getarea() ;
};

void printArea(Shape* shape); //you can change this to * to take pointers in your implementation/main instead of using & which will take r1 but not *r1
//passing references (&) Shape& shape is good practice

using namespace std;
