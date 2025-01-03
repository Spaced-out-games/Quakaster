#pragma once


// Templated function pointer. Easy to read and understand, lol
template <typename return_t, typename... arg_t>
using func_ptr_t = return_t(*)(arg_t...);