#ifndef __STRING_H__
#define __STRING_H__

#include <core/libapi.hpp>
#include <format>
#include <core/logging.hpp>
#include <core/memory.hpp>
#include <core/types.hpp>
#include <core/error.hpp>

#include <core/list.hpp> //For CStringList

#include <core/options.hpp> //For optimization functions

CORE_DECLARE_NAMESPACE


#ifdef _UNICODE
using char_t = wchar_t;
#else
using char_t = char;
#endif

using strlen_t = u16;


constexpr strlen_t STRING_INVALID = 65535;

namespace String {
	CORE_API extern strlen_t FindLength(const char_t* p_String);
	template<typename... Args>
	inline char_t* MakeFromFormat(std::format_string<Args...> p_Format, Args&&... p_Args) {
		std::string f = std::format(p_Format, p_Args...);
		size_t fSize = f.length() + 1;
		char_t* ret = new char_t[fSize] {'!'};
		mem::Copy(f.data(), ret, fSize);
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
	CORE_API extern bool Compare(const char_t* p_String, const char_t* p_Check, const strlen_t p_EndIndex);
	CORE_API extern bool Compare(const char_t* p_String, const char_t* p_Check);
}


class CORE_API CString {
private:
	char_t* m_Data = nullptr;
	strlen_t m_Length = 0;
	//u8 m_RefCount = 0;

	/// <summary>
	/// Pushes new char data into the main string.
	/// </summary>
	/// <param name="p_Increase">The length of the strings array (or 1 if it is one element)</param>
	/// <param name="p_NewData">The array of new data (or a char pointer)</param>
	void _extend(s8 p_Increase, const char_t* p_NewData) {
		if (!m_Data) {
			if (p_Increase == 1) {
				m_Data = new char_t[2];
				m_Data[0] = *p_NewData;
				m_Data[1] = '\0';
				m_Length = 1;
				return;
			}
			m_Length = p_Increase;
			m_Data = new char_t[p_Increase + 1];
			mem::Copy(p_NewData, m_Data, p_Increase);
			m_Data[m_Length] = '\0';
			return;
		}
		char_t* output = new char_t[m_Length + p_Increase + 1];
		mem::Copy(m_Data, output, m_Length);
		if (p_Increase == 1) {
			output[m_Length] = *p_NewData;
		}
		else {
			//Copy the new data to the rest of the output
			mem::Copy(p_NewData, (output + m_Length), p_Increase);
		}
		output[m_Length + p_Increase] = '\0';
		delete[] m_Data;
		m_Data = output;
		m_Length += p_Increase;
	}

	bool xRange(strlen_t p_Value) const {
		return (p_Value > m_Length + 1);
	}
public:

	/// <summary>
	/// A getter function for m_Data
	/// </summary>
	/// <returns>the raw char array pointer to our string</returns>
	char_t* data() const {
		return m_Data;
	}
	/// <summary>
	/// A getter function for a copy of m_Data
	/// </summary>
	/// <returns>A copy of our string</returns>
	char_t* data_copy() const {
		char_t* ret = new char_t[m_Length + 1];
		mem::Copy(m_Data, ret, m_Length + 1);
		return ret;
	}
	/// <summary>
	/// Unsafe version that simply returns the character at an index.
	/// </summary>
	/// <param name="p_Index">The index of the string</param>
	/// <returns>The character at the specified index of the string</returns>
	char_t& at(strlen_t p_Index) const {
		return static_cast<char_t&>(m_Data[p_Index]);
	}
	/// <summary>
	/// Safe version of at() that checks if the data is valid and if the index is valid.
	/// </summary>
	/// <param name="p_Index">The index of the string</param>
	/// <returns>The character at the specified index of the string</returns>
	char_t& at_safe(strlen_t p_Index) const {
		if (!m_Data)
			throw Error::CRuntime("The string has no data.");
		if (xRange(p_Index))
			throw Error::CExOutOfBounds();
		return at(p_Index);
	}
	char_t& operator[](strlen_t p_Index) {
		return at(p_Index);
	}
	/// <summary>
	/// Returns the first character of the string
	/// </summary>
	/// <returns>The first character of the string</returns>
	char_t& first() const {
		return at(0);
	}
	/// <summary>
	/// Returns the last character of the string
	/// </summary>
	/// <returns>The last character of the string</returns>
	char_t& last() const {
		return at(m_Length - 1);
	}


	/// <summary>
	/// A getter for the strings size
	/// </summary>
	/// <returns>The strings size</returns>
	strlen_t size() const {
		return m_Length + 1;
	}
	/// <summary>
	/// A getter for the strings length
	/// </summary>
	/// <returns>The strings length</returns>
	strlen_t length() const {
		return m_Length;
	}

	char_t* substring_raw(strlen_t p_Beginning, strlen_t p_End, bool p_EndIsLength = false) const {
		//strlen_t realEnd = (p_EndIsLength ? (p_Beginning + p_End) : p_End - 1);
		strlen_t len = (p_End - p_Beginning);
		char* dat = new char_t[len + 1];
		mem::Copy(m_Data + p_Beginning, dat, len);
		dat[len] = '\0';
		return dat;
	}
	CString substring(strlen_t p_Beginning, strlen_t p_End, bool p_EndIsLength = false) const {
		return CString(substring_raw(p_Beginning, p_End, p_EndIsLength));
	}

