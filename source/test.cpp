#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL_main.h>

#include <core/string.hpp>

#include <print>

#include <core/logging.hpp>

#include <core/comp/renderer.hpp>
#include <core/comp/input.hpp>
using namespace cl;



//#include <GLFW/glfw3.h>



u8 bUp = 0;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	LogInfo("Program start");
	COMPONENT_CALL_INIT(Renderer, {
		LogError("Failed to start renderer component! Returning...");
		return SDL_APP_FAILURE;
		})
		COMPONENT_CALL_INIT_NOFAIL(Input);
	bUp = Input::GetBindingIndexFromBindingT(CName::AsHashed("up"));
	
	SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, "120");
	return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void* appstate) {
	u64 startTicks = SDL_GetTicks();
	
	Render::SetColor({ 0, 255, 0, 255 });
	Render::Clear();
	
	COMPONENT_CALL_UPDATE(Input);

	Render::FillRect({ 200, 200, 50, 50 }, { 255, 0, 0, 255 });
	Render::Present();

	return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	SDL_Quit();
}