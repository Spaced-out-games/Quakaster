#pragma once
#include <include/components/transform.h>
#include <include/IO/InputDelegate.h>
#include <include/base/Event.h>
#include <include/components/camera.h>
#include <include/thirdparty/entt.hpp>
#include <math.h>


struct Controller {

	Controller(
		std::function<void(KeyPressEvent)> on_key_press_fn = [](KeyPressEvent) {},
		std::function<void(KeyHoldEvent)> on_key_hold_fn = [](KeyHoldEvent) {},
		std::function<void(KeyReleaseEvent)> on_key_release_fn = [](KeyReleaseEvent) {},
		std::function<void(MouseMoveEvent)> on_mouse_move_fn = [](MouseMoveEvent) {}
	) :
		on_key_press(on_key_press_fn),
		on_key_hold(on_key_hold_fn),
		on_key_release(on_key_release_fn),
		on_mouse_move(on_mouse_move_fn)
	{}

	protected:
		EventListener<KeyPressEvent> on_key_press;
		EventListener<KeyHoldEvent> on_key_hold;
		EventListener<KeyReleaseEvent> on_key_release;
		EventListener<MouseMoveEvent> on_mouse_move;

	//private:



};