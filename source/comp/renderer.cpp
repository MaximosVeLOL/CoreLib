#include <core/comp/renderer.hpp>
#include <core/logging.hpp>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <core/string.hpp>
#include <core/filesystem.hpp>


CORE_DECLARE_NAMESPACE

bool g_HasFontEngine = false;

//Place private stuff here

TTF_Font* g_DefaultFont = nullptr;

SDL_FRect floatRectFromRenderRect(const Renderer::CRenderRect& p_Rect) {
	return SDL_FRect{ static_cast<float>(p_Rect.x), static_cast<float>(p_Rect.y), static_cast<float>(p_Rect.width), static_cast<float>(p_Rect.height) };
}


namespace Renderer {
	constexpr bool g_UseHA = true;

	SDL_Window* g_Window = nullptr;
	SDL_Renderer* g_Renderer = nullptr;
	
	CColor::operator SDL_Color() {
		return SDL_Color{ r, g, b, a };
	}

	SDL_Texture* CTexture::As_SDLTexture() {
		return static_cast<SDL_Texture*>(m_Data);
	}
	void CTexture::Destroy() {
		SDL_DestroyTexture(static_cast<SDL_Texture*>(m_Data));
	}


	COMPONENT_IMPLEMENT_INIT() {
		if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK)) {
			LogError("Failed to initialize SDL3!");
			return false;
		}
		g_Window = SDL_CreateWindow(APP_NAME, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
		if (!g_Window) {
			LogError("Failed to create window!");
			return false;
		}
		g_Renderer = SDL_CreateRenderer(g_Window, NULL);
		if (!g_Renderer) {
			LogError("Failed to create renderer!");
			return false;
		}
		g_HasFontEngine = TTF_Init(); 
		FileSystem::CFile fontFile("fonts/default.ttf");
		if (fontFile.IsOpen()) {
			g_DefaultFont = TTF_OpenFontIO(fontFile.GetStream(), false, 12.0f);
			if (!g_DefaultFont) {
				LogWarning("(Renderer::Init) Failed to load default font! No text will be displayed on-screen!");
			}
		}
		else LogWarning("(Renderer::Init) Failed to load file (fonts/default.ttf)! No text will be displayed on-screen!");
		
		//fontFile.Close();
		return true;
	}
	/*
	COMPONENT_IMPLEMENT_UPDATE() {

	}
	*/
	COMPONENT_IMPLEMENT_UNLOAD() {

	}
}



namespace Render {
	void Clear() {
		SDL_RenderClear(Renderer::g_Renderer);
	}
	void SetColor(Renderer::CColor p_Color) {
		if (Renderer::g_UseHA) {
			SDL_SetRenderDrawColor(Renderer::g_Renderer, p_Color.r, p_Color.g, p_Color.b, p_Color.a);
			return;
		}
		
	}
	void Rect(Renderer::CRenderRect p_Rect) {
		SDL_FRect r = {static_cast<float>(p_Rect.x), static_cast<float>(p_Rect.y), static_cast<float>(p_Rect.width), static_cast<float>(p_Rect.height) };
		SDL_RenderRect(Renderer::g_Renderer, &r);
	}
	void Rect(Renderer::CRenderRect p_Rect, Renderer::CColor p_Color) {
		SetColor(p_Color);
		Rect(p_Rect);
	}
	void FillRect(Renderer::CRenderRect p_Rect) {
		SDL_FRect r = {static_cast<float>(p_Rect.x), static_cast<float>(p_Rect.y), static_cast<float>(p_Rect.width), static_cast<float>(p_Rect.height) };
		SDL_RenderFillRect(Renderer::g_Renderer, &r);
	}
	void FillRect(Renderer::CRenderRect p_Rect, Renderer::CColor p_Color) {
		SetColor(p_Color);
		FillRect(p_Rect);
	}
	void Present() {
		SDL_RenderPresent(Renderer::g_Renderer);
	}
	void Texture(Renderer::CTexture& p_Texture, Renderer::CRenderRect p_Rect) {
		SDL_Texture* t = p_Texture.As_SDLTexture();
		SDL_FRect r = floatRectFromRenderRect(p_Rect);

		SDL_RenderTexture(Renderer::g_Renderer, t, NULL, &r);
	}
	void Texture(Renderer::CTexture& p_Texture, Renderer::CRenderRect p_Rect, Renderer::CRenderRect p_Part) {
		SDL_Texture* t = p_Texture.As_SDLTexture();
		SDL_FRect renderPos = floatRectFromRenderRect(p_Rect);
		SDL_FRect part = floatRectFromRenderRect(p_Part);
		SDL_RenderTexture(Renderer::g_Renderer, t, &part, &renderPos);
	}
	Renderer::CTexture CreateTextureFromText(const char* p_Text, unsigned __int16 p_Length, Renderer::CColor p_Foreground, Renderer::CColor p_Background) {
		if (!g_HasFontEngine) {
			return Renderer::CTexture{ nullptr };
		}
		SDL_Surface* s = TTF_RenderText_Blended_Wrapped(g_DefaultFont, p_Text, p_Length, p_Foreground, 0);
		if (!s) {
			return Renderer::CTexture{ nullptr };
		}
		SDL_Texture* t = SDL_CreateTextureFromSurface(Renderer::g_Renderer, s);
		SDL_DestroySurface(s);
		return Renderer::CTexture{ t };
	}
	void Text(Renderer::CRenderVector p_Position, const char* p_Text, unsigned __int16 p_Length, Renderer::CColor p_Foreground, Renderer::CColor p_Background) {
		Renderer::CTexture t = CreateTextureFromText(p_Text, p_Length, p_Foreground, p_Background);
		Render::Texture(t, Renderer::CRenderRect{ p_Position.x, p_Position.y, t.GetWidth<s16>(), t.GetHeight<s16>() });
		t.Destroy();
	}
	void Text(Renderer::CRenderVector p_Position, CString p_String, Renderer::CColor p_Foreground, Renderer::CColor p_Background) {
		Text(p_Position, p_String.data_safe(), p_String.size(), p_Foreground, p_Background);
	}
}

CORE_END_NAMESPACE