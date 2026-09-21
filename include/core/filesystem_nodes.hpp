#pragma once

#define CO_F_PACKED 1
#define CO_F_LOWMEM 1

#ifdef CO_F_PACKED

#include <core/filesystem.hpp>

CORE_DECLARE_NAMESPACE

namespace Node {
	struct Node;
	

	struct FileInfo {
		FileSystem::fsize_t size = 0;
#if CO_F_LOWMEM
		FileSystem::fsize_t packIndex = 0;
#endif
		u8* data = nullptr;
	};

	/* Node
	* In order to get the contents of the file,
	* Subtract the last pointer's address by the folder's node address and so forth to the get amount
	* lastPointer can also be a ND_File pointer
	*/

	struct Node {
		bool isFolder = false;
		char name[9] = "invalid";
		void* lastPointer = nullptr;
	};


	void LoadFile() {
		FileSystem::File f;
	}
}


CORE_END_NAMESPACE

#endif