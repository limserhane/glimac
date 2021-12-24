#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <glimac/Program.hpp>
#include <glimac/glm.hpp>

using namespace glimac;

struct Vertex2DUV {
	glm::vec2 position;
	glm::vec2 texCoords;

	Vertex2DUV():
		position(glm::vec2(0.f, 0.f)),
		texCoords(glm::vec2(0.f, 0.f))
	{}

	Vertex2DUV(glm::vec2 pPosition, glm::vec2 pTextureCoordinates = glm::vec2(0.f, 0.f)):
		position(pPosition),
		texCoords(pTextureCoordinates)
	{}
};

glm::mat3 translate(float tx, float ty) {
  glm::mat3 t = glm::mat3(1.f);
  t[2].x = tx;
  t[2].y = ty;
  return t;
}

glm::mat3 scale(float sx, float sy) {
  glm::mat3 s = glm::mat3(1);
  s[0].x = sx;
  s[1].y = sy;
  return s;
}

glm::mat3 rotate(float a) {
  return glm::mat3(
    glm::vec3(glm::cos(a), glm::sin(a), 0),
    glm::vec3(-glm::sin(a), glm::cos(a), 0),
    glm::vec3(0, 0, 1)
  );
}

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
		applicationPath.dirPath() + "shaders/tex2Dmat.vs.glsl",
		applicationPath.dirPath() + "shaders/tex2Dmat.fs.glsl"
	);
	program.use();
	GLuint modelUniform = glGetUniformLocation(program.getGLId(), "uModelMatrix");

	int verticesCount = 3;
	Vertex2DUV vertices[] = {
		Vertex2DUV(glm::vec2(-1.f, -1.f)),
		Vertex2DUV(glm::vec2(1.f, -1.f)),
		Vertex2DUV(glm::vec2(0.f, 1.f))
	};

	// VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// VAO - Position
	int VERTEX_ATTRIBUTE_POSITION = 0;
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(VERTEX_ATTRIBUTE_POSITION, 2, GL_FLOAT, false, sizeof(Vertex2DUV), (const void*)offsetof(Vertex2DUV, position));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VAO - Texture coordinates
	int VERTEX_ATTRIBUTE_TEXCOORDS = 1;
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE_TEXCOORDS);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(VERTEX_ATTRIBUTE_TEXCOORDS, 2, GL_FLOAT, false, sizeof(Vertex2DUV), (const void*)offsetof(Vertex2DUV, texCoords));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	float timeStep = 0.01f;
	float time = 0;

	glm::mat3 modelMatrix;
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

		modelMatrix = rotate(time);

		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vao);
		glUniformMatrix3fv(modelUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glDrawArrays(GL_TRIANGLES, 0, verticesCount);

		time += timeStep;

		// Update the display
		windowManager.swapBuffers();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return EXIT_SUCCESS;
}
