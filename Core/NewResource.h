#pragma once
#include <utility>
#include "core.h"
#include <atomic>
#include <type_traits>
#include <unordered_map>
#include <spdlog/spdlog.h>

namespace qk {

	class NewResource {
		std::atomic<u32> m_RefCount = 0;
		std::atomic<bool> m_FlaggedForDestruction = false;
		
		#ifdef _DEBUG
		str m_Name;
		#endif



		public:

			bool acquire() {

				if (m_RefCount == INT32_MAX) return false;
				
				m_RefCount++;

				return true;
			}

			bool release() {

				if (m_RefCount == 0) return false;
				
				m_RefCount--;
				
				return true;
			}

			const str* name() const {
				#ifdef _DEBUG
					return &m_Name;
				#else
					return nullptr;
				#endif
			}

			void name(const str& name) {
				#ifdef _DEBUG
					m_Name = name;
				#endif
			}

			void flag_for_destruction(bool flag = true) {
				m_FlaggedForDestruction = flag;
			}

			// We want these to always be called for the name of debugging
			NewResource() {
				#ifdef _DEBUG
					spdlog::info("Resource ctor called at {} on resource `{}`", (void*)this, m_Name);
				#endif
			}

			~NewResource() {
				#ifdef _DEBUG
					spdlog::info("Resource dtor called at {} on resource `{}`", (void*)this, m_Name);
				#endif
			}


	};




}