#include <core/filesystem.hpp>
#include <filesystem>
#include <SDL3/SDL.h>

namespace fs = std::filesystem;

CORE_DECLARE_NAMESPACE

namespace FileSystem {

#pragma region CPath
	strsize_t CPath::_find(char p_Target) {
		for (s32 i = static_cast<s32>(m_Length); i >= 0;i--) {
			if (m_Path[i] == p_Target)
				return static_cast<strsize_t>(i);
		}
		return STRING_INVALID;
	}

	char* CPath::GetCurrentDirectory() {
		return SDL_GetCurrentDirectory();
	}

	const char* CPath::GetBaseDirectory() {
		return "C:/source/Magea2/vendored/CoreLib/makeshiftassets";
	}
	bool CPath::IsFolder() {
		//I think we should have all folder paths as "C:\path\" but we also need to consider "C:\path"
		//The thing is it is impossible to determine if it's a file or folder.
		//I think its best to put it on the user, as the user knows if it is a file or folder
		SDL_PathInfo i;
		SDL_GetPathInfo(m_Path, &i);
		return (i.type == SDL_PATHTYPE_DIRECTORY);
		//return (m_Path[m_Length] == '/' || m_Path[m_Length] == '\\');
	}
	bool CPath::IsFile() {
		SDL_PathInfo i;
		SDL_GetPathInfo(m_Path, &i);
		return (i.type == SDL_PATHTYPE_FILE);
		/*
		//This looks for files with extensions, which is very common, but some only have names.
		strsize_t extIndex = _find('.');
		if (extIndex == STRING_INVALID) {
			return false;
		}
		for (;extIndex < m_Length;extIndex++) {
			if (m_Path[extIndex] == '/') { //We have dots in our folder names (C:/My.File.Path/)
				return false;
			}
		}
		*/
	}


#pragma endregion

#pragma region CFile

	void CFile::Open(const char* p_Directory, EOpenMode p_OpenMode) {
		char openMode[2] = "!";
		u8 openIndex = 0;
		if (p_OpenMode.doText) {
		}
		else {
			openMode[0] = 'b';
			openIndex++;
		}
		if (p_OpenMode.doImport) {
			openMode[openIndex] = 'r';
		}
		else {
			openMode[openIndex] = 'w';
		}
		CPath p(p_Directory);
#if CORE_PLATFORM_MODERN
		const char* realPath = p.AsRealPath();
		m_Stream = SDL_IOFromFile(realPath, openMode);
		if (!m_Stream) {
			LogWarning("(CFile::Open) Failed to open file in directory ({})!", realPath);
		}
		if (p_OpenMode.doImport) {
			SDL_SeekIO(m_Stream, 0, SDL_IO_SEEK_END);
			m_Size = static_cast<fsize_t>(SDL_TellIO(m_Stream));
			//rewind(m_Stream);
			SDL_SeekIO(m_Stream, 0, SDL_IO_SEEK_SET);
		}
#endif

	}

	fsize_t CFile::GetSize() {
		return m_Size;
	}
	bool CFile::IsOpen() {
		return static_cast<bool>(m_Stream); //Returns false if m_Stream is nullptr
	}
	u8* CFile::Read(fsize_t p_Amount) {
		u8* ret = new u8[p_Amount];
		SDL_ReadIO(m_Stream, reinterpret_cast<void*>(&ret), p_Amount);
		return ret;
	}
	void CFile::ReadTo(void* p_Data, fsize_t p_Amount) {
		SDL_ReadIO(m_Stream, p_Data, p_Amount);
	}
	char* CFile::ReadText(fsize_t p_Amount) {
		char* ret = new char[p_Amount + 1];
		SDL_ReadIO(m_Stream, reinterpret_cast<void*>(&ret), p_Amount);
		ret[p_Amount] = '\0';
		return ret;
	}

	void CFile::Write(void* p_Value, fsize_t p_Size) {
		SDL_WriteIO(m_Stream, p_Value, p_Size);
	}

	fsize_t CFile::Tell() {
		return static_cast<fsize_t>(SDL_TellIO(m_Stream));
	}
	void CFile::Seek(fsize_t p_Offset, SeekBase p_Base) {
		//Since SeekBase matches the SDL_SEEK_ defines, we can use them here instead of converting them.
		SDL_SeekIO(m_Stream, static_cast<Sint64>(p_Offset), static_cast<SDL_IOWhence>(p_Base));
	}
	void CFile::Close() {
		SDL_FlushIO(m_Stream);
		SDL_CloseIO(m_Stream);
		m_Stream = nullptr;
		m_Size = 0;
	}



#pragma endregion

	namespace Utility {
		bool DirectoryIterator(CPath p_Path, void(*p_Iterator)(const CPath& p_Current)) {
			char* asRaw = p_Path.AsRealPath();
			if (!asRaw) {
				throw Error::CRuntime("asd");
			}
			try {
				for (const fs::directory_entry& e : fs::directory_iterator(asRaw)) {
					fs::path path = e;
					std::string asString = path.string();
					CPath cur(asString.data());
					p_Iterator(cur);
				}
			}
			catch (std::exception e) { //Use std::exception because we are using std library
				//LogError("(FileSystem::Utility::DirectoryIterator) Failed to iterate in a file! ({})", e.what());
				delete[] asRaw;
				return false;
			}
			delete[] asRaw;
			return true;
		}

		bool CreateFolder(CPath p_Path) {
#if CO_FS_PACKED
			throw Error::CRuntime("Trying to create a folder in a packed filesystem!");
#else
			char* real = p_Path.AsRealPath();
			SDL_CreateDirectory(real);
			delete[] real;
#endif
			return true;
		}
	}

}

CORE_END_NAMESPACE