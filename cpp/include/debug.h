/**
 * @author wcc
 */
#include <iostream>

namespace debug {
	template<class T> void printa(T array, int array_size);
}

namespace debug {
	template<class T> void printa(T s, int n) {
		int i;
		for (i = 0; i < n; ++i)
			std::cout << s[i] << (i == n-1 ? "\n" : " ");
	}
}

