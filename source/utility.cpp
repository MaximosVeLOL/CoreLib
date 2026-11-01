#include <core/utility.hpp>
#include <SDL3/SDL.h>

CORE_DECLARE_NAMESPACE

namespace Utility {
	u64 GetTicks() {
		return SDL_GetTicks();
	}
	u64 GetTicksNS() {
		return SDL_GetTicksNS();
	}
}

CORE_END_NAMESPACE