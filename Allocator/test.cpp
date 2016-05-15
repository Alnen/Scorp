#include "stl_allocator.cpp"
#include <iostream>

int main(){
    const int numItems = 99;
    std::list<int, Allocator<int> > l;
    for (int i = 0; i < numItems; ++i) {
        l.push_back(i);
    }

    l.clear();
std::cout << "Success" << std::endl;

    return 0;
}
