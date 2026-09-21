#include <core/string.hpp>

#include <print>

#include <core/logging.hpp>

#include <core/comp/renderer.hpp>
using namespace cl;

#include <GLFW/glfw3.h>

int main() {
	LogInfo("Program start");
	COMPONENT_CALL_INIT(Renderer, {
		LogError("Failed to start renderer component! Returning...");
		return 1;
	})

	while (!glfwWindowShouldClose(Renderer::gWindow)) {
		COMPONENT_CALL_UPDATE(Renderer);

		glfwPollEvents();
	}
	COMPONENT_CALL_UNLOAD(Renderer);
	return 0;
}