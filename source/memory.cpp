#include <core/memory.hpp>
#include <core/platform.hpp>
#if CORE_PLATFORM_MODERN
#include <SDL3/SDL.h>
#else
#include <memory>
#endif

#if CORE_MEMORY_USAGE
#define MEM(toDo) cl::mem::g_MemUsage toDo
#else
#define MEM(toDo)
#endif

#define AS_KB(x) ((x) * 1024)
#define AS_MB(x) ((x) * 1024 * 1024)
#define AS_GB(x) ((x) * 1024 * 1024 * 1024)

CORE_DECLARE_NAMESPACE

namespace mem {

#if CORE_MEMORY_USAGE
	memsize_t g_MemUsage = 0;
#endif

	float GetMemoryUsage() {
		constexpr memsize_t max = ((4) * (memsize_t)1024 * 1024 * 1024);
		return static_cast<float>((float)g_MemUsage / (float)max) * 100;
	}

	void* Allocate(memsize_t p_Amount) {
		//return ::operator new(p_Amount);
		MEM(+= p_Amount);
#if CORE_PLATFORM_MODERN
		return SDL_malloc(static_cast<size_t>(p_Amount));
#else
		return std::malloc(static_cast<size_t>(p_Amount));
#endif
	}
	void* ZeroAllocate(memsize_t p_Amount) {
		MEM(+= p_Amount);
		//void* ret = ::operator new(p_Amount) {0x00};
		//Is this hacky?
		//return static_cast<void*>(new unsigned __int8[p_Amount] {0x00});
#if CORE_PLATFORM_MODERN
		return SDL_calloc(static_cast<size_t>(p_Amount), 1);
#else
		return std::calloc(static_cast<size_t>(p_Amount, 1));
#endif

	}
	void* Reallocate(void* p_Data, memsize_t p_NewAmount) {
		MEM(+= (p_NewAmount - sizeof(p_Data)));
#if CORE_PLATFORM_MODERN
		return SDL_realloc(p_Data, static_cast<size_t>(p_NewAmount));
#else
		return std::realloc(p_Data, static_cast<size_t>(p_NewAmount));
#endif
	}
	void Free(void* p_Data) {
		MEM( -= sizeof(p_Data));
#if CORE_PLATFORM_MODERN
		SDL_free(p_Data);
#else
		std::free(p_Data);
#endif
		
	}
	void Copy(const void* p_Source, void* p_Output, memsize_t p_CopyAmount) {
#if CORE_PLATFORM_MODERN
		SDL_memcpy(p_Output, p_Source, p_CopyAmount);
#else
		std::memcpy(p_Output, p_Source, p_CopyAmount);
#endif
		
	}
	void Set(void* p_Source, unsigned __int8 p_Value, memsize_t p_Amount) {
#if CORE_PLATFORM_MODERN
		SDL_memset(p_Source, p_Value, p_Amount);
#else
		std::memset(p_Source, p_Value, p_Amount);
#endif
		
	}
	void ZeroMemory(void* p_Source, memsize_t p_Size) {
		Set(p_Source, 0x00, p_Size);
	}
}

CORE_END_NAMESPACE