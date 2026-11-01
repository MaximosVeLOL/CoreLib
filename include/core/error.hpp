#ifndef __CORE_ERROR_HPP__
#define __CORE_ERROR_HPP__

#include <core/libapi.hpp>
#include <exception>
#include <stdexcept>
#include <format>

CORE_DECLARE_NAMESPACE

namespace Error {
	
	class CRuntime : public std::runtime_error {
	public:
		template<typename... Args>
		CRuntime(std::format_string<Args...> p_Format, Args&&... p_Args) : std::runtime_error(std::format(p_Format, p_Args...)) {}
		CRuntime(const char* p_Error) : std::runtime_error(p_Error) {}
	};
	class CExOutOfBounds : public CRuntime {
	public:
		CExOutOfBounds() : CRuntime("The index was out of bounds of the array.") {}
	};
	class CExUnknown : public CRuntime {
	public:
		CExUnknown() : CRuntime("An unknown error has occured.") {}
	};
}


CORE_END_NAMESPACE

#endif