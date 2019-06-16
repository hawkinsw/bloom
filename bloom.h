#ifndef _BLOOM_CPP
#define _BLOOM_CPP

#include <array>
#include <functional>
#include <bitset>
#include <iostream>


namespace Bloom {
	template <class KGF_Value>
	using KGF = std::function<void(KGF_Value, unsigned int *)>;

	template <unsigned int M, unsigned int K, class Value>
	class Filter {

		public:
			Filter(std::array<KGF<Value>, K> kgfs) :
				mKgfs(kgfs),
				mDebug(false) {}
			Filter(std::array<KGF<Value>, K> kgfs, bool debug) : Filter(kgfs) {
				mDebug = debug;
			}

			unsigned int getM() {
				return M;
			}
			unsigned int getK() {
				return K;
			}

			void insert(Value v) {
				for (auto f : mKgfs) {
					unsigned int kf{0};
					f(v, &kf);
					mSet.set(kf, true);
				}
				if (mDebug) {
					std::cout << "bitset: " << mSet << std::endl;
				}
			}

			bool contains(Value v) {
				for (auto f : mKgfs) {
					unsigned int kf{0};
					f(v, &kf);
					if (mSet[kf] != 1)
						return false;
				}
				return true;
			}

		private:
		std::array<KGF<Value>, K> mKgfs;
		std::bitset<M> mSet;
		bool mDebug;
	};
}
#endif
