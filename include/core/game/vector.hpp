#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

#include <core/libapi.hpp>
#include <core/common.hpp>
#include <core/types.hpp>

CORE_DECLARE_NAMESPACE

template<typename T>
struct CVectorTemplate {
	T x = T();
	T y = T();
};
using CVector = CVectorTemplate<POS_TYPE>;

using CBigVector = CVectorTemplate<__int64>;

using CFloatVector = CVectorTemplate<float>;

using CCharVector = CVectorTemplate<u8>;
using CByteVector = CVectorTemplate<u8>;

CORE_END_NAMESPACE


#endif