#include <Dunjun/Common.hpp>
#include <Dunjun/ShaderProgram.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <string>
#include <fstream>

GLOBAL const int g_windowWidth = 854;
GLOBAL const int g_windowHeight = 480;

void glfwHints()
{
	glfwWindowHint(GLFW_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_VERSION_MINOR, 1);
};

int main(int argc, char** argv)
{
	GLFWwindow* window; // Window pointer

	if (!glfwInit())
	{
		return EXIT_FAILURE;
	}

	glfwHints();
	window = glfwCreateWindow(
	    g_windowWidth, g_windowHeight, "Dunjun", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window); // Context to use OpenGL functions

	// GLEW has to be init-ed after creating the context
	glewInit(); // Create all OpenGL pointers

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	float vertices[] = {
		+0.5f, +0.5f, 1.0f, 1.0f, 1.0f, // v0
		-0.5f, +0.5f, 0.0f, 0.0f, 1.0f, // v1
		+0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // v2
		-0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // v3
	};

	// Create vertex buffer
	GLuint vbo;
	glGenBuffers(1, &vbo);              // Generate
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // Bind
	glBufferData(GL_ARRAY_BUFFER,
	             sizeof(vertices),
	             vertices,
	             GL_STATIC_DRAW); // Add data

	Dunjun::ShaderProgram shaderProgram;
	shaderProgram.attachShaderFromFile(Dunjun::ShaderType::Vertex,
	                                   "data/shaders/default.vert.glsl");
	shaderProgram.attachShaderFromFile(Dunjun::ShaderType::Fragment,
	                                   "data/shaders/default.frag.glsl");
	shaderProgram.bindAttribLocation(0, "vertPosition");
	shaderProgram.bindAttribLocation(1, "vertColor");
	shaderProgram.link();
	shaderProgram.use();

	bool running = true;
	bool fullscreen = false;

	while (running)
	{
		// Render
		glClearColor(0.5f, 0.69f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw things
		{
			glEnableVertexAttribArray(0); // vertPosition
			glEnableVertexAttribArray(1); // vertColor

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const GLvoid*)(2 * sizeof(float)));

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glDisableVertexAttribArray(0); // vertPosition
			glDisableVertexAttribArray(1); // vertColor
		}

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
			glfwHints();
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