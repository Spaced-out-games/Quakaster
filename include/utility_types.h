#pragma once

template <typename return_t, typename... arg_t>
using func_ptr_t = return_t(*)(arg_t...);