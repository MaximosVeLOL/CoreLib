#ifndef	__CORE_THREADING_HPP__
#define	__CORE_THREADING_HPP__

#include <core/libapi.hpp>
#include <core/platform.hpp>

#ifdef CORE_PLATFORM_MODERN
#include <thread>
#endif

CORE_DECLARE_NAMESPACE

#ifdef CORE_PLATFORM_MODERN
using thread = std::thread;
#endif


CORE_END_NAMESPACE

#endif