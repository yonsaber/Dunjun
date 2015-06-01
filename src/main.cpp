#include <Dunjun/OpenGL.hpp>
#include <Dunjun/Common.hpp>
#include <Dunjun/ShaderProgram.hpp>
#include <Dunjun/Image.hpp>
#include <Dunjun/Texture.hpp>

#include <Dunjun/Clock.hpp>
#include <Dunjun/TickCounter.hpp>

#include <Dunjun/Color.hpp>
#include <Dunjun/Math.hpp>

#include <Dunjun/OpenGL.hpp>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

GLOBAL const int g_windowWidth = 854;
GLOBAL const int g_windowHeight = 480;

struct Vertex
{
	Dunjun::Vector2 position;
	Dunjun::Color color;
	Dunjun::Vector2 texCoord;
};

INTERNAL void glfwHints()
{
	glfwWindowHint(GLFW_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_VERSION_MINOR, 1);
	glfwSwapInterval(1);
};

INTERNAL void render()
{
	// Draw things
	glEnableVertexAttribArray(0); // vertPosition
	glEnableVertexAttribArray(1); // vertColor
	glEnableVertexAttribArray(2); // vertTexCoord

	glVertexAttribPointer(0,
	                      2,
	                      GL_FLOAT,
	                      GL_FALSE,
	                      sizeof(Vertex), // Stride
	                      (const GLvoid*)0);
	glVertexAttribPointer(1,
	                      4,
	                      GL_UNSIGNED_BYTE,
	                      GL_TRUE,
	                      sizeof(Vertex), // Stride
	                      (const GLvoid*)(sizeof(Dunjun::Vector2)));
	glVertexAttribPointer(
	    2,
	    2,
	    GL_FLOAT,
	    GL_FALSE,
	    sizeof(Vertex), // Stride
	    (const GLvoid*)(sizeof(Dunjun::Vector2) + sizeof(Dunjun::Color)));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(0); // vertPosition
	glDisableVertexAttribArray(1); // vertColor
	glDisableVertexAttribArray(2); // vertTexCoord
}

INTERNAL void handleInput(GLFWwindow* window, bool* running, bool* fullscreen)
{
	// TODO: Move this into a seperate window class
	if (glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		*running = false;
	}

	/*if (glfwGetKey(window, GLFW_KEY_F11))
	{
	*fullscreen = !fullscreen;

	GLFWwindow* newWindow;
	glfwHints();
	if (*fullscreen)
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
	}*/
}

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

	Vertex vertices[] = {
	    {{+0.5f, +0.5f}, {255, 255, 255, 255}, {1.0f, 0.0f}}, // v0
	    {{-0.5f, +0.5f}, {255, 0, 0, 255}, {0.0f, 0.0f}},     // v1
	    {{+0.5f, -0.5f}, {0, 255, 0, 255}, {1.0f, 1.0f}},     // v2
	    {{-0.5f, -0.5f}, {0, 0, 255, 255}, {0.0f, 1.0f}},     // v3
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

	if (!shaderProgram.attachShaderFromFile(Dunjun::ShaderType::Vertex,
	                                        "data/shaders/default.vert.glsl"))
	{
		throw std::runtime_error(shaderProgram.getErrorLog());
	}
	if (!shaderProgram.attachShaderFromFile(Dunjun::ShaderType::Fragment,
	                                        "data/shaders/default.frag.glsl"))
	{
		throw std::runtime_error(shaderProgram.getErrorLog());
	}
	shaderProgram.bindAttribLocation(0, "vertPosition");
	shaderProgram.bindAttribLocation(1, "vertColor");
	shaderProgram.bindAttribLocation(2, "vertTexCoord");
	if (!shaderProgram.link())
	{
		throw std::runtime_error(shaderProgram.getErrorLog());
	}
	// shaderProgram.use();

	Dunjun::Texture tex;
	tex.loadFromFile("data/textures/kitten.jpg");
	tex.bind(0);

	shaderProgram.setUniform("uniTex", 0);

	bool running = true;
	bool fullscreen = false;

	Dunjun::TickCounter tc;
	Dunjun::Clock frameClock;

	while (running)
	{
		{
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			glViewport(0, 0, width, height);
		}

		glClearColor(0.5f, 0.69f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.use();

		Dunjun::Matrix4 mat = Dunjun::translate({0.5f, 0.5f, 0.0f}) *
		                      Dunjun::rotate(3.14f / 3.0f, {0, 0, 1});

		shaderProgram.setUniform("uniModel", mat);

		render();

		shaderProgram.stopUsing();

		if (tc.update(0.5))
		{
			std::cout << tc.getTickRate() << std::endl;
			std::stringstream ss;
			ss << "Dunjun - " << 1000.0 / tc.getTickRate() << " ms";
			glfwSetWindowTitle(window, ss.str().c_str());
		}

		// Switch buffers
		glfwSwapBuffers(window);
		// Poll and process events
		glfwPollEvents();

		handleInput(window, &running, &fullscreen);

		while (frameClock.getElapsedTime() < 1.0 / 240)
			;
		frameClock.restart();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}