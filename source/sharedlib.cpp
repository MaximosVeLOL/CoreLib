#include <core/sharedlib.hpp>
#include <core/platform.hpp>
#if CORE_PLATFORM_MODERN
#include <SDL3/SDL_loadso.h>
#endif


CORE_DECLARE_NAMESPACE

namespace sl {
	generic_func_t CLibrary::FindGeneric(const char* p_Name) {
		return SDL_LoadFunction(DataAs<SDL_SharedObject*>(), p_Name);
	}
	

	void CLibrary::Unload() {
		SDL_UnloadObject(DataAs<SDL_SharedObject*>());
	}

	bool CLibrary::IsValid() {
		return static_cast<bool>(m_LibData);
	}

	CLibrary::CLibrary(const char* p_Path) {
		m_LibData = SDL_LoadObject(p_Path);
	}
}

CORE_END_NAMESPACE