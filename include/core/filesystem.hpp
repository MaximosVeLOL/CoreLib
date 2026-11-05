#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

/* Filesystem
* Replaces C or SDL3 filesystems with a custom one
* designed for using asset apis (texture/yay.png instead of C:\game\assets\texture\yay.png)
* Replaces the need for operating system APIs, and uses a custom filesystem instead

*/

#include <core/types.hpp>
#include <core/libapi.hpp>

#include <core/string.hpp>

#include <core/memory.hpp>
#include <core/platform.hpp>

#include <functional>

#if CORE_PLATFORM_MODERN
struct SDL_IOStream;
#endif

CORE_DECLARE_NAMESPACE

namespace FileSystem {
	//A path, can be a asset directory path or a real (binary) directory.
	//This should only be used when you need basic utilities or don't know
	//The type a path is.
	class CORE_API CPath {
	private:
		strsize_t _find(char p_Target);
		/*
		strsize_t _getExtensionIndex() {
			//A regular file can be this: "./.gitignore"
			//So we have to go from the end to the start to solve for the extension
			//The extension is also the last dot at the end, so that makes even more sense

		}
		strsize_t _getParentFolderIndex() {
			for (s32 i = static_cast<s32>(m_Length); i >= 0;i--) {
				if (m_Path[i] == '/') {
					return static_cast<strsize_t>(i);
				}
			}
		}
		*/
	public:
		const char* m_Path = nullptr;
		strsize_t m_Length = 0;

		static char* GetCurrentDirectory();

		static const char* GetBaseDirectory();


		const char* ParentFolder() {
			strsize_t folderIndex = _find('/');
			if (folderIndex == STRING_INVALID) folderIndex = _find('\\');
			if (folderIndex == STRING_INVALID) return nullptr;
			return static_cast<const char*>(m_Path + folderIndex);
		}

		const char* Extension() {
			strsize_t extIndex = _find('.');
			if (extIndex == STRING_INVALID) return nullptr;
			return static_cast<const char*>(m_Path + extIndex);
		}
		char* FileName() {
			strsize_t parentFolder = _find('/');
			//If there is no folder "name.ext", then assume the filename starts at 0
			if (parentFolder == STRING_INVALID) parentFolder = 0;
			strsize_t extIndex = _find('.');
			//Assume the path is "filename"
			if (extIndex == STRING_INVALID) extIndex = m_Length;
			char* ret = new char[(extIndex - parentFolder) + 1] {'\0'};
			for (strsize_t i = parentFolder + 1; i < extIndex;i++) {
				ret[i - (parentFolder + 1)] = m_Path[i];
			}
			return ret;
		}

		bool IsFolder();
		bool IsFile();

		bool IsRealPath() {
			return (m_Path[0] == 'C');
		}
		bool IsGameAssetPath() {
			return (m_Path[0] != 'C');
		}


		//Converts game asset paths (sys/INPUTS to real paths C:\game\assets\sys\INPUTS)
		char* AsRealPath() {
			if (IsRealPath()) {
				//Just a little hack here!
				char* ret = new char[m_Length + 1] {'\0'};
				Mem::Copy(static_cast<const void*>(m_Path), ret, m_Length);
				return ret; 
			}

			const char* baseDir = GetBaseDirectory();
			strsize_t baseDirLen = String::FindLength(baseDir);
			char* ret = new char[baseDirLen + m_Length + 8 + 1] {'\0'};
			strsize_t i = 0;
			for (; i < baseDirLen;i++) {
				ret[i] = baseDir[i];
			}
			ret[baseDirLen] = '/';
			ret[baseDirLen + 1] = 'a';
			ret[baseDirLen + 2] = 's';
			ret[baseDirLen + 3] = 's';
			ret[baseDirLen + 4] = 'e';
			ret[baseDirLen + 5] = 't';
			ret[baseDirLen + 6] = 's';
			ret[baseDirLen + 7] = '/';
			for (i = 0; i < m_Length;i++) {
				ret[baseDirLen + 8 + i] = m_Path[i];
			}
			return ret;
			//return CString::MakeCharArrayFromFormat("{}/assets/{}", GetBaseDirectory(), m_Path);
		}
		char* AsGamePath() {
			if (IsGameAssetPath()) {
				//Just a little hack here! (x2)
				char* ret = new char[m_Length + 1] {'\0'};
				Mem::Copy(static_cast<const void*>(m_Path), ret, m_Length);
				return ret;
			}
			throw("Implement!");
			strsize_t baseIndex = 0;
			const char* baseDir = GetBaseDirectory();
			strsize_t len = String::FindLength(baseDir);
			for (strsize_t i = 0; i < m_Length;i++) {
				if (m_Path[i] == baseDir[baseIndex]) {
					if (++baseIndex >= len) {
						//We found the beginning index!
					}
				}
			}
		}

		CPath(const char* p_Directory) : m_Path(p_Directory) {
			m_Length = String::FindLength(p_Directory);
		}
		~CPath() {
		}
	};

	//In order to distinguish gamepaths from real paths, here is a simple solution
	//Check the first char at the start and check if it is 'C'
	//But what if other paths don't include it?
	//I guess we can check if it isn't capital?s
	struct EOpenMode {
		bool doImport : 4 = 0;
		bool doText : 4 = 0;
	};
	enum SeekBase : u8 {
		F_SEEK_START = 0,
		F_SEEK_CURRENT = 1,
		F_SEEK_END = 2,
	};

	using fsize_t = u32;

class CORE_API CFile {
private:
	fsize_t m_Size = 0;
	SDL_IOStream* m_Stream = nullptr;
public:
	void Open(const char* p_Directory, EOpenMode p_OpenMode = { true, true });

	//Utilities
	fsize_t GetSize();
	bool IsOpen();
	SDL_IOStream* GetStream() {
		return m_Stream;
	}

	//Reading
	template<typename T>
	T Read() {
		T ret = T();
		//char* ret = new char[sizeof(T)];
		ReadTo(static_cast<void*>(&ret), sizeof(T)); //Smart hack?
		return ret;
	}
	u8* Read(fsize_t p_Amount);
	void ReadTo(void* p_Data, fsize_t p_Amount);
	char* ReadText(fsize_t p_Amount);

	//Writing
	template<typename T>
	void Write(T p_Value) {
		Write(static_cast<void*>(&p_Value), sizeof(p_Value));
		//SDL_WriteIO(m_Stream, reinterpret_cast<void*>(&p_Value), sizeof(T));
	}
	void Write(void* p_Value, fsize_t p_Size);

	fsize_t Tell();
	void Seek(fsize_t p_Offset, SeekBase p_Base);
	void Close();
	CFile() {}
	CFile(const char* p_Directory, EOpenMode p_OpenMode = {true, true}) {
		Open(p_Directory, p_OpenMode);
	}
	~CFile();
};

namespace Utility {
	CORE_API extern bool DirectoryIterator(CPath p_Path, void(*p_Iterator)(CPath& p_Current));

	CORE_API extern bool CreateFolder(CPath p_Path);

	CORE_API extern char* OpenFileAndReadAll(CPath p_Path, strsize_t& p_OutSize);
}


namespace Dialog {
	struct CFilter {

	};

	inline void File(std::function<void(const CPath&)> p_OnSelect, CPath p_DefaultPath = nullptr) {

	}
}

} //End of filesystem namespace

CORE_END_NAMESPACE


#endif