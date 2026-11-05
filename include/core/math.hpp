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
	template<typename Original, typename Return, typename GridT = u8>
	Return Grid(Original p_Value, GridT p_Grid = (GridT)32, bool p_UseRounding = true) {
		
		if (p_UseRounding)
			return static_cast<Return>(Round<Original, Return>(static_cast<Original>(p_Value / p_Grid)) * p_Grid);
		return static_cast<Return>(Floor<Original, Return>(static_cast<Original>(p_Value / p_Grid)) * p_Grid);

	}

}


CORE_END_NAMESPACE


#endif