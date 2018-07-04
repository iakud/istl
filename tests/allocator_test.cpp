#include "allocator.h"
#include <memory>

int main() {
	istd::allocator<int> alloc;
	int* p = alloc.allocate(1);
	alloc.construct(p, 1);

	std::allocator<int> std_alloc;
	int* std_p = std_alloc.allocate(1);
	std_alloc.construct(std_p, 1);

	return 0;
}