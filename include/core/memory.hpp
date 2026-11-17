#ifndef __MEMORY_HPP__
#define __MEMORY_HPP__

#include <core/libapi.hpp>
#include <core/logging.hpp>
#include <memory>

#define CORE_MEMORY_USAGE 1

CORE_DECLARE_NAMESPACE

namespace mem {

	using memsize_t = unsigned long long int;
	using refcount_t = unsigned __int8;
#if CORE_MEMORY_USAGE
	extern memsize_t g_MemUsage;
#endif
	/// <summary>
	/// Gets the memory used overrall compared to the maximum amount.
	/// </summary>
	/// <returns>The current memory usage as a percentage over 4 GB (or an overriden amount)</returns>
	extern CORE_API float GetMemoryUsage();

	/// <summary>
	/// Allocates memory the same way a normal alloc() function would work.
	/// </summary>
	/// <param name="p_Amount">The size of the requested memory block.</param>
	/// <returns>A pointer to the memory block, or nullptr if we failed to allocate it.</returns>
	extern CORE_API void* Allocate(memsize_t p_Amount);
	/// <summary>
	/// Allocates memory with the memory block containing all zeros the same way a normal calloc() function would work.
	/// </summary>
	/// <param name="p_Amount">The size of the requested memory block.</param>
	/// <returns>A pointer to the memory block, or nullptr if we failed to allocate it.</returns>
	extern CORE_API void* ZeroAllocate(memsize_t p_Amount);
	/// <summary>
	/// Reallocates a memory block to a larger or smaller size.
	/// </summary>
	/// <param name="p_Data">The existing memory block</param>
	/// <param name="p_NewAmount">The new size of the memory block</param>
	/// <returns>The new address of the memory block, or nullptr if we failed.</returns>
	extern CORE_API void* Reallocate(void* p_Data, memsize_t p_NewAmount);
	/// <summary>
	/// Frees a memory block and marks the memory block as nullptr.
	/// </summary>
	/// <param name="p_Data">The memory block</param>
	/// <returns></returns>
	extern CORE_API void Free(void* p_Data);
	/// <summary>
	/// Copies data from one memory block to another.
	/// </summary>
	/// <param name="p_Source">What to copy from</param>
	/// <param name="p_Output">The output of the copied data</param>
	/// <param name="p_CopyAmount">How many bytes you want to copy from the source (or the size of the source if you want all)</param>
	/// <returns></returns>
	extern CORE_API void Copy(const void* p_Source, void* p_Output, memsize_t p_CopyAmount);
	/// <summary>
	/// Sets a memory block's data to a specific value, up to p_Amount bytes.
	/// </summary>
	/// <param name="p_Source">The memory block</param>
	/// <param name="p_Value">The value to set all bytes to.</param>
	/// <param name="p_Amount">The amount of bytes to set the memory block to (or the size of the memory block for all)</param>
	/// <returns></returns>
	extern CORE_API void Set(void* p_Source, unsigned __int8 p_Value, memsize_t p_Amount);
	/// <summary>
	/// Same thing as Set(), but sets it to zero automatically.
	/// </summary>
	/// <param name="p_Source">The memory block</param>
	/// <param name="p_Size">How many bytes you want to set the memory block to.</param>
	/// <returns></returns>
	extern CORE_API void ZeroMemory(void* p_Source, memsize_t p_Size);
	
	/*
	template<typename Type, typename Count = refcount_t>
	class CPointer {
	private:
		Type* m_Pointer = nullptr;
		Count m_RefCount = 0;
		void _increaseRefCount() {
			unsigned long long int next = m_RefCount + 1;
			if (next >= static_cast<Count>(-1)) {
				return;
			}
			m_RefCount++;
		}
		void _decreaseRefCount() {
			if (m_RefCount - 1 <= 0) {
				delete m_Pointer;
				return;
			}
			m_RefCount--;

		}
	public:
		//Returns true if m_Pointer = nullptr
		bool is_nullptr() {
			return static_cast<bool>(m_Pointer);
		}
		//Returns is_nullptr()
		operator bool() {
			return is_nullptr();
		}


		//Operator wrappers
		operator Type() {
			_increaseRefCount();
			return *m_Pointer;
		}
		operator Type& () {
			_increaseRefCount();
			return *m_Pointer;
		}
		operator Type* () {
			_increaseRefCount();
			return m_Pointer;
		}
		operator void* () {
			_increaseRefCount();
			return static_cast<void*>(m_Pointer);
		}
		void operator=(Type*&& p_Value) {
			destroy();
			m_Pointer = p_Value;
		}

		void destroy(bool p_Array = false) {
			if (m_RefCount > 0) {
				LogWarning("(mem::CPointer::destroy) Trying to delete a used memory pointer! Ignoring...");
				return;
			}
			if (p_Array)
				delete[] m_Pointer;
			else
				delete m_Pointer;
			m_Pointer = nullptr;
			//m_RefCount = 0;
		}


		CPointer() {
			_increaseRefCount();
		}
		CPointer(Type* p_Value) : m_Pointer(p_Value) { m_RefCount = 1; }

		~CPointer() {
			_decreaseRefCount();
		}
	};
	*/
	template<typename T>
	using CPointer = std::shared_ptr<T>;

}
using memsize_t = mem::memsize_t;

CORE_END_NAMESPACE

#endif