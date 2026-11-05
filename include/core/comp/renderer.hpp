#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <core/types.hpp>
#include <core/libapi.hpp>
#include <core/comp/base.hpp>
#include <core/game/rect.hpp>
#include <core/game/vector.hpp>
#include <core/platform.hpp>
#include <core/memory.hpp>
//#if CORE_PLATFORM == PLATFORM_SDLSUPPORTED
//#include <SDL3/SDL.h> //For SDL_Texture data stuff
//#endif
/* Renderer System
* Use the Renderer namespace for the system stuff
* Use the Render namespace to actually render things to the screen

*/
#if 1
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Color;
struct SDL_Texture;
#endif

CORE_DECLARE_NAMESPACE

COMPONENT_DEFINE_START(Renderer, true)

using scrsize_t = u16;

constexpr scrsize_t SCREEN_WIDTH = 960;
constexpr scrsize_t SCREEN_HEIGHT = 540;
inline const char* APP_NAME = "Application";



CORE_API extern SDL_Window* g_Window;
CORE_API extern SDL_Renderer* g_Renderer;

COMPONENT_DECLARE_INIT();

//COMPONENT_DECLARE_UPDATE();

COMPONENT_DECLARE_UNLOAD();
/*
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
*/

struct CColor {
	u8 r = 0;
	u8 g = 0;
	u8 b = 0;
	u8 a = 0;
#if CORE_PLATFORM == PLATFORM_SDLSUPPORTED
	operator SDL_Color();
	//operator CFloatColor() {
	//	return CFloatColor{ static_cast<float>(r / 255), static_cast<float>(g / 255), static_cast<float>(b / 255), static_cast<float>(a / 255) };
	//}
#endif
};

struct CORE_API CTexture {
	//Can either be one of the following:
	// A SDL_Texture or SDL_Surface (modern platforms)
	//
	void* m_Data = nullptr;
#if CORE_PLATFORM == PLATFORM_SDLSUPPORTED
	template<typename T = u32>
	T GetWidth() {
		return static_cast<T>(As_SDLTexture()->w);
	}
	template<typename T = u32>
	T GetHeight() {
		return static_cast<T>(As_SDLTexture()->h);
	}
	
	SDL_Texture* As_SDLTexture();
#else
	template<typename T = u32>
	T GetWidth() {
#error Implement this!
	}
#endif

	bool IsValid() {
		return static_cast<bool>(m_Data);
	}

	void Destroy();

	CTexture(){}
	CTexture(void* p_Data) : m_Data(p_Data) {}
};
/*
struct CObject {
	
	float* vertices = nullptr;
	float* indices = nullptr;
};
*/
struct CRenderRect : CRectTemplate<s16, s16> {
	CRenderRect() {}
	CRenderRect(const CRect& p_Other) {
		x = static_cast<s16>(p_Other.x);
		y = static_cast<s16>(p_Other.y);
		width = static_cast<s16>(p_Other.width);
		height = static_cast<s16>(p_Other.height);
	}
	CRenderRect(s16 p_X, s16 p_Y, s16 p_W, s16 p_H) {
		x = p_X;
		y = p_Y;
		width = p_W;
		height = p_H;
	}
	CRenderRect(const CRectTemplate<s16, s16> p_Derivative) {
		x = p_Derivative.x;
		height = p_Derivative.y;
		width = p_Derivative.width;
		height = p_Derivative.height;
	}
};
using CRenderVector = CVectorTemplate<s16>;

COMPONENT_DEFINE_END

class CString;

namespace Render {
	CORE_API extern void Clear();
	CORE_API extern void SetColor(Renderer::CColor p_Color);
	CORE_API extern void Rect(Renderer::CRenderRect p_Rect);
	CORE_API extern void Rect(Renderer::CRenderRect p_Rect, Renderer::CColor p_Color);
	CORE_API extern void FillRect(Renderer::CRenderRect p_Rect, Renderer::CColor p_Color);
	CORE_API extern void FillRect(Renderer::CRenderRect p_Rect, Renderer::CColor p_Color);
	CORE_API extern void Present();
	//static_assert(sizeof(strsize_t) == sizeof(unsigned __int16), "Please update strsize_t in Render::Text()!");
	CORE_API extern Renderer::CTexture CreateTextureFromText(const char* p_Text, unsigned __int16 p_Length, Renderer::CColor p_Foreground = { 0xFF, 0xFF, 0xFF, 0xFF }, Renderer::CColor p_Background = {0, 0, 0, 0xFF});
	CORE_API extern void Text(Renderer::CRenderVector p_Position, const char* p_Text, unsigned __int16 p_Length, Renderer::CColor p_Foreground = { 0xFF, 0xFF, 0xFF, 0xFF }, Renderer::CColor p_Background = { 0, 0, 0, 0xFF });
	CORE_API extern void Text(Renderer::CRenderVector p_Position, CString p_String, Renderer::CColor p_Foreground = { 0xFF, 0xFF, 0xFF, 0xFF }, Renderer::CColor p_Background = { 0, 0, 0, 0 });
	CORE_API extern void Texture(Renderer::CTexture& p_Texture, Renderer::CRenderRect p_Rect);
	CORE_API extern void Texture(Renderer::CTexture& p_Texture, Renderer::CRenderRect p_Rect, Renderer::CRenderRect p_Part);
}


CORE_END_NAMESPACE

#endif