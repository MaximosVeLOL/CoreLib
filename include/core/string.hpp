#ifndef __STRING_H__
#define __STRING_H__

#include <core/libapi.hpp>
#include <core/list.hpp>
#include <format>


CORE_DECLARE_NAMESPACE


using strsize_t = unsigned __int16;

constexpr strsize_t STRING_INVALID = 65535;

class CORE_API CString : public CList<char, strsize_t> {
private:
	void _setString() {

	}
public:

	void operator+=(const char* p_String) {
		_extend(GetLength(p_String) + 1, const_cast<char*>(p_String));
	}

	void operator+=(char p_Char) {
	}

	static strsize_t GetLength(const char* p_String) {
		strsize_t ret = 0;
		while (p_String[ret++]);
		return ret;
	}
	static char* MakeCharArrayFromConstChar(const char* p_String, strsize_t* p_OutCount = nullptr) {
		strsize_t len = GetLength(p_String);
		char* ret = new char[len + 1] {'!'};
		for (strsize_t i = 0; i <= len;i++) {
			ret[i] = p_String[i];
		}
		if (p_OutCount)
			*p_OutCount = len;
		return ret;
	}

	template<typename... Args>
	static const char* MakeCharArrayFromFormat(std::format_string<Args...> p_Format, Args... p_Args) {
		std::string format = std::format(p_Format, p_Args...);
		const char* str = format.c_str();
		return str;
	}

	void operator=(const char* p_String) {
		delete[] m_Data;
		m_Data = MakeCharArrayFromConstChar(p_String, &m_Count);
	}

	CString() {}

	CString(const char* p_String) {
		operator=(p_String);
	}

	operator char* () {
		return m_Data;
	}
};
struct CCharArrayWrapper {
	const char* m_Data = nullptr;
	strsize_t m_Length = 0;
	bool m_IsConst = true;

	CCharArrayWrapper(const char* p_Data) {
		m_Data = p_Data;
		m_Length = CString::GetLength(p_Data);
		m_IsConst = true;
	}
	CCharArrayWrapper(char* p_Data) {
		m_Data = p_Data;
		m_Length = CString::GetLength(p_Data);
		m_IsConst = false;
	}
};

CORE_END_NAMESPACE


#endif