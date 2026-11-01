#include <core/module.hpp>
#include <core/filesystem.hpp>


#if CORE_PLATFORM_MODERN
#include <SDL3/SDL_loadso.h>
#endif
#if CO_FS_FIXEDCODE
#include <windows.h>
#endif
#include <core/error.hpp>


CORE_DECLARE_NAMESPACE

namespace Module {
	void IModule::Unload() {
		SDL_UnloadObject(m_Object);
	}

	IModule* Load(const char* p_Name) {
		constexpr bool CE_useCurrentPath = true;
		CString pathString;
		if (CE_useCurrentPath) {
			pathString += FileSystem::CPath::GetCurrentDirectory();
			pathString += '\\';
			pathString += p_Name;

		}
		else {

			CString pathString = "dev/tools/";
			pathString += p_Name;
		}
#ifdef WIN32 //Will the other option ever be available? No. But it is good to support other platforms pre-maturely
		pathString += ".dll";
#else
		pathString += ".so";
#endif
		FileSystem::CPath theRealPath(pathString);
		char_t* realPath = theRealPath.AsRealPath();
#if CO_FS_FIXEDCODE
#else


#if	CORE_PLATFORM_MODERN
		SDL_SharedObject* o = SDL_LoadObject(realPath);
		if (!o) {
			//const char_t* stupidHack = pathString.data();
			//LogError("(Module::Load) Failed to load module in path ({}!}", "todo");
			delete[] realPath;
			return nullptr;
		}
		delete[] realPath;

		using _modGetFunc = IModule * (*)();

		CString funcName = "CoreModule_Get";
		funcName += toupper(p_Name[0]);
		funcName += (p_Name + 1);
		const char_t* n = funcName.data();
		_modGetFunc getFunc = (_modGetFunc)SDL_LoadFunction(o, n);
#endif //modern platform

#endif //fixed code
		if (!getFunc) {
			LogError("(Module::Load) Failed to find function ({}) in shared object!", n);
			return nullptr;
		}

		IModule* ret = getFunc();
		if (!ret) {
			throw Error::CRuntime("We somehow failed to get the IModule! (Bug check for this SMH)");
		}
		ret->m_Object = o;
		return ret;
	}
}

CORE_END_NAMESPACE