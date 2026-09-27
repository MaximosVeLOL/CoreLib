#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

/* Filesystem
* Replaces C or SDL3 filesystems with a custom one
* designed for using asset apis (texture/yay.png instead of C:\game\assets\texture\yay.png)
* Replaces the need for operating system APIs, and uses a custom filesystem instead

*/

#include <core/types.hpp>
#include <core/libapi.hpp>

#define USE_CPP_FILEAPI 1

#if USE_CPP_FILEAPI
#include <fstream>
#else
#include <stdio.h>
#endif

CORE_DECLARE_NAMESPACE

namespace FileSystem {

	//In order to distinguish gamepaths from real paths, here is a simple solution
	//Check the first char at the start and check if it is 'C'
	//But what if other paths don't include it?
	//I guess we can check if it isn't capital?s

	enum OpenMode : u8 {
		F_OPEN_IMPORT = 0 << 0,
		F_OPEN_EXPORT = 1 << 0,

		F_OPEN_BINARY = 0 << 1,
		F_OPEN_TEXT = 1 << 1,
	};
	enum SeekBase : u8 {
		F_SEEK_START = 0,
		F_SEEK_CURRENT = 1,
		F_SEEK_END = 2,
	};

	using fsize_t = u32;
#if USE_CPP_FILEAPI
	class CFile {
	private:
		fsize_t m_Size = 0;
		std::fstream m_Stream;
	public:
		void Open(const char* p_Directory, OpenMode p_OpenMode = F_OPEN_IMPORT) {
			std::ios_base::openmode mode = m_Stream.in;
			if (p_OpenMode & F_OPEN_IMPORT) {
				//Already set
			}
			else if (p_OpenMode & F_OPEN_EXPORT) {
				mode |= m_Stream.out;
			}
			if (p_OpenMode & F_OPEN_BINARY) {
				mode |= m_Stream.binary;
			}
			else if (p_OpenMode & F_OPEN_TEXT) {
				//Already set & impossibe to set
			}
			m_Stream.open(p_Directory, mode);
			if (p_OpenMode & F_OPEN_IMPORT) {
				m_Stream.seekg(0, m_Stream.end);
				m_Size = static_cast<fsize_t>(m_Stream.tellg());
				m_Stream.seekg(0, m_Stream.beg);
			}

		}

		//Utilities
		fsize_t GetSize() {
			return m_Size;
		}
		bool IsOpen() {
			return m_Stream.is_open();
		}

		//Reading
		template<typename T>
		T Read() {
			T ret = T();
			//char* ret = new char[sizeof(T)];
			m_Stream.read(reinterpret_cast<char*>(ret), sizeof(T));
			return ret;
		}
		u8* Read(fsize_t p_Amount) {
			u8* ret = new u8[p_Amount];
			m_Stream.read(reinterpret_cast<char*>(ret), p_Amount);
			return ret;
		}
		char* ReadText(fsize_t p_Amount) {
			char* ret = new char[p_Amount + 1];
			m_Stream.read(ret, p_Amount);
			ret[p_Amount] = '\0';
			return ret;
		}

		//Writing
		template<typename T>
		void Write(T p_Value) {
			m_Stream.write(reinterpret_cast<char*>(&p_Value), sizeof(T));
		}
		void Write(void* p_Value, fsize_t p_Size) {
			m_Stream.write(reinterpret_cast<const char*>(p_Value), p_Size);
		}

		size_t Tell() {
			return static_cast<size_t>(m_Stream.tellg());
		}
		void Seek(size_t p_Offset, SeekBase p_Base) {
			std::ios_base::seekdir dir;
			switch (p_Base) {
			case F_SEEK_START:
				dir = m_Stream.beg;
				break;

			case F_SEEK_CURRENT:
				dir = m_Stream.cur;
				break;

			case F_SEEK_END:
				dir = m_Stream.end;
				break;
			}
			m_Stream.seekg(p_Offset, dir);
		}
		void Close() {
			m_Stream.flush();
			m_Stream.close();
			m_Size = 0;
		}
		CFile() {}
		CFile(const char* p_Directory, OpenMode p_OpenMode) {
			Open(p_Directory, p_OpenMode);
		}
		~CFile() {
			Close();
		}
	};
#else

	class CFile {
	private:
		fsize_t m_Size = 0;

	public:

	};
#endif
}

CORE_END_NAMESPACE


#endif