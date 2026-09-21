#ifndef __STRING_H__
#define __STRING_H__

#include <core/common.hpp>
#include <core/list.hpp>

CORE_DECLARE_NAMESPACE


using strsize_t = unsigned __int16;

class CString : public CList<char, strsize_t> {
private:
	void _setString() {

	}
public:

	void operator+=(const char* p_String) {
		_extend(GetLength(p_String) + 1, const_cast<char*>(p_String));
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

CORE_END_NAMESPACE


#endif