#pragma once

#include <core/options.hpp>

#if CO_G_SCRIPTING

#include <core/libapi.hpp>
#include <angelscript/include/angelscript.h>
#include <core/comp/base.hpp>
#include <core/types.hpp>
#include <core/string.hpp>
CORE_DECLARE_NAMESPACE


COMPONENT_DEFINE_START(Scripting, true)

using scriptid_t = u32;

class CScript {
public:
	CString m_Name;
	scriptid_t m_ID = 0;

	template<typename... Args>
	void Call(Args... p_Parameters) {

	}
	void Init() {}
	void Destroy(){}
};

class IScriptBase {
public:

};

class CAngelScript : public IScriptBase {

};

class CMaxPlusPLus : public IScriptBase {

};

COMPONENT_DECLARE_INIT();

COMPONENT_DECLARE_UPDATE();

COMPONENT_DECLARE_UNLOAD();


COMPONENT_DEFINE_END

CORE_END_NAMESPACE
#endif