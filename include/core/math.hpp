#ifndef __MATH_HPP__
#define __MATH_HPP__

#include <core/libapi.hpp>

#define CORE_MATH_CUSTOMIMPL 0

#include <SDL3/SDL.h>

#if CORE_MATH_CUSTOMIMPL

#define CORE_MATH_ABS(x) ((x) >= 0 ? (x) : -(x))

#define CORE_MATH_SIGN(x) ((x) >= 0 ? 1 : 0)

#define CORE_MATH_POW(x, amt) for(u8 i = 0 ; i < amt;i++) {x *= x;}

#define CORE_MATH_SQUARE(x) ((x) << 2)

#else

#define CORE_MATH_ABS(x) SDL_abs

#define CORE_MATH_SIGN(x) ((x) >= 0 ? 1 : 0)

#define CORE_MATH_POW(x, amt) SDL_pow

#define CORE_MATH_SQUARE(x) ((x) << 2)


#endif

CORE_DECLARE_NAMESPACE

namespace Math {

	template<typename Original, typename Return>
	Return Round(Original p_Value) {
		return static_cast<Return>(SDL_round(static_cast<double>(p_Value)));
	}
	template<typename Original, typename Return>
	Return Floor(Original p_Value) {
		return static_cast<Return>(SDL_floor(static_cast<double>(p_Value)));
	}

}


CORE_END_NAMESPACE


#endif