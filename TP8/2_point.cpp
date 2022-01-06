#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Image.hpp>
#include <glimac/FreeflyCamera.hpp>

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
		applicationPath.dirPath() + "shaders/pointlight.fs.glsl"
	);
	program.use();
	GLuint mvUniform = glGetUniformLocation(program.getGLId(), "uMVMatrix");
	GLuint mvpUniform = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
	GLuint normalUniform = glGetUniformLocation(program.getGLId(), "uNormalMatrix");
	GLuint kdUniform = glGetUniformLocation(program.getGLId(), "uKd");
	GLuint ksUniform = glGetUniformLocation(program.getGLId(), "uKs");
	GLuint shininessUniform = glGetUniformLocation(program.getGLId(), "uShininess");
	GLuint lightPositionUniform = glGetUniformLocation(program.getGLId(), "uLightPos_vs");
	GLuint lightIntensityUniform = glGetUniformLocation(program.getGLId(), "uLightIntensity");


	Sphere sphere(1.f, 512, 256);
	FreeflyCamera camera;
	
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
	glm::mat4 viewMatrix;

	glm::mat4 earthModelMatrix, sphereNormalMatrix;
	glm::mat4 moonModelMatrix, moonNormalMatrix;

	glm::vec3 spherePosition = glm::vec3(0.f, 0.f, -5.f);
	float earthScale = 1.f;
	float moonScale = 0.2f;

	glm::vec4 initialLightPosition = glm::vec4(spherePosition + glm::vec3(0.f, 2 * earthScale, 0.f), 1.f);
	glm::mat4 lightModelMatrix;
	glm::vec4 lightPosition_vs;

	int nMoons = 8; // 16
	std::vector<glm::vec3[2]> moonParameters(nMoons);
	for(int i = 0; i < nMoons; i++ )
	{
		moonParameters[i][0] = glm::sphericalRand(2 * earthScale);
		moonParameters[i][1] = glm::sphericalRand(2 * earthScale);
	}

	glUniform3f(lightIntensityUniform, 5, 5, 5);

	// Application loop:
	bool done = false;
	while(!done) {
		// Event loop:
		SDL_Event e;
		while(windowManager.pollEvent(e)) {
			switch (e.type)
			{
			case SDL_QUIT:
				done = true;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_LEFT:
					camera.rotateLeft(0.5f);
					break;
				case SDLK_RIGHT:
					camera.rotateLeft(- 0.5f);
					break;
				case SDLK_UP:
					camera.rotateUp(0.5f);
					break;
				case SDLK_DOWN:
					camera.rotateUp(-0.5f);
					break;
				case SDLK_z:
					camera.moveFront(1.f);
					break;
				case SDLK_s:
					camera.moveFront(-1.f);
					break;
				case SDLK_a:
					camera.moveLeft(1.f);
					break;
				case SDLK_e:
					camera.moveLeft(-1.f);
					break;
				}
				break;
			}
		}

		/*********************************
		 * HERE SHOULD COME THE RENDERING CODE
		 *********************************/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		viewMatrix = camera.getViewMatrix();
		// Sphere

		earthModelMatrix = glm::translate(glm::mat4(1.f), spherePosition);
		earthModelMatrix = glm::scale(earthModelMatrix, glm::vec3(earthScale, earthScale, earthScale));
		
		sphereNormalMatrix = glm::transpose(glm::inverse(viewMatrix * earthModelMatrix));

		glUniformMatrix4fv(mvUniform, 1, false, glm::value_ptr(viewMatrix * earthModelMatrix));
		glUniformMatrix4fv(normalUniform, 1, false, glm::value_ptr(sphereNormalMatrix));
		glUniformMatrix4fv(mvpUniform, 1, false, glm::value_ptr(projectionMatrix * viewMatrix * earthModelMatrix));

		lightModelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(
			2 * glm::cos(0.5f * windowManager.getTime()), 
			0.f, 
			2 * glm::sin(0.5f * windowManager.getTime())
		));
		lightPosition_vs = viewMatrix * lightModelMatrix * initialLightPosition;
		glUniform3f(lightPositionUniform, lightPosition_vs.x, lightPosition_vs.y, lightPosition_vs.z);
		
		glUniform3f(kdUniform, 1, 1, 1);
		glUniform3f(ksUniform, 1, 1, 1);
		glUniform1f(shininessUniform, 16);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

		// Lunes

		for(auto randomPosition : moonParameters)
		{
			moonModelMatrix = glm::translate(glm::mat4(1.f), spherePosition); // Translation
			moonModelMatrix = glm::rotate(moonModelMatrix, windowManager.getTime(), glm::cross(randomPosition[0], randomPosition[1]));
			moonModelMatrix = glm::translate(moonModelMatrix, randomPosition[0]);
			moonModelMatrix = glm::scale(moonModelMatrix, glm::vec3(moonScale, moonScale, moonScale));

			moonNormalMatrix = glm::transpose(glm::inverse(viewMatrix * moonModelMatrix));

			glUniformMatrix4fv(mvUniform, 1, false, glm::value_ptr(viewMatrix * moonModelMatrix));
			glUniformMatrix4fv(normalUniform, 1, false, glm::value_ptr(moonNormalMatrix));
			glUniformMatrix4fv(mvpUniform, 1, false, glm::value_ptr(projectionMatrix * viewMatrix * moonModelMatrix));

			glUniform3f(kdUniform, 1, 1, 1);
			glUniform3f(ksUniform, 1, 1, 1);
			glUniform1f(shininessUniform, 256);
			
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
		}

		// Update the display
		windowManager.swapBuffers();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return EXIT_SUCCESS;
}
