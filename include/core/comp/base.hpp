#ifndef __BASE_H__
#define __BASE_H__

#include <core/libapi.hpp>
#include <core/types.hpp>

/* Component System
* Use abunch of defines, as using classes would mean we are able to have
* Multiple components, which is not true, and shouldn't happen!
* So here we are, having abunch of defines.
* 
* Usage:
* Interfaces
* Use the define start to declare your component, and if it is used
* Use the declares to have it exist in a header file
* Use the define end to end the definition, use this or else you will get compiler errors
* 
* 
* Implementation:
* Use the implement defines in order to define it
* 
* Types:
* 
* Init - Can return a boolean (true, false), or any other value.
* Just because it returns a boolean, doesn't mean that it will return a boolean
* 
* Update & Unload - Simple functions that should never fail

*/

#define COMPONENT_DEFINE_START(name, isUsed) constexpr bool COMPONENT_##name##_USED = isUsed; namespace name {

#define COMPONENT_DECLARE_INIT CORE_API extern cl::u8 Init
#define COMPONENT_IMPLEMENT_INIT cl::u8 Init

#define COMPONENT_DECLARE_UPDATE CORE_API extern void Update
#define COMPONENT_IMPLEMENT_UPDATE void Update

#define COMPONENT_DECLARE_UNLOAD CORE_API extern void Unload
#define COMPONENT_IMPLEMENT_UNLOAD void Unload

#define COMPONENT_DEFINE_END }


#define COMPONENT_CALL_INIT(name) cl::name::Init()
#define COMPONENT_CALL_INIT(name, onFail) if(!cl::name::Init()) onFail
#define COMPONENT_CALL_UPDATE(name) cl::name::Update()
#define COMPONENT_CALL_UNLOAD(name) cl::name::Update()

#endif