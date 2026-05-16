#pragma once
#include "../core/io/Layer.h"
#include "CommandBuffer.h"
#include "../core.h"



namespace qk::gfx {



	struct QK_API Canvas3D : qk::io::Layer {

		CommandBuffer* m_CommandBuffer = nullptr;

		virtual ~Canvas3D() override;

		[[nodiscard]] EBlock on_event(const qk::io::Event& evt);

		void on_render() override;

		void on_attach() override;

		void on_detach() override;

	};

}