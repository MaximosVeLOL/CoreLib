#ifndef __STRING_H__
#define __STRING_H__

#include <core/libapi.hpp>
#include <format>
#include <core/logging.hpp>
#include <core/memory.hpp>
#include <core/types.hpp>
#include <core/error.hpp>

#include <core/list.hpp> //For CStringList

CORE_DECLARE_NAMESPACE


#ifdef _UNICODE
using char_t = wchar_t;
#else
using char_t = char;
#endif

using strsize_t = u16;

constexpr strsize_t STRING_INVALID = 65535;

namespace String {
	CORE_API extern strsize_t FindLength(const char_t* p_String);
	template<typename... Args>
	inline char_t* MakeFromFormat(std::format_string<Args...> p_Format, Args&&... p_Args) {
		std::string f = std::format(p_Format, p_Args...);
		size_t fSize = f.size();
		char_t* ret = new char_t[fSize] {'!'};
		Mem::Copy(f.data(), ret, fSize);
		return ret;
	}
	template<typename T = u16>
	inline T GetHash(const char_t* p_String) {
		u8 index = 0;
		T ret = 0;
		while (p_String[index]) {
			ret += p_String[index];
			index++;
		}
		return ret;
	}
	CORE_API extern bool Compare(const char_t* p_String, const char_t* p_Check, const strsize_t p_EndIndex);
	CORE_API extern bool Compare(const char_t* p_String, const char_t* p_Check);
}

//Static const char_t* wrapper, or char_t*
class CORE_API CString_Simple {
protected:
	bool m_IsConst = true;
	//Have this be null, because const char_t* cannot be converted to char_t*, but char_t* can be converted to const char_t*
	char_t* m_Data = nullptr;
	strsize_t m_Size = 0; //Size = length + 1
public:
	//Returns a const char_t* if it is constant.
	const char_t* data_const() {
		if (!m_IsConst)
			throw Error::CRuntime("The string's data is not constant. (expected constant)");
		return m_Data;
	}
	//Returns a char_t* if it is not constant.
	char_t* data() {
		if (m_IsConst)
			throw Error::CRuntime("The string's data is constant. (expected non-constant)");
		return m_Data;
	}
	const char_t* data_safe() {
		return m_Data;
	}

	strsize_t length() {
		if (m_Size == 0)
			return 0;
		return m_Size - 1;
	}
	strsize_t size() {
		return m_Size;
	}

	char_t& at(strsize_t p_Index) {
		return static_cast<char_t&>(m_Data[p_Index]);
	}
	
	char_t operator[](strsize_t p_Index) {
		return m_Data[p_Index];
	}

	char* substring(strsize_t p_StartIndex, strsize_t p_EndIndex = STRING_INVALID, bool p_EndIndexIsLength = false) {
		if (p_EndIndexIsLength) {
			if (p_EndIndex == STRING_INVALID) {
				p_EndIndex = (m_Size - 1) - p_StartIndex;
			}
			if (p_StartIndex + p_EndIndex >= m_Size) {
				throw Error::CExOutOfBounds();
			}
			char* ret = new char[p_EndIndex + 1] {'\0'};
			Mem::Copy((m_Data + p_StartIndex), ret, p_EndIndex);
			
			return ret;
		}
		if (p_EndIndex == STRING_INVALID) {
			p_EndIndex = m_Size - 1;
		}
		strsize_t len = p_EndIndex - p_StartIndex;
		if (len == 0) {
			throw Error::CRuntime("substring length is 0! (start & end = ({}, {}))", p_StartIndex, p_EndIndex);
		}
		char* ret = new char[len + 1] {'\0'};
		Mem::Copy((m_Data + p_StartIndex), ret, len);
		return ret;
	}
	strsize_t find(const char p_Char, strsize_t p_Offset = 0) {
		strsize_t len = length();
		for (;p_Offset < len;p_Offset++) {
			if (m_Data[p_Offset] == p_Char)
				return p_Offset;
		}
		return STRING_INVALID;
	}
	strsize_t find(const char* p_Data, strsize_t p_Offset = 0) {
		strsize_t len = String::FindLength(p_Data);
		strsize_t match = 0;
		for (;p_Offset < len;p_Offset++) {
			if (m_Data[p_Offset] == p_Data[match]) {
				if (++match >= len) {
					return p_Offset;
				}
				continue;
			}
			match = 0;
		}
		return STRING_INVALID;
	}
	bool contains(const char* p_Value) {
		return (find(p_Value) != STRING_INVALID);
	}

	virtual void operator=(char_t* p_Data) {
		m_IsConst = false;
		m_Data = p_Data;
		m_Size = String::FindLength(p_Data);
	}
	virtual void operator=(const char_t* p_Data) {
		m_IsConst = true;
		m_Data = const_cast<char_t*>(p_Data);
		m_Size = String::FindLength(p_Data);
	}
	
