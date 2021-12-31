#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Image.hpp>
#include <glimac/FreeflyCamera.hpp>

using namespace glimac;

struct EarthProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;

    EarthProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.getGLId(), "uEarthTexture");
        uCloudTexture = glGetUniformLocation(m_Program.getGLId(), "uCloudTexture");
    }
};

struct MoonProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;

    MoonProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/tex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
    }
};

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
	EarthProgram earthProgram(applicationPath);
	MoonProgram moonProgram(applicationPath);

	Sphere sphere(1.f, 32, 16);
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

	// Texttures
	std::unique_ptr<Image> earthImage = loadImage("/mnt/2E1C47BA1C477C39/esiee/e5/opengl/glimac/assets/textures/EarthMap.jpg");
	std::unique_ptr<Image> moonImage = loadImage("/mnt/2E1C47BA1C477C39/esiee/e5/opengl/glimac/assets/textures/MoonMap.jpg");
	std::unique_ptr<Image> cloudImage = loadImage("/mnt/2E1C47BA1C477C39/esiee/e5/opengl/glimac/assets/textures/CloudMap.jpg");
	GLuint earthTexture, moonTexture, cloudTexture;

	const int TEXTURE_SLOT_EARTH = 0;
	glGenTextures(1, &earthTexture);
	glActiveTexture(GL_TEXTURE0 + TEXTURE_SLOT_EARTH);
	glBindTexture(GL_TEXTURE_2D, earthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, earthImage->getWidth(), earthImage->getHeight(), 0, GL_RGBA, GL_FLOAT, earthImage->getPixels());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	const int TEXTURE_SLOT_MOON = 1;
	glGenTextures(1, &moonTexture);
	glActiveTexture(GL_TEXTURE0 + TEXTURE_SLOT_MOON);
	glBindTexture(GL_TEXTURE_2D, moonTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, moonImage->getWidth(), moonImage->getHeight(), 0, GL_RGBA, GL_FLOAT, moonImage->getPixels());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	const int TEXTURE_SLOT_CLOUD = 2;
	glGenTextures(1, &cloudTexture);
	glActiveTexture(GL_TEXTURE0 + TEXTURE_SLOT_CLOUD);
	glBindTexture(GL_TEXTURE_2D, cloudTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cloudImage->getWidth(), cloudImage->getHeight(), 0, GL_RGBA, GL_FLOAT, cloudImage->getPixels());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Uniforms
	glm::mat4 projectionMatrix = glm::perspective(70.f, (float)800/600, 0.1f, 100.f);
	glm::mat4 viewMatrix;

	glm::mat4 sphereModelMatrix, sphereNormalMatrix;
	glm::mat4 moonModelMatrix, moonNormalMatrix;

	glm::vec3 spherePosition = glm::vec3(0.f, 0.f, -5.f);
	
	int nMoons = 16;
	std::vector<glm::vec3[2]> moonParameters(nMoons);
	for(int i = 0; i < nMoons; i++ )
	{
		moonParameters[i][0] = glm::sphericalRand(2.f);
		moonParameters[i][1] = glm::sphericalRand(2.f);
	}

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
			
			case SDL_MOUSEMOTION:
				camera.rotateUp(0.5f * - e.motion.yrel);
				camera.rotateLeft(0.5f * - e.motion.xrel);

			case SDL_MOUSEBUTTONUP:
				switch (e.button.button)
				{
				case SDL_BUTTON_WHEELUP:
					camera.moveFront(0.2f);
					break;

				case SDL_BUTTON_WHEELDOWN:
					camera.moveFront(-0.2f);
					break;
				}
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

		sphereModelMatrix = glm::translate(glm::mat4(1.f), spherePosition);
		
		sphereNormalMatrix = glm::transpose(glm::inverse(viewMatrix * sphereModelMatrix));

		earthProgram.m_Program.use();
		
		glUniformMatrix4fv(earthProgram.uMVMatrix, 1, false, glm::value_ptr(viewMatrix * sphereModelMatrix));
		glUniformMatrix4fv(earthProgram.uNormalMatrix, 1, false, glm::value_ptr(sphereNormalMatrix));
		glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, false, glm::value_ptr(projectionMatrix * viewMatrix * sphereModelMatrix));

		glUniform1i(earthProgram.uEarthTexture, TEXTURE_SLOT_EARTH);
		glUniform1i(earthProgram.uCloudTexture, TEXTURE_SLOT_CLOUD);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

		// Lunes

		moonProgram.m_Program.use();
		for(auto randomDirection : moonParameters)
		{
			moonModelMatrix = sphereModelMatrix; // Translation
			moonModelMatrix = glm::rotate(moonModelMatrix, windowManager.getTime(), glm::cross(randomDirection[0], randomDirection[1]));
			moonModelMatrix = glm::translate(moonModelMatrix, randomDirection[0]);
			moonModelMatrix = glm::scale(moonModelMatrix, glm::vec3(0.2, 0.2, 0.2));

			moonNormalMatrix = glm::transpose(glm::inverse(viewMatrix * moonModelMatrix));

			glUniformMatrix4fv(moonProgram.uMVMatrix, 1, false, glm::value_ptr(viewMatrix * moonModelMatrix));
			glUniformMatrix4fv(moonProgram.uNormalMatrix, 1, false, glm::value_ptr(moonNormalMatrix));
			glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, false, glm::value_ptr(projectionMatrix * viewMatrix * moonModelMatrix));
			
			glUniform1i(moonProgram.uTexture, TEXTURE_SLOT_MOON);

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
