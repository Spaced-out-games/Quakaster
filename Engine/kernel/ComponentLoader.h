#pragma once
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include "../vendor/emhash/hash_table5.hpp"
#include <variant>
#include <vector>
#include <core.h>
#include <media/json.h>
#include <fnv1a.h>
#include <KeyValues.h>
using qk::media::JSONArrayClassification;
using qk::media::flatten_json;
using qk::media::classify;
using qk::media::json;
using qk::string_hash;

namespace qk {





	using EntityComponentLoader_pfn_t = void (*)(entt::registry& registry, entt::entity target, const KeyValues& arguments);
	
	class ComponentLoader {

	
		// name of this component
		str m_Name;

		// which keys this component loader uses
		std::vector<str> m_Dependencies;

		// how to load this component
		EntityComponentLoader_pfn_t			   m_pLoader = nullptr;


		public:

		operator bool() const noexcept {
			return m_pLoader != nullptr;
		}

		size_t num_dependencies() const { return m_Dependencies.size(); }

		std::span<const str> dependencies() const {
			return m_Dependencies;

		}


		ComponentLoader(str_view name, std::span<const str_view> required_components, EntityComponentLoader_pfn_t pImpl):
			m_Name(name),
			m_Dependencies(required_components.begin(), required_components.end()),
			m_pLoader(pImpl)
		{

		}

		void operator()(entt::registry& registry, entt::entity target, const KeyValues& arguments) {
			QK_ASSERT(m_pLoader, "m_pLoader was nullptr");

			m_pLoader(registry, target, arguments);

		}

		EntityComponentLoader_pfn_t impl() const {
			return m_pLoader;
		}

		str_view name() const {
			return m_Name;
		}

	};


}