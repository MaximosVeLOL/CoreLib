#ifndef __RECT_H__
#define __RECT_H__

#include <core/libapi.hpp>
#include <core/common.hpp>
#include <core/types.hpp>

CORE_DECLARE_NAMESPACE

template<typename Pos, typename Size>
struct CRectTemplate {
	Pos x = Pos();
	Pos y = Pos();
	Size width = Size();
	Size height = Size();
	template<typename NewPos, typename NewSize>
	CRectTemplate<NewPos, NewSize> ConvertTo() {
		return { static_cast<NewPos>(x), static_cast<NewPos>(y), static_cast<NewSize>(width), static_cast<NewSize>(height) };
	}


	template<typename NewPos, typename NewSize>
	operator CRectTemplate<NewPos, NewSize>() {
		return ConvertTo<NewPos, NewSize>();
	}

	CRectTemplate() = default;
};
//Basic rect type, for 
using CRect = CRectTemplate<POS_TYPE, SIZE_TYPE>;

using CBigRect = CRectTemplate<s64, SIZE_TYPE>;

using CCharRect = CRectTemplate<s8, s8>;
using CByteRect = CRectTemplate<u8, u8>;


CORE_END_NAMESPACE


#endif