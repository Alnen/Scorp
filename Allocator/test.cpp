#include "stl_allocator.h"
#include <iostream>

int main(){
    const int numItems = 50;
    std::list<int, Allocator<int> > l;
    for (int i = 0; i < numItems; ++i) {
        l.push_back(i);
    }

   // l.pop_back();
    //l.clear();
std::cout << "Success" << std::endl;

    return 0;
}
