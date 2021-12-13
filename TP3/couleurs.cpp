#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>

#include <glimac/Program.hpp>

using namespace glimac;

int main(int argc, char** argv) {
	// Initialize SDL and open a window
	SDLWindowManager windowManager(800, 600, "GLImac");

	// Initialize glew for OpenGL3+ support
	GLenum glewInitError = glewInit();
	if(GLEW_OK != glewInitError) {
		std::cerr << glewGetErrorString(glewInitError) << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

	/*********************************
	 * HERE SHOULD COME THE INITIALIZATION CODE
	 *********************************/

	FilePath applicationPath(argv[0]);
	Program program = loadProgram(
		applicationPath.dirPath() + "shaders/triangle.vs.glsl",
		applicationPath.dirPath() + "shaders/triangle.fs.glsl"
	);
	program.use();
	
	GLfloat vertices[] = { 
		-0.5f, -0.5f, 1.f, 0.f, 0.f, // P1
		0.5f, -0.5f, 0.f, 1.f, 0.f, // P2
		0.0f, 0.5f, 0.f, 0.f, 1.f // P3
	};

	// VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 5 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// VAO - Position
	const GLuint VERTEX_ATTRIBUTE_POSITION = 3;
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(VERTEX_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VAO - Color
	const GLuint VERTEX_ATTRIBUTE_COLOR = 8;
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE_COLOR);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(VERTEX_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid*)(2 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VAO
	glBindVertexArray(0);

	// Application loop:
	bool done = false;
	while(!done) {
		// Event loop:
		SDL_Event e;
		while(windowManager.pollEvent(e)) {
			if(e.type == SDL_QUIT) {
				done = true; // Leave the loop after this iteration
			}
		}

		/*********************************
		 * HERE SHOULD COME THE RENDERING CODE
		 *********************************/
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		

		// Update the display
		windowManager.swapBuffers();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return EXIT_SUCCESS;
}
