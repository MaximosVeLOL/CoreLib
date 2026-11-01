#ifndef __MEMORY_HPP__
#define __MEMORY_HPP__

#include <core/libapi.hpp>
#include <memory>

CORE_DECLARE_NAMESPACE

namespace Mem {

	using memsize_t = unsigned __int32;
	using refcount_t = unsigned __int8;

	extern CORE_API void* Allocate(memsize_t p_Amount);
	extern CORE_API void* ZeroAllocate(memsize_t p_Amount);
	extern CORE_API void* Reallocate(void* p_Data, memsize_t p_NewAmount);
	extern CORE_API void Free(void* p_Data);
	extern CORE_API void Copy(const void* p_Source, void* p_Output, memsize_t p_CopyAmount);
	extern CORE_API void Set(void* p_Source, unsigned __int8 p_Value, memsize_t p_Amount);
	extern CORE_API void ZeroMemory(void* p_Source, memsize_t p_Size);

}

CORE_END_NAMESPACE

#endif