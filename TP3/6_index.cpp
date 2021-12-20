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

	void print() const
	{
		std::cout << "{" << position << " - " << color << "}";
	}
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

	int N = 50;
	float r = 0.9f;

	int vertices_count = N + 1; // N border + 1 center
	int indices_count = N * 3; // 3 vertices for N triangles

	std::vector<Vertex2DColor> vertices(vertices_count);
	std::vector<uint32_t> indices(indices_count);

	int i;
	float angle;

	// vertices
	vertices[0] = Vertex2DColor(glm::vec2(0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	for(i = 0; i < N; i ++)
	{
		angle = 2.f * glm::pi<float>() * (float)(i) / (float)(N);
		vertices[i + 1] = Vertex2DColor(glm::vec2(r * glm::cos(angle), r * glm::sin(angle)), glm::vec3(1.f, 1.f, 1.f));
	}

	// indices
	for(i = 0; i < N - 1; i ++)
	{
		indices[3 * i] = 0;
		indices[3 * i + 1] = i + 1;
		indices[3 * i + 2] = i + 2;
	}
	// last index
	indices[3 * i] = 0;
	indices[3 * i + 1] = i + 1;
	indices[3 * i + 2] = 1;

	// VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(Vertex2DColor), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// IBO
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(0, ibo);

	// VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

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

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // must unbind ibo AFTER vao, otherwise vao registers the unbind;

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
		glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
		

		// Update the display
		windowManager.swapBuffers();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return EXIT_SUCCESS;
}
