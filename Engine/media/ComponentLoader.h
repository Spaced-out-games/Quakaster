#pragma once
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <unordered_map>
#include <variant>
#include <core.h>
#include <media/json.h>
using qk::media::json;
namespace qk {

	// Represents a single value
	using EntityComponentArgument = std::variant<bool, i32, u32, f32, str, std::vector<bool>, std::vector<i32>, std::vector<u32>, std::vector<f32>, std::vector<str>>;


	// Argument identifier
	using ArgumentID = u64;

	// key-value map
	class EntityArgumentMap {
		std::unordered_map<ArgumentID, EntityComponentArgument> m_ArgMap;

	public:
		using value_type = std::pair<const ArgumentID, EntityComponentArgument>;

		EntityArgumentMap() = default;

		EntityArgumentMap(std::initializer_list<value_type> init)
			: m_ArgMap(init)
		{}

		operator const std::unordered_map<ArgumentID, EntityComponentArgument>& () const {
			return m_ArgMap;
		}

		auto begin() { return m_ArgMap.begin(); }
		auto end() { return m_ArgMap.end(); }

		auto begin() const { return m_ArgMap.begin(); }
		auto end() const { return m_ArgMap.end(); }

		bool contains(ArgumentID key) const {
			return m_ArgMap.contains(key);
		}

		EntityComponentArgument& operator[](ArgumentID key) {
			return m_ArgMap[key];
		}

		const auto& data() const {
			return m_ArgMap;
		}

		const EntityComponentArgument& at(ArgumentID id) const {
			return m_ArgMap.at(id);
		}
		template<typename T>
		T* try_get(ArgumentID id)
		{
			auto it = m_ArgMap.find(id);
			if (it == m_ArgMap.end())
				return nullptr;

			return std::get_if<T>(&it->second);
		}
		template<typename T>
		const T* try_get(ArgumentID id) const
		{
			auto it = m_ArgMap.find(id);
			if (it == m_ArgMap.end())
				return nullptr;

			return std::get_if<T>(&it->second);
		}

	};


	EntityArgumentMap from_json(const json& j);


	// returns the union of the argument maps
	EntityArgumentMap operator+(const EntityArgumentMap& self, const EntityArgumentMap& other);

	// takes a key-value set and adds the component to the target entity.
	using EntityComponentLoader_pfn_t = void (*)(entt::registry& registry, entt::entity target, const EntityArgumentMap& arguments);


	using EntityComponentID = u64;

	// Maps a component to the respective function that handles its deserialization
	using EntityComponentLoaderMap = std::unordered_map<EntityComponentID, EntityComponentLoader_pfn_t>;


}