#pragma once
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <unordered_map>
#include <variant>
#include <core.h>
namespace qk {

	// Represents a single value
	template <class ...Ts>
	using IComponentArgument = std::variant<Ts...>;


	// Argument identifier
	using ArgumentID = u64;

	// key-value map
	template <class... Ts>
	class IArgumentMap {
		std::unordered_map<ArgumentID, IComponentArgument<Ts...>> m_ArgMap;

	public:
		using value_type = std::pair<const ArgumentID, IComponentArgument<Ts...>>;

		IArgumentMap() = default;

		IArgumentMap(std::initializer_list<value_type> init)
			: m_ArgMap(init)
		{}

		operator const std::unordered_map<ArgumentID, IComponentArgument<Ts...>>& () const {
			return m_ArgMap;
		}

		auto begin() { return m_ArgMap.begin(); }
		auto end() { return m_ArgMap.end(); }

		auto begin() const { return m_ArgMap.begin(); }
		auto end() const { return m_ArgMap.end(); }

		bool contains(ArgumentID key) const {
			return m_ArgMap.contains(key);
		}

		IComponentArgument<Ts...>& operator[](ArgumentID key) {
			return m_ArgMap[key];
		}

		const auto& data() const {
			return m_ArgMap;
		}

		const IComponentArgument<Ts...>& at(ArgumentID id) const {
			return m_ArgMap.at(id);
		}
	};

	using BasicArgumentMap = IArgumentMap<bool, i32, u32, f32, str, std::vector<bool>, std::vector<i32>, std::vector<u32>, std::vector<f32>, std::vector<str>>;


	// returns the union of the argument maps
	template <class ...Ts>
	IArgumentMap<Ts...> operator+(const IArgumentMap<Ts...>& self, const IArgumentMap<Ts...>& other) {

		IArgumentMap<Ts...> arg_map;

		for (const auto& [k, v] : self) {
			assert(!arg_map.contains(k));
			arg_map[k] = v;
		}

		for (const auto& [k, v] : other) {
			assert(!arg_map.contains(k));
			arg_map[k] = v;
		}

		return arg_map;

	}

	// takes a key-value set and adds the component to the target entity.
	template <class ...Ts>
	using IComponentLoader_pfn = void (*)(entt::registry& registry, entt::entity target, const IArgumentMap<Ts...>& arguments);


	using ComponentID = u64;

	// Maps a component to the respective function that handles its deserialization
	template <class ...Ts>
	using IComponentLoaderMap = std::unordered_map<ComponentID, IComponentLoader_pfn<Ts...>>;


}