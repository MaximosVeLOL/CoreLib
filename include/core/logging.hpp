#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <core/libapi.hpp>
#include <format>
#include <print>
#include <core/utility.hpp>

constexpr bool CORE_LOGGER_USECOLOR = false;

#define CORE_LOGGER_CREATE(_p_Type, _p_Color) \
template<typename... Args> \
inline void Log##_p_Type##(std::format_string<Args...> p_Format, Args&&... p_Args) { \
	if constexpr(_p_Color != 7 && CORE_LOGGER_USECOLOR) \
		system("Color 0" #_p_Color##); \
	std::print("{} ({}): ", #_p_Type##, cl::Utility::GetTicks()); \
	std::println(p_Format, p_Args...); \
	if constexpr(_p_Color != 7 && CORE_LOGGER_USECOLOR) \
		system("Color 07"); \
}
CORE_DECLARE_NAMESPACE

CORE_LOGGER_CREATE(Info, 7)
CORE_LOGGER_CREATE(Debug, 5)
CORE_LOGGER_CREATE(Warning, 6)
CORE_LOGGER_CREATE(Error, 4)

CORE_END_NAMESPACE

#endif