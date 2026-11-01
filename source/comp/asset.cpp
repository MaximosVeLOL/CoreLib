#include <core/comp/asset.hpp>
#include <core/comp/asset_loader.hpp>

CORE_DECLARE_NAMESPACE

//Do we always fail to load the asset?
constexpr bool debug_alwaysFail = false;

namespace Asset {

	CAsset* g_LoadedAssets = nullptr;
	u8 g_LoadedAssetsCount = 0;

	void _doOptimization(u16 p_Hash) {
		for (refcount_t i = 0; i < g_LoadedAssetsCount;i++) {
			if (g_LoadedAssets[i].m_Hash == p_Hash) {
				for (refcount_t j = i; j < --g_LoadedAssetsCount;j++) {
					g_LoadedAssets[j] = g_LoadedAssets[j + 1];
				}
				return;
			}
		}
		LogError("(Asset:_doOptimization) Failed to find hash called ({}!)", p_Hash);
		throw std::runtime_error("Failed to find hash!");
	}

	void(*g_OnInit)() = nullptr;

	void CAsset::Use() {
		if (static_cast<u16>(m_RefCount + 1) > 0xFF) {
			m_RefCount = 0xFF;
			return;
		}
		m_RefCount++;
	}


	void CAsset::StopUsing() {
		if (static_cast<s16>(--m_RefCount) <= 0) {
			CAsset::Free();
		}
	}

	void CAsset::Free(bool p_DoOptimization) {
		m_Loader->Unload(this);
		if (p_DoOptimization) [[likely]] {
			_doOptimization(m_Hash);
			return;
		}
		//Just incase...
		m_Data = nullptr;
		m_Loader = nullptr;
		m_RefCount = 0;
		m_Hash = 0;
		
	}

	void CReference::Free() {
		g_LoadedAssets[m_Index].StopUsing();
	}

	CReference::~CReference() {
		//Free();
	}




	CReference Request(FileSystem::CPath p_Path, ILoader* p_Loader) {
		char* gamePath = p_Path.AsGamePath();
		if (g_LoadedAssetsCount > 0) {
			u16 hash = String::GetHash<u16>(gamePath);
			for (refcount_t i = 0; i < g_LoadedAssetsCount;i++) {
				if (hash == g_LoadedAssets[i].m_Hash) {
					return CReference{ i };
				}
			}
		}
		//Check if we have enough space for a new asset
		if (static_cast<u16>(g_LoadedAssetsCount + 1) >= ASSETS_MAX) {
			LogError("(Asset::Request) Asset limit exceeded!");
			return CReference();
		}
		FileSystem::CFile *f = new FileSystem::CFile(gamePath);
		
		if (!f->IsOpen()) {
			LogWarning("(Asset::Request) Failed to load asset! (filesystem error!, failed to open dir ({}))", gamePath);

			g_LoadedAssets[g_LoadedAssetsCount].m_Data = p_Loader->m_DefaultData;
			g_LoadedAssets[g_LoadedAssetsCount].m_Hash = String::GetHash<u16>(gamePath);
			g_LoadedAssets[g_LoadedAssetsCount].m_Loader = p_Loader;
			delete[] gamePath;
			delete f;
			return CReference{ g_LoadedAssetsCount++ }; //CReferences are invalid by default, change when that is changed
		}
		CAsset *curAsset = &g_LoadedAssets[g_LoadedAssetsCount];
		curAsset->m_Data = p_Loader->m_DefaultData;
		curAsset->m_Hash = String::GetHash<u16>(gamePath);
		curAsset->m_Loader = p_Loader;
#if CO_O_MULTI_THREADING
		std::thread t([p_Loader, curAsset, f]() -> void {
			p_Loader->Load(curAsset, *f);
			f->Close();
			delete f;
			});
		t.detach();
#else
		p_Loader->Load(curAsset, f);
#endif
		delete[] gamePath;
		return CReference{ g_LoadedAssetsCount++ }; 
	}


	COMPONENT_IMPLEMENT_INIT() {
		g_LoadedAssets = new CAsset[ASSETS_MAX]{CAsset()};
		if (g_OnInit)
			g_OnInit();
		return true;
	}

	COMPONENT_IMPLEMENT_UNLOAD() {
		for (refcount_t i = 0; i < g_LoadedAssetsCount;i++) {
			g_LoadedAssets[i].Free();
		}
	}
}


CORE_END_NAMESPACE