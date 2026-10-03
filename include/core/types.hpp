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

static_assert(sizeof(u8) == 1, "A byte's size is not 1 byte!");
static_assert(sizeof(u16) == 2, "A short's size is not 2 bytes!");
static_assert(sizeof(u32) == 4, "An int's size is not 4 bytes!");
static_assert(sizeof(u64) == 8, "A int64's size is not 8 bytes!");

//GLuint = unsigned int, so unsigned int instead of u32!
using glID = unsigned int;
constexpr glID glInvalid = 0xDEADBEEF;

CORE_END_NAMESPACE

#ifndef NULL
#define NULL (void)0
#endif

#ifndef __cplusplus
#define bool u8
#define true 1
#define false 0
#endif

#endif