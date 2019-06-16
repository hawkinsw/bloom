#include "bloom.h"
#include <iostream>

template <unsigned int K>
class Key {
	public:
		void operator()(int value, unsigned int *result) const {
			*result = K + value;
		}
};

int main() {
	std::array<Bloom::KGF<int>, 3> keys{Key<0>(), Key<1>(), Key<2>()};
	Bloom::Filter<32,3,int> filter(keys, true);

	filter.insert(5);
	std::cout << "Contains? " << std::boolalpha << filter.contains(1) << std::endl;
	return 0;
}
