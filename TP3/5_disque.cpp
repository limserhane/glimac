#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <glimac/Program.hpp>
#include <glimac/glm.hpp>

using namespace glimac;

struct Vertex2DColor {
	glm::vec2 position;
	glm::vec3 color;

	Vertex2DColor(): 
		position(glm::vec2(0.f, 0.f)), 
		color(glm::vec3(0.f, 0.f, 0.f))
	{}

	Vertex2DColor(glm::vec2 p_position, glm::vec3 p_color):
		position(p_position),
		color(p_color)
	{}
};

int main(int argc, char** argv) {
	// Initialize SDL and open a window
	SDLWindowManager windowManager(800, 800, "GLImac"); // 800 x 600

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

	int N = 40;
	float r = 0.9f;

	int vertices_count = N * 3;
	std::vector<Vertex2DColor> vertices(vertices_count);
	float angle_current, angle_next;
	for(int i = 0; i < N; i ++)
	{
		angle_current = 2.f * glm::pi<float>() * (float)(i) / (float)(N);
		angle_next = 2.f * glm::pi<float>() * (float)(i + 1) / (float)(N);

		vertices[3 * i] = Vertex2DColor(glm::vec2(0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));

		vertices[3 * i + 1] = Vertex2DColor(
			glm::vec2(
				r * glm::cos(angle_current), 
				r * glm::sin(angle_current)
			), 
			glm::vec3(1.f, 1.f, 1.f)
		);

		vertices[3 * i + 2] = Vertex2DColor(
			glm::vec2(
				r * glm::cos(angle_next), 
				r * glm::sin(angle_next)
			), 
			glm::vec3(1.f, 1.f, 1.f)
		);
	}

	// VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(Vertex2DColor), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// VAO - Position
	const GLuint VERTEX_ATTRIBUTE_POSITION = 3;
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(VERTEX_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VAO - Color
	const GLuint VERTEX_ATTRIBUTE_COLOR = 8;
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE_COLOR);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(VERTEX_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)(offsetof(Vertex2DColor, color)));
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
		glDrawArrays(GL_TRIANGLES, 0, vertices_count);
		

		// Update the display
		windowManager.swapBuffers();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return EXIT_SUCCESS;
}