	CString_Simple() {}
	CString_Simple(char_t* a) {
		operator=(a);
	}
	CString_Simple(const char_t* a) {
		operator=(a);
	}
};
class CORE_API CString : public CString_Simple {
private:
	//Custom implemented based off of CList's _extend()
	void _extend(unsigned __int8 p_ElementCount, char_t* p_NewElements = nullptr) {
		if (static_cast<size_t>(static_cast<size_t>(m_Size) + p_ElementCount) >= STRING_INVALID) {
			throw("m_Size exceeded 65535!");
		}
		//This is a char_t*
		//p_ElementCount is always 1
		//p_NewElements = char_t*
		if (p_ElementCount == 1) {
			if (!m_Data) {
				m_Data = new char_t[2] {0x00};
				m_Data[0] = *p_NewElements; //The char_t* is not a char_t[]!
				m_Size = 2;
				return;
			}
			char_t* output = new char_t[m_Size + 1]; //Since m_Size should include a null terminator, only increase it by one
			strsize_t i = 0;
			for (; i < m_Size - 1;i++) {
				output[i] = m_Data[i];
			}
			//i = m_Size
			output[i] = *p_NewElements;
			output[i + 1] = '\0';
			delete[] m_Data; //m_Data should always be a char_t[] pointer, since it always has a base char_t + a null terminator
			m_Data = output;
			m_Size += p_ElementCount;
			return;
		}
		//p_ElementCount = string length of p_NewElements
		//p_NewElements = a null terminated string

		if (!m_Data) {
			operator=(p_NewElements);
			//m_Data = p_NewElements;
			//m_Size = static_cast<strsize_t>(p_ElementCount) + 1;
			return;
		}
		//My size (length + '\0') + string length of new string
		//m_Size = my size (length + '\0)
		strsize_t myLen = (m_Size - 1);
		
		char_t* output = new char_t[myLen + p_ElementCount + 1 + 10] {0}; //m_Size includes the null terminator, but p_ElementCount should not, so this  
		strsize_t i = 0;
		
		for (; i < myLen;i++) { //Get all the data (except for the null terminator)
			output[i] = m_Data[i];
		}
		for (strsize_t j = 0; j < p_ElementCount;j++) { //Also remove the null terminator
			output[myLen + j] = p_NewElements[j];
		}
		output[myLen + p_ElementCount + 1] = '\0';

		delete[] m_Data;
		m_Data = output;
		m_Size += p_ElementCount;

	}
public:
	void remove(strsize_t p_Index, strsize_t p_Amount = 1) {
		if (!m_Data)
			throw Error::CRuntime("WTF!");
		if (m_Size - p_Amount <= 0) {
			delete[] m_Data;
			m_Data = nullptr;
			m_Size = 0;
			return;
		}
		strsize_t newLen = (m_Size - p_Amount - 1);
		char_t* output = new char_t[newLen + 1]{ '!' };
		for (strsize_t i = 0;i < p_Index;i++) {
			if (i < p_Index) {
				output[i] = m_Data[i];
				continue;
			}
			output[i] = m_Data[i + p_Amount];

		}
		output[newLen] = '\0';
		delete[] m_Data;
		m_Data = output;
		m_Size = newLen + 1;
	}

	void operator+=(const char_t* p_String) {
		_extend(String::FindLength(p_String), const_cast<char_t*>(p_String));
	}

	void operator+=(char_t p_char_t) {
		_extend(1, &p_char_t);
	}

	char_t& operator[](strsize_t p_Index) {
		return m_Data[p_Index];
	}

	char_t& at(strsize_t p_Index) {
		return m_Data[p_Index];
	}
	char_t& first() {
		return m_Data[0];
	}
	char_t& last() {
		return m_Data[1];
	}

	void operator=(char_t* p_String) override {
		if (m_Data) {
			delete[] m_Data;
		}
		m_Data = p_String;
		m_Size = String::FindLength(p_String);
	}

	void operator=(const char* p_Data) override {
		if (m_Data) {
			delete[] m_Data;
		}
		m_Size = String::FindLength(p_Data);
		m_Data = new char_t[m_Size + 1]{ '!' };
		Mem::Copy(p_Data, m_Data, m_Size);
		m_Data[m_Size++] = '\0';
	}
	bool operator==(const char* p_Data) {
		return String::Compare(m_Data, p_Data);
	}
	bool operator==(CString&& p_Data) {
		return String::Compare(m_Data, p_Data.m_Data);
	}
	CString operator+(const char* p_Other) {
		CString ret(m_Data);
		ret += p_Other;
		return ret;
	}
	CString operator+(CString&& p_Other) {
		CString ret(m_Data);
		ret += p_Other;
		return ret;
	}

	//Add new operators here


	u8 get_count(char_t p_LookFor, strsize_t p_Beginning = 0, strsize_t p_EndIndex = STRING_INVALID) {
		if (p_EndIndex == STRING_INVALID) {
			p_EndIndex = m_Size - 1;
		}
		u8 count = 0;
		for (strsize_t i = p_Beginning; i < p_EndIndex;i++) {
			if (m_Data[i] == p_LookFor) {
				count++;
			}
		}
		return count;
	}

	CString() { m_IsConst = false; }

	CString(char_t* p_String) {
		m_IsConst = false;
		operator=(p_String);
	}
	CString(const char_t* p_String) {
		m_IsConst = false;
		operator=(p_String);
	}


	template<typename... Args>
	CString(std::format_string<Args...> p_Format, Args&&... p_Args) {
		m_Data = String::MakeFromFormat(p_Format, p_Args...);
		m_Size = String::FindLength(m_Data);
	}

	CString(std::nullptr_t p_Null) {
		m_IsConst = true;
		m_Size = 0;
		m_Data = nullptr;
	}

	~CString() {
		//delete[] m_Data;
		//m_Data = nullptr;
		//m_Size = 0;
	}

	operator char_t* () {
		return m_Data;
	}
};

using CStringList = CList<CString>;
/*
//A custom list that includes many stupid stuff
class CStringList {

};
*/


CORE_END_NAMESPACE


#endif