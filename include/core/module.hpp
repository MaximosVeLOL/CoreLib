#ifndef __MODULE_HPP__
#define __MODULE_HPP__

//Not to be confused with CMake modules and C++ modules, 
//This is meant for external connections with another application, but removes the need for DLL functions and all that complicated stuff.


#include <core/libapi.hpp>


#include <core/platform.hpp>
#include <core/options.hpp>

#if CO_FS_FIXEDCODE == 0
#if CORE_PLATFORM_MODERN
struct SDL_SharedObject;
#else

#endif
#endif

CORE_DECLARE_NAMESPACE

namespace Module {
	class CORE_API IModule {
	public:
#if CO_FS_FIXEDCODE == 0
#if CORE_PLATFORM_MODERN
		SDL_SharedObject* m_Object = nullptr;
#else

#endif
#endif
		bool m_Active : 4 = true;
		bool m_Visible : 4 = true;
		//Returns true if we successfully initialized
		virtual bool OnInit() = 0;

		virtual void OnUpdate() = 0;

		virtual void OnRender() = 0;
		//Prepares for unloading. Is called when we fail to load.
		virtual void OnUnload() = 0;
		
		void Unload();
	};

	extern CORE_API IModule* Load(const char* p_Name);
}

#define CORE_MODULE_CREATE_ACCESS(name, className) extern "C" __declspec(dllexport) Module::IModule* CoreModule_Get##name##() {return new className();}

CORE_END_NAMESPACE


#endif