#pragma once
#include <functional>
#include <vector>
#include <queue> // for reference
#include "../../core.h"
#include <cassert>


namespace qk::util {
	struct DestructionQueue {
			using dtor_fn_t = std::function<void(void)>;
		private:
			std::vector <dtor_fn_t> m_DtorQueue;
		public:
			template<typename F>
			void push(F&& func) {
				m_DtorQueue.emplace_back(std::forward<F>(func));
			}
			void flush() noexcept(false) {
				for (auto it = m_DtorQueue.rbegin(); it != m_DtorQueue.rend(); ++it) {
					std::move(*it)();
				}

				m_DtorQueue.clear();
			}

			void operator()() {
				flush();
			}
	};
}