#ifndef __LIBAPI_H__
#define __LIBAPI_H__

#ifdef CoreLib_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif


#define CORE_DECLARE_NAMESPACE namespace cl {

#define CORE_END_NAMESPACE }

#endif