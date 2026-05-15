#pragma once
#include "core.h"
#include <io/Layer.h>
#include "GUIWidget.h"
#include <vector>
#include <memory>
#include "SharedDefs.h"

namespace qk::gfx::gui {

	struct QK_API GUILayer : qk::Layer {


		qk::HWindow m_hWindow;
		std::vector<std::unique_ptr<Widget>> m_Widgets;

		void push(std::unique_ptr<Widget> widget)
		{
			m_Widgets.emplace_back(std::move(widget));
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

		GUILayer(qk::HWindow hWindow);


		// sorts widgets by their Z
		void sort();

		virtual void onRender() override;

		virtual void onAttach() override;

		virtual void onDetach() override;

		[[nodiscard]] virtual EBlock onEvent(const Event& evt) override;


	};

}