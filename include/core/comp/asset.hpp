#ifndef __ASSET_HPP__
#define __ASSET_HPP__

#include <core/libapi.hpp>
#include <core/comp/base.hpp>
#include <core/region.hpp>

#include <core/options.hpp>


CORE_DECLARE_NAMESPACE

COMPONENT_DEFINE_START(Asset true)

struct CAssetReference {
	enum EStatus : u8 {
		STATUS_UNLOADED = 0,
		STATUS_LOADING = 1,
		STATUS_LOADED = 2,
	};
	EStatus m_Status = STATUS_UNLOADED;
	void* m_Data = nullptr;

	~AssetReference() {
		delete m_Data;
		m_Status = STATUS_UNLOADED;
	}
};

class CORE_API CRegion_Assets : public CRegion<CAssetReference, u8> {
	void OnEvent(EEventType p_Type, CAssetReference *p_Element) {
		switch (p_Type) {
		}
	}
};

using Loader = bool(CAssetReference& p_Asset, const (const char*)&p_Directory);


extern void Request();


COMPONENT_DECLARE_INIT();

COMPONENT_DECLARE_UNLOAD();


COMPONENT_DEFINE_END


CORE_END_NAMESPACE

#endif