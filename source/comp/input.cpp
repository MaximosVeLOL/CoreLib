#include <core/comp/input.hpp>
#include <core/filesystem.hpp>
#include <core/logging.hpp>
#include <core/memory.hpp>
#include <SDL3/SDL.h>
//For Renderer::g_Window
#include <core/comp/renderer.hpp>

CORE_LOGGER_CREATE(Input)

CORE_DECLARE_NAMESPACE

namespace Input {

	CMouse g_Mouse = CMouse();

	CBinding* g_Bindings = nullptr;
	u8 g_BindingCount = 0;

	CAxis* g_Axises = nullptr;
	u8 g_AxisesCount = 0;

	CState* g_States = nullptr;

	std::function<void(const char*)> g_OnTextInputEvent = nullptr;

	CList<CKeyEvent> g_OnKeyDownEvents;

	COMPONENT_IMPLEMENT_INIT() {
		FileSystem::CFile file("sys/INPUTS");
		if (!file.IsOpen()) {
			LogWarning("(Input Component) Failed to open sys/INPUTS! Issues may occur now!");
			return false;
		}

		LogInput("Going to parse INPUTS");
		char read[32];
		u8 readIndex = 0;
		enum EMode : u8 {
			MODE_GETREGION = 0,
			MODE_FINDNAME = 1,
			MODE_FINDVALUE = 2,

		};
		enum ERegion : u8 {
			REGION_NONE = 0,
			REGION_BINDINGS = 1,
			REGION_AXISES = 2
		};
		EMode curMode = MODE_GETREGION;
		ERegion curRegion = REGION_NONE;

		CList<CBinding, u8> outBindings;
		CList<CAxis, u8> outAxises;

		u8 MODE_FINDVALUE_multipleIndex = 0;

		auto close = [&file]() -> void {
			file.Close();
		};
		bool doSkip = false;

		for (u32 i = 0; i < file.GetSize();i++) {
			char cur = file.Read<char>();
			if (cur == 0x20 || cur == 0x09 || cur == 0x0A || cur == 0x0D) 
				continue;
			switch (curMode) {
			case MODE_GETREGION:
				if (cur != '{')
					break;
				read[readIndex] = '\0';
				curMode = MODE_FINDNAME;
				doSkip = true;
				if (String::Compare(read, "bindings")) {
					curRegion = REGION_BINDINGS;
				}
				else if (String::Compare(read, "axises")) {
					curRegion = REGION_AXISES;
				}
				else {
					LogError("(Input Component) Detected an unknown region! ({}), returning false...", read);
					close();
					return false;
				}
			break;

			case MODE_FINDNAME:
				if (cur == '}') {
					curMode = MODE_GETREGION;
					doSkip = true;
					break;
				}
				if (cur != '(')
					break;
				read[readIndex] = '\0';
				if (curRegion == REGION_BINDINGS) {
					CBinding toAdd = CBinding();
					for (u8 i = 0; i < 12;i++) {
						toAdd.name[i] = read[i];
						if (read[i] == '\0')
							break;
					
					}
					toAdd.GenerateHash();
					outBindings.push(toAdd);
				}
				else {
					CAxis toAdd = CAxis();
					for (u8 i = 0; i < 12;i++) {
						toAdd.name[i] = read[i];
						if (read[i] == '\0')
							break;
					}
					toAdd.GenerateHash();
					outAxises.push(toAdd);
				}
				curMode = MODE_FINDVALUE;
				doSkip = true;
				break;
			case MODE_FINDVALUE: {
				if (cur == ',') {
					read[readIndex++] = '\0';
					MODE_FINDVALUE_multipleIndex = readIndex; //Skip the null terminator
					goto _continue;
					//Does this work here?
					break;
				}
				else if (cur != ')') {
					break;
				}
				read[readIndex] = '\0';
				const char* temp = (read + MODE_FINDVALUE_multipleIndex);

				if (curRegion == REGION_BINDINGS) {
					CBinding& b = outBindings.last();
					b.m_keyIndex = SDL_GetScancodeFromName(read);
					if (b.m_keyIndex == SDL_SCANCODE_UNKNOWN) {
						LogWarning("(Input::Input) Failed to get input key name ({})! Defaulting to unknown...", read);
					}
					b.m_padIndex = SDL_GetGamepadButtonFromString(temp);
					if (b.m_padIndex == SDL_GAMEPAD_BUTTON_INVALID && MODE_FINDVALUE_multipleIndex != 0) {
						LogWarning("(Input::Input) Failed to get input pad name ({})! Defaulting to invalid...", temp);
					}
				}
				else {
					CAxis& a = outAxises.last();
					u8 i = 0;
					for (; i < outBindings.size();i++) {
						CBinding& c = outBindings[i];
						if (String::Compare(c.name, read)) {
							a.binding1 = i;
							break;
						}
					}
					const char* nextBindingName = read + MODE_FINDVALUE_multipleIndex;
					for (i = 0; i < outBindings.size();i++) {
						CBinding& c = outBindings.at(i);
						if (String::Compare(c.name, nextBindingName)) {
							a.binding2 = i;
							break;
						}
					}
				}

				MODE_FINDVALUE_multipleIndex = 0;
				doSkip = true;

				curMode = MODE_FINDNAME;

				} break;
			}
			if (doSkip) {

				readIndex = 0;
				doSkip = false;
				_continue:
				continue;
			}
			
			read[readIndex++] = cur;

			
		}
		g_BindingCount = outBindings.size();
		g_Bindings = outBindings.data();
		g_AxisesCount = outAxises.size();
		g_Axises = outAxises.data();
		g_States = new CState[g_BindingCount]{ CState() };

		LogInput("Parse Inputs: Success");
		return true;
	}

