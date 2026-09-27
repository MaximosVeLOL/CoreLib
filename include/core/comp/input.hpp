#ifndef __INPUT_H__
#define __INPUT_H__

#include <core/libapi.hpp>
#include <core/types.hpp>
#include <core/comp/base.hpp>
#include <GLFW/glfw3.h>

CORE_DECLARE_NAMESPACE

struct CState {
	bool current : 4 = false;
	bool previous: 4 = false;
};


struct CBinding {
	s16 m_keyIndex1 : 9 = GLFW_KEY_UNKNOWN;
	u8 m_keyIndex2 : 7 = 127;
};

struct CAxis {
	u8 binding1 = 0xFF;
	u8 binding2 = 0xFF;
};


CORE_API extern void AddAxis(CAxis p_Axis);

COMPONENT_DEFINE_START(Input, true)

COMPONENT_DECLARE_INIT();

COMPONENT_DECLARE_UNLOAD();

CORE_API extern void GLFW_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

COMPONENT_DEFINE_END

CORE_END_NAMESPACE

#endif