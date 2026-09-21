#ifndef __RECT_H__
#define __RECT_H__

#include <core/common.hpp>

CORE_DECLARE_NAMESPACE

template<typename Pos, typename Size>
struct CRectTemplate {
	Pos x = Pos();
	Pos y = Pos();
	Size width = Size();
	Size height = Size();
};
//Basic rect type, for 
using CRect = CRectTemplate<POS_TYPE, SIZE_TYPE>;

using CBigRect = CRectTemplate<__int64, SIZE_TYPE>;

using CCharRect = CRectTemplate<__int8, __int8>;
using CByteRect = CRectTemplate<unsigned __int8, unsigned __int8>;


CORE_END_NAMESPACE


#endif