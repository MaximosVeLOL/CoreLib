#pragma once

#include <core/options.hpp>

#if CO_FS_PACKED

#include <core/filesystem.hpp>

CORE_DECLARE_NAMESPACE

namespace FileSystem {
	namespace Packed {
		
		struct CFileInfo {
			u8 fileMarker = 67;
			fsize_t size = 0;
			u8* data = nullptr;
		};

		struct CNode {
			char name[25] = "/!invalid!/";
			//For files, this has a special struct,
			//Else, this contains a pointer to the last CNode.
			void* data = nullptr;
		};

		
	}
}


CORE_END_NAMESPACE

#endif