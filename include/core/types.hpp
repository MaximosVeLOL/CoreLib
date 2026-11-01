#ifndef __TYPES_H__
#define __TYPES_H__

#include <core/libapi.hpp>
#include <stdfloat>
#include <cstdint>


#ifndef _Float16
#define _Float16 __int16;
#endif

#ifndef _Float32
#define _Float32 float;
#endif


CORE_DECLARE_NAMESPACE


using u8 = std::uint8_t;
using s8 = std::int8_t;

using u16 = std::uint16_t;
using s16 = std::int16_t;

using u32 = uint32_t;
using s32 = int32_t;

using u64 = uint64_t;
using s64 = int64_t;

using f16 = _Float16;
using f32 = _Float32;
using f64 = double;
using f128 = long double;

static_assert(sizeof(u8) == 1, "A byte's size is not 1 byte!");
static_assert(sizeof(u16) == 2, "A short's size is not 2 bytes!");
static_assert(sizeof(u32) == 4, "An int's size is not 4 bytes!");
static_assert(sizeof(u64) == 8, "A int64's size is not 8 bytes!");

//GLuint = int, so int instead of u32!
using glID = int;
constexpr glID glInvalid = 0xDEADBEEF;

CORE_END_NAMESPACE

#ifndef NULL
#define NULL 0
#endif

#ifndef __cplusplus
#define bool u8
#define true 1
#define false 0
#endif

#endif