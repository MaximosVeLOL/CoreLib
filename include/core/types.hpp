#ifndef __TYPES_H__
#define __TYPES_H__

#include <core/libapi.hpp>

CORE_DECLARE_NAMESPACE


using u8 = unsigned __int8;
using s8 = signed __int8;

using u16 = unsigned __int16;
using s16 = signed __int16;

using u32 = unsigned __int32;
using s32 = signed __int32;

using u64 = unsigned __int64;
using s64 = signed __int64;

//GLuint = unsigned int, so unsigned int instead of u32!
using glID = unsigned int;
constexpr glID glInvalid = 0xDEADBEEF;

CORE_END_NAMESPACE

#endif