	COMPONENT_IMPLEMENT_UPDATE() {
		int outCount = 0;
		const bool* state = SDL_GetKeyboardState(&outCount);
		for (u8 i = 0; i < g_BindingCount;i++) {
			CBinding& b = g_Bindings[i];
			g_States[i].previous = g_States[i].current;
			g_States[i].current = state[b.m_keyIndex];
		}
		for (const CKeyEvent& e : g_OnKeyDownEvents) {
			if (e(state)) {
				//TODO - Remove the element!
			}
		}
		//Update mouse previous
		Input::g_Mouse.statePrevious = Input::g_Mouse.stateCurrent;

		SDL_MouseButtonFlags f = SDL_GetMouseState(NULL, NULL);
		switch (f) {
		case SDL_BUTTON_LEFT:
			g_Mouse.stateCurrent = MBUTTON_LEFT;
			break;
		case SDL_BUTTON_MIDDLE:
			g_Mouse.stateCurrent = MBUTTON_MIDDLE;
			break;
		case SDL_BUTTON_RIGHT:
			g_Mouse.stateCurrent = MBUTTON_RIGHT;
			break;
		case SDL_BUTTON_X1:
			g_Mouse.stateCurrent = MBUTTON_SIDEUP;
			break;
		case SDL_BUTTON_X2:
			g_Mouse.stateCurrent = MBUTTON_SIDEDOWN;
			break;
		}

		Input::g_Mouse.stateCurrent = static_cast<cl::EMouseState>(f);
	}

	COMPONENT_IMPLEMENT_UNLOAD() {

	}


	bool GetBindingUp(u8 p_BindingIndex) {
		return (!g_States[p_BindingIndex].current);
	}
	bool GetBindingWentDown(u8 p_BindingIndex) {
		return (!g_States[p_BindingIndex].previous && g_States[p_BindingIndex].current);
	}
	bool GetBindingDown(u8 p_BindingIndex) {
		return (g_States[p_BindingIndex].current);
	}
	bool GetBindingWentUp(u8 p_BindingIndex) {
		return (g_States[p_BindingIndex].previous && !g_States[p_BindingIndex].current);
	}



	u8 GetBindingIndexFromBindingT(binding_t p_Binding) {
		for (u8 i = 0; i < g_BindingCount;i++) {
	#if INPUT_USE_HASH
			if (g_Bindings[i].hash == p_Binding) {
				return i;
			}
	#else
			if (String::Compare(g_Bindings[i].name, p_Binding)) {
				return i;
			}
	#endif
		}
		LogError("(Input::GetBindingUp) Failed to find binding with hash/name of ({})! Returning 0...", p_Binding); \
		return 0;
	}
	u8 GetAxisIndexFromBindingT(binding_t p_Binding) {
		for (u8 i = 0; i < g_AxisesCount;i++) {
#if INPUT_USE_HASH
			if (g_Axises[i].hash == p_Binding)
				return i;
#else
			if (String::Compare(g_Axises[i].name, p_Binding))
				return i;
#endif
		}
		LogError("(Input::GetAxisIndexFromBindingT) Failed to find axis with hash/name of ({})! Returning 0...", p_Binding); \
		return 0;
	}

	void ResetState(u8 p_BindingIndex) {
		g_States[p_BindingIndex] = CState{ false, false };
	}

	void ResetState(binding_t p_Binding) {
		ResetState(GetBindingIndexFromBindingT(p_Binding));
	}

	void ResetAllStates() {
		for (u8 i = 0; i < g_BindingCount;i++) {
			ResetState(i);
		}
	}

	s8 GetAxisState(binding_t p_Axis) {
		u8 index = GetAxisIndexFromBindingT(p_Axis);
		CAxis& a = g_Axises[index];
		return (GetBindingDown(a.binding1) - GetBindingDown(a.binding2));
	}
	s8 GetAxisState(u8 p_AxisIndex) {
		CAxis& a = g_Axises[p_AxisIndex];
		return (GetBindingDown(a.binding1) - GetBindingDown(a.binding2));
	}


	void SetTextInputState(bool p_State, std::function<void(const char*)> p_OnInputEvent) {
		if (p_State) {
			SDL_StartTextInput(Renderer::g_Window);
		}
		else SDL_StopTextInput(Renderer::g_Window);
		g_OnTextInputEvent = p_OnInputEvent;
	}


#define CREATE_BASIC_BINDING_T_FUNCTION(stateName) \
bool GetBinding##stateName##(binding_t p_Binding) { \
	return GetBinding##stateName##(GetBindingIndexFromBindingT(p_Binding)); \
}

	CREATE_BASIC_BINDING_T_FUNCTION(Up)
	CREATE_BASIC_BINDING_T_FUNCTION(WentDown)
	CREATE_BASIC_BINDING_T_FUNCTION(Down)
	CREATE_BASIC_BINDING_T_FUNCTION(WentUp)






}

CORE_END_NAMESPACE