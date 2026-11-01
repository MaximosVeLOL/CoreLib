#ifndef __INPUT_H__
#define __INPUT_H__

#include <core/libapi.hpp>
#include <core/types.hpp>
#include <core/comp/base.hpp>
#include <core/list.hpp>
#include <functional>


#if CORE_PLATFORM == PLATFORM_SDLSUPPORTED
using SDL_Keycode = cl::u32;
#endif

CORE_DECLARE_NAMESPACE

struct CState {
	bool current : 4 = false;
	bool previous: 4 = false;
};

//Do we use hashing in CName instead of comparing strings?
#define INPUT_USE_HASH 1


struct CName {
	//We expect it to be unsigned
	char name[12] = "yo";
#if INPUT_USE_HASH
	u16 hash = 0; //The strings all added up together
	void GenerateHash() {
		u8 index = 0;
		while (name[index]) {
			hash += name[index];
			index++;
		}
	}
	static u16 AsHashed(const char* p_String) {
		u8 index = 0;
		u16 ret = 0;
		while (p_String[index]) {
			ret += p_String[index];
			index++;
		}
		return ret;
	}
#endif
};

struct CBinding : CName {
	s16 m_keyIndex : 9 = 0;
	u8 m_padIndex : 7 = 127;
};

struct CAxis : CName {
	u8 binding1 = 0xFF;
	u8 binding2 = 0xFF;
};
enum EMouseState : u8 {
	MBUTTON_NONE = 0,
	MBUTTON_LEFT = 1,
	MBUTTON_MIDDLE = 2,
	MBUTTON_RIGHT = 3,
	MBUTTON_SIDEUP = 4,
	MBUTTON_SIDEDOWN = 5,
};
struct CMouse {
	s16 x = -1;
	s16 y = -1;
	EMouseState stateCurrent = MBUTTON_NONE;
	EMouseState statePrevious = MBUTTON_NONE;
	bool GetDown(EMouseState p_KeyIndex) {
		return (stateCurrent == p_KeyIndex);
	}
	bool GetWentDown(EMouseState p_KeyIndex) {
		return (stateCurrent == p_KeyIndex && statePrevious != p_KeyIndex);
	}
};




//CORE_API extern void AddAxis(CAxis p_Axis);

COMPONENT_DEFINE_START(Input, true)

using CKeyEvent = std::function<bool(const bool*)>;

#if INPUT_USE_HASH
using binding_t = u16;
#else
using binding_t = char[12];
#endif

CORE_API extern CMouse g_Mouse;
//Only exposed due to Magea needing to connect this to CoreLib. SMH
CORE_API extern std::function<void(const char*)> g_OnTextInputEvent;

//The functions return true whether it is going away or not.
CORE_API extern CList<CKeyEvent> g_OnKeyDownEvents;

CORE_API extern u8 GetBindingIndexFromBindingT(binding_t p_Binding);
CORE_API extern u8 GetAxisIndexFromBindingT(binding_t p_Binding);

CORE_API extern bool GetBindingUp(u8 p_BindingIndex);
CORE_API extern bool GetBindingWentDown(u8 p_BindingIndex);
CORE_API extern bool GetBindingDown(u8 p_BindingIndex);
CORE_API extern bool GetBindingWentUp(u8 p_BindingIndex);

CORE_API extern bool GetBindingUp(binding_t p_Binding);
CORE_API extern bool GetBindingWentDown(binding_t p_Binding);
CORE_API extern bool GetBindingDown(binding_t p_Binding);
CORE_API extern bool GetBindingWentUp(binding_t p_Binding);

//Can return 0 for no input, -1 for the down axis, and 1 for the positive axis
CORE_API extern s8 GetAxisState(binding_t p_Axis);
CORE_API extern s8 GetAxisState(u8 p_AxisIndex);

CORE_API extern void ResetState(u8 p_BindingIndex);
CORE_API extern void ResetState(binding_t p_Binding);
CORE_API extern void ResetAllStates();

CORE_API extern void SetTextInputState(bool p_State, std::function<void(const char*)> p_OnInputEvent = nullptr);

COMPONENT_DECLARE_INIT();

COMPONENT_DECLARE_UPDATE();

COMPONENT_DECLARE_UNLOAD();

COMPONENT_DEFINE_END

CORE_END_NAMESPACE

#endif