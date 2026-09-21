#include <core/comp/renderer.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <core/logging.hpp>

CORE_DECLARE_NAMESPACE

namespace Renderer {

	GLFWwindow* gWindow = nullptr;

	CFloatColor gColor = CFloatColor(0, 255, 0, 255);

	glID gVertexBufferObject = glInvalid;
	glID gVertexArrayObject = glInvalid;
	glID gEndicesBufferObject = glInvalid;

	COMPONENT_IMPLEMENT_INIT() {
		if (!glfwInit()) {
			LogError("(Component::Render::Init) Failed to initialize GLFW! returning false...");
			return false;
		}
		gWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Application", NULL, NULL);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		if (!gWindow) {
			LogError("(Component::Renderer::Init) Failed to create window! returning false...");
			return false;
		}
		LogInfo("(Component::Renderer::Init) Success!");

		glfwMakeContextCurrent(gWindow);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			LogError("(Component::Render::Init) Failed to initialize GL Loader!");
			return false;
		}

		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);


		return true;
	}

	COMPONENT_IMPLEMENT_UPDATE() {
		glClearColor(gColor.r, gColor.g, gColor.b, gColor.a);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(gWindow);
		//glClearColor(CORE_RENDER_CCOLOR_DECLARE_AS_PARAMS(gColor));
		//glClear(GL_COLOR_BUFFER_BIT);
	}

	COMPONENT_IMPLEMENT_UNLOAD() {
		glfwTerminate();
	}
}

namespace Render {

}

CORE_END_NAMESPACE