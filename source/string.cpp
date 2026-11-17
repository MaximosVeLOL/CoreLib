#include <core/string.hpp>

CORE_DECLARE_NAMESPACE

namespace String {
	strlen_t FindLength(const char_t* p_String) {
		if (!p_String)
			return 0; //String is nullptr, so return 0, as there is no data
		strlen_t ret = 0;
		while (p_String[ret] != '\0') { ret++; }
		return ret;
	}
	bool Compare(const char_t* p_String, const char_t* p_Check, const strlen_t p_EndIndex) {
		for (strlen_t i = 0; i < p_EndIndex;i++) {
			if (p_String[i] != p_Check[i])
				return false;
		}
		return true;
	}
	bool Compare(const char_t* p_String, const char_t* p_Check) {
		strlen_t s1 = FindLength(p_String);
		if (s1 != FindLength(p_Check)) {
			return false;
		}
		return Compare(p_String, p_Check, s1);
	}
}


CORE_END_NAMESPACE