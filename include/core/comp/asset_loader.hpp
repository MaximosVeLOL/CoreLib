#ifndef __ASSET_LOADER_HPP__
#define __ASSET_LOADER_HPP__

#include <core/types.hpp>
#include <core/libapi.hpp>
#include <core/comp/asset.hpp>

CORE_DECLARE_NAMESPACE

namespace Asset {
#pragma pack(push, 8)
	struct ILoader {
		void* m_DefaultData = nullptr;
		virtual void Initialize() = 0;
		virtual void Load(CAsset* p_Asset, FileSystem::CFile& p_File) = 0;
		virtual void Unload(CAsset* p_Asset) = 0;
	};
#pragma pack(pop)
}


CORE_END_NAMESPACE

#endif