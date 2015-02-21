#include <Dunjun/Common.hpp>

#include <GLFW/glfw3.h>

#include <iostream>

GLOBAL const int g_windowWidth = 854;
GLOBAL const int g_windowHeight = 480;

int main(int argc, char** argv)
{
	GLFWwindow* window; // Window pointer

	if (!glfwInit())
	{
		return EXIT_FAILURE;
	}

	window = glfwCreateWindow(
	    g_windowWidth, g_windowHeight, "Dunjun", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window); // Context to use OpenGL functions

	bool running = true;
	bool fullscreen = false;

	while (running)
	{
		// Render
		glClearColor(0.5f, 0.69f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Switch buffers
		glfwSwapBuffers(window);
		// Poll and process events
		glfwPollEvents();

		// TODO: Move this into a seperate window class
		if (glfwWindowShouldClose(window) ||
		    glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			running = false;
		}

		if (glfwGetKey(window, GLFW_KEY_F11))
		{
			fullscreen = !fullscreen;

			GLFWwindow* newWindow;
			if (fullscreen)
			{
				const GLFWvidmode* videoMode = glfwGetVideoMode(
				    glfwGetPrimaryMonitor()); // Fetch the video mode of the
				                              // primary display
				newWindow =
				    glfwCreateWindow(videoMode->width,
				                     videoMode->height,
				                     "Dunjun",
				                     glfwGetPrimaryMonitor(),
				                     window); // Reference old OpenGL context
			}
			else
			{
				newWindow = glfwCreateWindow(
				    g_windowWidth, g_windowHeight, "Dunjun", nullptr, nullptr);
			}

			glfwDestroyWindow(window);
			window = newWindow;
			glfwMakeContextCurrent(window);
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}