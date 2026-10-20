///
///		Quakaster system kernel
/// 

#pragma once
#include <functional>
#include <cstdint>
#include <vector>
#include <array>

#define QK_KERNEL_SYSCALL_REGISTER_COUNT 16
#define QK_KERNEL_SYSCALL_RETURN_REGISTER 0
#define QK_KERNEL_SYSCALL_ID_REGISTER 0

namespace qk::scripting {

	// Explicit values: scripts and logs depend on these numbers, so never renumber.
	// Add new codes at the end, before QK_KERNEL_STATUS_COUNT.
	enum SyscallStatus : std::uintptr_t {
		QK_KERNEL_OK = 0,
		QK_KERNEL_NOT_IMPLEMENTED = 1,  // syscall number out of range or unbound
		QK_KERNEL_INVALID_HANDLE = 2,  // ID is stale, never existed, or names a dead object
		QK_KERNEL_INVALID_ARGUMENT = 3,  // malformed or out of range, invalid regardless of engine state
		QK_KERNEL_OUT_OF_MEMORY = 4,  // allocation failed (e.g. glBufferData GL_OUT_OF_MEMORY)
		QK_KERNEL_INTERNAL_ERROR = 5,  // engine invariant broke; a bug, not caller error

		QK_KERNEL_STATUS_COUNT            // sentinel, keep last
	};

	constexpr const char* to_string(SyscallStatus s) {
		switch (s) {
		case QK_KERNEL_OK:               return "OK";
		case QK_KERNEL_NOT_IMPLEMENTED:  return "NOT_IMPLEMENTED";
		case QK_KERNEL_INVALID_HANDLE:   return "INVALID_HANDLE";
		case QK_KERNEL_INVALID_ARGUMENT: return "INVALID_ARGUMENT";
		case QK_KERNEL_OUT_OF_MEMORY:    return "OUT_OF_MEMORY";
		case QK_KERNEL_INTERNAL_ERROR:   return "INTERNAL_ERROR";
		default:                         return "UNKNOWN";
		}
	}

	constexpr bool ok(SyscallStatus s) { return s == QK_KERNEL_OK; }


	using SyscallRegisters = std::array<uintptr_t, QK_KERNEL_SYSCALL_REGISTER_COUNT>;
	
	struct Kernel;

	using Kernel_fn_t = std::function<void(Kernel&, SyscallRegisters& registers)>;

	struct Kernel {
		std::vector<Kernel_fn_t> m_SyscallTable;
	};
}