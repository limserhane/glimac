#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/Sphere.hpp>

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

	glEnable(GL_DEPTH_TEST);

	FilePath applicationPath(argv[0]);
	Program program = loadProgram(
		applicationPath.dirPath() + "shaders/3D.vs.glsl",
		applicationPath.dirPath() + "shaders/normals.fs.glsl"
	);
	program.use();
	GLuint mvUniform = glGetUniformLocation(program.getGLId(), "uMVMatrix");
	GLuint mvpUniform = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
	GLuint normalUniform = glGetUniformLocation(program.getGLId(), "uNormalMatrix");

	Sphere sphere(1.f, 32, 16);

	// VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount() * sizeof(ShapeVertex), sphere.getDataPointer(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// VAO - Position
	const int VERTEX_ATTRIBUTE_POSITION = 0;
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(VERTEX_ATTRIBUTE_POSITION, 3, GL_FLOAT, false, sizeof(ShapeVertex), (const void*) offsetof(ShapeVertex, position));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// VAO - Normal
	const int VERTEX_ATTRIBUTE_NORMAL = 1;
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(VERTEX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, false, sizeof(ShapeVertex), (const void*) offsetof(ShapeVertex, normal));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VAO - TexCoords
	const int VERTEX_ATTRIBUTE_TEXCOORDS = 2;
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE_TEXCOORDS);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(VERTEX_ATTRIBUTE_TEXCOORDS, 2, GL_FLOAT, false, sizeof(ShapeVertex), (const void*) offsetof(ShapeVertex, texCoords));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VAO
	glBindVertexArray(0);

	// Uniforms
	glm::mat4 projectionMatrix = glm::perspective(70.f, (float)800/600, 0.1f, 100.f);
	glm::mat4 mvMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -5.f));
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(mvMatrix));

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(mvUniform, 1, false, glm::value_ptr(mvMatrix));
		glUniformMatrix4fv(normalUniform, 1, false, glm::value_ptr(normalMatrix));
		glUniformMatrix4fv(mvpUniform, 1, false, glm::value_ptr(projectionMatrix * mvMatrix));
		
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

		// Update the display
		windowManager.swapBuffers();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return EXIT_SUCCESS;
}
