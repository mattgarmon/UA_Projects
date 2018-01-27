#include <iostream>
#include "vector.hpp"
#include <string>

int main()
{
    Vector<std::string> v1;
    v1.display();
    v1.push_back("yo");
    v1.display();
    v1.resize(10);
    v1.display();
    return 0;
}
