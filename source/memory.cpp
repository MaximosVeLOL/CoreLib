#include <core/memory.hpp>
#include <SDL3/SDL.h>

CORE_DECLARE_NAMESPACE

namespace Mem {
	void* Allocate(memsize_t p_Amount) {
		//return ::operator new(p_Amount);
		return SDL_malloc(static_cast<size_t>(p_Amount));
	}
	void* ZeroAllocate(memsize_t p_Amount) {
		//void* ret = ::operator new(p_Amount) {0x00};
		//Is this hacky?
		//return static_cast<void*>(new unsigned __int8[p_Amount] {0x00});
		return SDL_calloc(p_Amount, 1);
	}
	void* Reallocate(void* p_Data, memsize_t p_NewAmount) {
		return SDL_realloc(p_Data, p_NewAmount);
	}
	void Free(void* p_Data) {
		SDL_free(p_Data);
	}
	void Copy(const void* p_Source, void* p_Output, memsize_t p_CopyAmount) {
		SDL_memcpy(p_Output, p_Source, p_CopyAmount);
	}
	void Set(void* p_Source, unsigned __int8 p_Value, memsize_t p_Amount) {
		SDL_memset(p_Source, p_Value, p_Amount);
	}
	void ZeroMemory(void* p_Source, memsize_t p_Size) {
		Set(p_Source, 0x00, p_Size);
	}
}

CORE_END_NAMESPACE