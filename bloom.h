#ifndef _BLOOM_CPP
#define _BLOOM_CPP

#include <array>
#include <functional>
#include <bitset>
#include <iostream>
#include <thread>
#include <cassert>

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
				unsigned int keys[K];

				generate_keys(v, keys);
				for (unsigned int key : keys) {
					mSet.set(key, true);
				}
				if (mDebug) {
					std::cout << "bitset: " << mSet << std::endl;
				}
			}

			bool contains(Value v) {
				unsigned int keys[K];

				generate_keys(v, keys);
				for (unsigned int key : keys) {
					if (!mSet[key]) {
						return false;
					}
				}
				return true;
			}


		private:
			std::array<KGF<Value>, K> mKgfs;
			std::bitset<M> mSet;
			bool mDebug;

			void generate_keys(Value v, unsigned int *keys) {
				unsigned int key_thread_idx;
				std::thread key_threads[K];

				/*
				 * Spawn K threads to calculate keys in parallel.
				 */
				for (key_thread_idx = 0; key_thread_idx<K; key_thread_idx++) {
					auto f = mKgfs[key_thread_idx];
					key_threads[key_thread_idx] =
						std::thread(f, v, &keys[key_thread_idx]);
				}
				assert(key_thread_idx==K);

				/*
				 * Wait for them to finish.
				 */
				for (key_thread_idx = 0; key_thread_idx<K; key_thread_idx++) {
					key_threads[key_thread_idx].join();
					if (keys[key_thread_idx] > M) {
						if (mDebug) {
							std::cout << "Key too big: "
							          << keys[key_thread_idx]
							          << " > "
							          << M << "." << std::endl;
						}
						assert(false);
					}
				}
				assert(key_thread_idx==K);
			}
	};
}
#endif
