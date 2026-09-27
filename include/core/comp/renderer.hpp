#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <core/types.hpp>
#include <core/libapi.hpp>
#include <core/comp/base.hpp>
#include <core/game/rect.hpp>

/* Renderer System
* Use the Renderer namespace for the system stuff
* Use the Render namespace to actually render things to the screen

*/

struct GLFWwindow;

CORE_DECLARE_NAMESPACE

COMPONENT_DEFINE_START(Renderer, true)

using scrsize_t = u16;

constexpr scrsize_t SCREEN_WIDTH = 960;
constexpr scrsize_t SCREEN_HEIGHT = 540;



CORE_API extern GLFWwindow* gWindow;

COMPONENT_DECLARE_INIT();

COMPONENT_DECLARE_UPDATE();

COMPONENT_DECLARE_UNLOAD();

struct CFloatColor {
	float r = 0;
	float g = 0;
	float b = 0;
	float a = 0;
	CFloatColor(){}
	CFloatColor(u8 p_R, u8 p_G, u8 p_B, u8 p_A) {
		r = static_cast<float>(p_R / 255.0f);
		g = static_cast<float>(p_G / 255.0f);
		b = static_cast<float>(p_B / 255.0f);
		a = static_cast<float>(p_A / 255.0f);

	}
};
/*
struct CColor {
	u8 r = 0;
	u8 g = 0;
	u8 b = 0;
	u8 a = 0;
	//operator CFloatColor() {
	//	return CFloatColor{ static_cast<float>(r / 255), static_cast<float>(g / 255), static_cast<float>(b / 255), static_cast<float>(a / 255) };
	//}
};
*/
struct CObject {
	
	float* vertices = nullptr;
	float* indices = nullptr;
};

using CRenderRect = CRectTemplate<signed __int16, signed __int16>;

COMPONENT_DEFINE_END

namespace Render {
	CORE_API extern void SetColor(Renderer::CFloatColor p_Color);
	CORE_API extern void Rect(Renderer::CRenderRect p_Rect);
	CORE_API extern void Rect(Renderer::CRenderRect p_Rect, Renderer::CFloatColor p_Color);
}


CORE_END_NAMESPACE

#endif