#ifndef TYPESTORAGE_H
#define TYPESTORAGE_H

#include <vector>

template <class T>
using TypeStorage = std::vector<T, std::allocator<T>>;

#endif //TYPESTORAGE_H
