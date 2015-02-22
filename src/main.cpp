#include <Dunjun/Common.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

	float vertices[] = {
	    0.0f,  // v1
	    0.5f,  // v1
	    -0.5f, // v2
	    -0.5f, // v2
	    0.5f,  // v3
	    -0.5f  // v3
	};

	// Create vertex buffer
	GLuint vbo;
	glGenBuffers(1, &vbo);              // Generate
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // Bind
	glBufferData(GL_ARRAY_BUFFER,
	             sizeof(vertices),
	             vertices,
	             GL_STATIC_DRAW); // Add data

	const char* vertexShaderText = {
	    "#version 120\n"
	    "\n"
	    "attribute vec2 position;" // Input item: attribute <type> <name>
	    "void main()"
	    "{"
	    "	gl_Position = vec4(position, 0.0, 1.0);"
	    "}"};

	const char* fragmentShaderText = {
	    "#version 120\n"
	    "\n"
	    "void main()"
	    "{"
	    "	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
	    "}"};

	// TODO: Seperate shader class
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderText, nullptr); // Get
	glCompileShader(vertexShader);                               // Compile

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderText, nullptr);
	glCompileShader(fragmentShader);

	// Attach shaders to a shader program
	GLuint shaderProgram =
	    glCreateProgram(); // Connect the small shaders together
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindAttribLocation(
	    shaderProgram, 0, "position"); // Before we link we can modify

	glLinkProgram(shaderProgram); // Link shader program

	glUseProgram(shaderProgram); // Use shader program

	bool running = true;
	bool fullscreen = false;

	while (running)
	{
		// Render
		glClearColor(0.5f, 0.69f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw things
		{
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

			glDrawArrays(GL_TRIANGLES, 0, 3);

			glDisableVertexAttribArray(0);
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