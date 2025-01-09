#pragma once

/*
#include <thirdparty/glm/glm.hpp>
#include <thirdparty/glm/gtc/quaternion.hpp>
#include <thirdparty/glm/gtc/matrix_transform.hpp>
#include <thirdparty/glm/gtc/type_ptr.hpp>
*/


// Templated function pointer. Easy to read and understand, lol
template <typename return_t, typename... arg_t>
using func_ptr_t = return_t(*)(arg_t...);



