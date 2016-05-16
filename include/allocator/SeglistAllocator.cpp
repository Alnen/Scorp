#include "allocator/SeglistAllocator.h"

const int SeglistAllocator::MAX_SIZE = SeglistAllocator::getPowerOfTwo(SeglistAllocator::max_power_of_two);