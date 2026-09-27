#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <core/libapi.hpp>
#include <format>
#include <print>


#define CORE_LOGGER_CREATE(type) \
template<typename... Args> \
inline void Log##type##(std::format_string<Args...> p_Format, Args... p_Args) { \
	std::print("{}: ", #type##); \
	std::println(p_Format, p_Args...); \
}

CORE_DECLARE_NAMESPACE

CORE_LOGGER_CREATE(Info)
CORE_LOGGER_CREATE(Debug)
CORE_LOGGER_CREATE(Warn)
CORE_LOGGER_CREATE(Error)

CORE_END_NAMESPACE

#endif