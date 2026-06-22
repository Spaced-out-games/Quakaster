/// **************************************** QUAKASTER ENGINE **************************************** 
/// utility/DestructionQueue.h 
/// Purpose: Automatically destroys resources
/// **************************************************************************************************
#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <cassert>


namespace qk::utility {
	struct DestructionQueue {
		using dtor_fn_t = std::function<void(void)>;
	private:
		std::vector <dtor_fn_t> m_DtorQueue;
	public:
		// Add a destructor function to the queue.
		template<typename F>
		void push(F&& func) {
			m_DtorQueue.emplace_back(std::forward<F>(func));
		}
		// Calls each function
		void flush() noexcept(false) {
			for (auto it = m_DtorQueue.rbegin(); it != m_DtorQueue.rend(); ++it) {
				// This std::moves then calls each function. It's hard to tell at a glance
				std::move(*it)();
			}

			m_DtorQueue.clear();
		}

		void operator()() {
			flush();
		}

		~DestructionQueue()
		{
			flush();
		}
	};
}