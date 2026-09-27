#ifndef __LIBAPI_H__
#define __LIBAPI_H__

#define TESTING 1

#ifdef TESTING

#define CORE_API 

#else

#ifdef CoreLib_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif

#endif


#define CORE_DECLARE_NAMESPACE namespace cl {

#define CORE_END_NAMESPACE }

#endif