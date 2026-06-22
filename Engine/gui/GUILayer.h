/// **************************************** QUAKASTER ENGINE **************************************** 
/// gui/GUILayer.h
/// Purpose: Holds ImGui widgets and propagates input events to them
/// **************************************************************************************************
#pragma once
#include "core.h"
#include <io/Layer.h>
#include "GUIWidget.h"
#include <vector>
#include <memory>

namespace qk::gfx::gui {

	// Holds ImGui widgets
	struct QK_API GUILayer : qk::Layer {


		void* m_hWindow = nullptr;

		std::vector<std::unique_ptr<Widget>> m_Widgets;

		void push(std::unique_ptr<Widget> pWidget)
		{
			m_Widgets.emplace_back(std::move(pWidget));
		}

		template <typename T, typename... Args>
		T& emplace(Args&&... args)
		{
			static_assert(std::is_base_of_v<Widget, T>);

			auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
			T& ref = *ptr;
			m_Widgets.emplace_back(std::move(ptr));
			return ref;
		}

		GUILayer(void* hWindow);


		// sorts widgets by their Z. Unimplemented.
		void sort();

		virtual void onRender() override;

		virtual void onAttach() override;

		virtual void onDetach() override;

		[[nodiscard]] virtual EBlock onEvent(const Event& evt) override;


	};

}