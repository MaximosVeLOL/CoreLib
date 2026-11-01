#ifndef __CORE_UTILITY_HPP__
#define __CORE_UTILITY_HPP__

#include <core/libapi.hpp>
#include <core/types.hpp>

CORE_DECLARE_NAMESPACE

namespace Utility {
	CORE_API extern u64 GetTicks();
	CORE_API extern u64 GetTicksNS();
}

CORE_END_NAMESPACE

#endif