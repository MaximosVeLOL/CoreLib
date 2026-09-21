#include <core/comp/input.hpp>

CORE_DECLARE_NAMESPACE

namespace Input {

	CBinding* gBindings = nullptr;
	u8 gBindingCount = 0;

	CAxis* gAxis = nullptr;
	u8 gAxisCount = 0;


	COMPONENT_IMPLEMENT_INIT() {
		return true;
	}

	COMPONENT_IMPLEMENT_UNLOAD() {

	}

	CState* getStateFromScancode(int p_Scancode) {
		return nullptr;
	}

	void GLFW_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	}
}

CORE_END_NAMESPACE