	//RVALUES
	bool operator==(CString&& p_Other) {
		return String::Compare(m_Data, p_Other.m_Data, m_Length);
	}
	bool operator==(const char_t*&& p_Other) {
		return String::Compare(m_Data, p_Other, m_Length);
	}
	//REFS
	bool operator==(CString& p_Other) {
		return String::Compare(m_Data, p_Other.m_Data, m_Length);
	}
	bool operator==(const char_t*& p_Other) {
		return String::Compare(m_Data, p_Other, m_Length);
	}

	void operator+=(const char_t* p_Other) {
		_extend(String::FindLength(p_Other), p_Other);
	}
	void operator+=(char_t p_Other) {
		_extend(1, &p_Other);
	}

	void operator+=(CString&& p_Other) {
		_extend(p_Other.m_Length, p_Other.m_Data);
	}
	void operator+=(CString& p_Other) {
		_extend(p_Other.m_Length, p_Other.m_Data);
	}

	CString operator+(CString&& p_Other) {
		CString ret;
		ret += m_Data;
		ret += p_Other.data();
		return ret;
	}
	CString operator+(CString& p_Other) {
		CString ret;
		ret += m_Data;
		ret += p_Other.data();
		return ret;
	}
	CString operator+(const char_t* p_Other) {
		CString ret;
		ret += m_Data;
		ret += p_Other;
		return ret;
	}
	CString operator+(char_t p_Other) {
		CString ret;
		ret += m_Data;
		ret += p_Other;
		return ret;
	}
	CString operator+(char_t&& p_Other) {
		CString ret;
		ret += m_Data;
		ret += p_Other;
		return ret;
	}

	explicit operator const char* () {
		return m_Data;
	}
	explicit operator char* () {
		return m_Data;
	}

	void erase(strlen_t p_Index, strlen_t p_Amount = 1) {
		if (m_Length - p_Amount == 0) {
			delete[] m_Data;
			m_Length = 0;
			return;
		}
		m_Length -= p_Amount;
		char_t* output = new char_t[m_Length + 1];
		for (strlen_t i = 0;i < m_Length;i++) {
			if (i < p_Index) {
				output[i] = m_Data[i];
				continue;
			}
			output[i] = m_Data[i + p_Amount + p_Index];
		}
		delete[] m_Data;
		m_Data = output;
	}

	bool is_empty() {
		//return (m_Length == 0);
		return (!m_Data || m_Length == 0);
	}
	void destroy() {
		if (!m_Data)
			return;
		LogDebug("Bye bye!");
		delete[] m_Data;
		m_Length = 0;
		m_Data = nullptr;
	}
	//constant char array
	void operator=(const char_t* p_Data) {
		destroy();
		m_Length = String::FindLength(p_Data);
		m_Data = new char_t[m_Length + 1];
		mem::Copy(p_Data, m_Data, m_Length + 1);
	}
	//editable char array
	void operator=(char_t* p_Data) {
		destroy();
		m_Data = p_Data;
		m_Length = String::FindLength(p_Data);
	}
	//CString reference
	void operator=(const CString& p_Data) {
		destroy();
		m_Data = p_Data.m_Data;
		m_Length = p_Data.m_Length;
	}
	//Null pointer
	void operator=(std::nullptr_t p_Null) {
		m_Data = nullptr;
		m_Length = 0;
	}
	//char
	void operator=(const char_t&& p_Value) {
		m_Data = new char_t[2]{ p_Value, '\0' };
		m_Length = 1;
	}


	CString() {}
	CString(const char_t* p_Data) {
		operator=(p_Data);
	}
	CString(char_t* p_Data) {
		operator=(p_Data);
	}
	CString(std::nullptr_t p_Null) {
		operator=(nullptr);
	}
	template<typename ...Args>
	static CString create_format(std::format_string<Args...> p_Format, Args&&... p_Args) {
		CString d;
		d.m_Data = String::MakeFromFormat(p_Format, p_Args...);
		d.m_Length = String::FindLength(d.m_Data);
		return d;
	}
	CString(const CString& p_Value) {
		operator=(p_Value);
	}
	CString(const char_t&& p_Value) {
		m_Data = new char_t[2]{ p_Value, '\0' };
		m_Length = 1;
	}
	template<typename ...Args>
	CString(Args&&... p_Args) {
		([&, this] 
		{
			operator+=(p_Args);


		}(), ...);

	}

	~CString() {
#if !CO_D_NOOPTI
		destroy();
#endif
	}


	char_t* begin() {
		return (m_Data);
	}
	char_t* end() {
		return (m_Data + m_Length);
	}
};




CORE_END_NAMESPACE


template<>
struct std::formatter<cl::CString> : std::formatter<std::string_view> {
	auto format(const cl::CString& p_String, std::format_context& p_CTX) const {
		return std::formatter<std::string_view>::format(std::string_view(p_String.data()), p_CTX);
	}

};

#endif