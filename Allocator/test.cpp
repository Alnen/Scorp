#include "stl_allocator.cpp"

int main(){
    const int numItems = 100;
    std::list<int, Allocator<int> > l;
    for (int i = 0; i < numItems; ++i) {
        l.push_back(i);
    }

    return 0;
}
