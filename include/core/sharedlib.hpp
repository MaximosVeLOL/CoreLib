#include <core/common.hpp>
#include <core/libapi.hpp>

//SharedLib - Simple wrapper for shared objects to ensure support for all platforms (cuz SDL3 doesn't support all platforms, how dumb!)


CORE_DECLARE_NAMESPACE

namespace sl {

	using generic_func_t = void(*)();

	struct CORE_API CLibrary {
		void* m_LibData = nullptr;
		
		template<typename Func>
		Func Find(const char* p_Name) {
			return static_cast<Func>(FindGeneric(p_Name));
		}
		
		template<typename T>
		T DataAs() {
			return static_cast<T>(m_LibData);
		}
		
		
		generic_func_t FindGeneric(const char* p_Name);
		void Unload();
		bool IsValid();

		CLibrary(){}
		CLibrary(const char* p_GoodPath);
	};
}

CORE_END_NAMESPACE