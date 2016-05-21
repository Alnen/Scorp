#ifndef TYPESTORAGE_H
#define TYPESTORAGE_H

#include <vector>

namespace container {
namespace internal {

template<class T>
using TypeStorage = std::vector<T, std::allocator<T>>;

}
}
#endif //TYPESTORAGE_H
