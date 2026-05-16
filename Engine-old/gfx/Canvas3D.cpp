#include "Canvas3D.h"
#include "GL/glew.h"

namespace qk::gfx {

	Canvas3D::~Canvas3D() {

	}

	qk::io::Layer::EBlock Canvas3D::on_event(const qk::io::Event& evt) {
		// don't worry about it yet
		return EBlock::Pass;
	}
	void Canvas3D::on_render() {
		// note: this WILL cause a bug if the graphics API isn't initialized before it is called, if m_CommandBuffer !=null!
		if (m_CommandBuffer) call(*m_CommandBuffer);

	}
	void Canvas3D::on_attach() {}
	void Canvas3D::on_detach() {}



}