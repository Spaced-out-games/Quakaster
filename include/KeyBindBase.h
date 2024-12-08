#pragma once
#include "utility_types.h"
#include <iostream>


class KeyBindBase
{
	func_ptr_t<void, KeyBindBase*> bind_func;
	static void default_bind(KeyBindBase* base)
	{
		std::cout << "Key is unbound";
	}

	public:
	KeyBindBase(func_ptr_t<void, KeyBindBase*> bind_impl = default_bind) : bind_func(bind_impl) {}
	
	inline void run() { bind_func(this); }
	
};

struct say_bind: public KeyBindBase
{
	std::string content;
	static void say_impl(KeyBindBase* base)
	{
		say_bind* self = (say_bind*)base;
		std::cout << self->content << std::endl;
	}
};