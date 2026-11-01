#ifndef __ASSET_HPP__
#define __ASSET_HPP__

#include <core/libapi.hpp>
#include <core/comp/base.hpp>
#include <core/region.hpp>

#include <core/options.hpp>
#include <core/filesystem.hpp>


CORE_DECLARE_NAMESPACE

COMPONENT_DEFINE_START(Asset, true)



using refcount_t = u8;

constexpr refcount_t ASSETS_MAX = 0xFE;

struct ILoader;

struct CORE_API CAsset {
	void* m_Data = nullptr;
	ILoader* m_Loader = nullptr;
	u8 m_RefCount = 0;
	u16 m_Hash = 0; //Use with CString::AsHash()!

	u8 _unused[4] = { 0x00 };

	void Use();
	void StopUsing();
	void Free(bool p_DoOptimization = true);


};

extern CORE_API CAsset* g_LoadedAssets;



struct CORE_API CReference {
	refcount_t m_Index = 0xFF;
	//Templates have to be defined in its declaraction!
	template<typename T>
	T* GetData() {
		g_LoadedAssets[m_Index].Use();
		return static_cast<T*>(g_LoadedAssets[m_Index].m_Data);
	}
	void Free();

	bool IsValid() {
		return (m_Index < 0xFF);
	}
	operator bool() {
		return IsValid();
	}

	~CReference();
};


extern CORE_API CReference Request(FileSystem::CPath p_Path, ILoader* p_Loader);

extern CORE_API void Unload(const CReference& p_Ref);

extern CORE_API void(*g_OnInit)();


COMPONENT_DECLARE_INIT();

COMPONENT_DECLARE_UNLOAD();

COMPONENT_DEFINE_END


CORE_END_NAMESPACE

#endif