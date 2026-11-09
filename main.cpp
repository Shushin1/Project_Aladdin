// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "shader.hpp"
#include "globals.hpp"

#include "assets/player.hpp"
#include "assets/lamp.hpp"
#include "assets/mesh_data.hpp"

//variables

float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Handling cursor position
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << "The mouse cursor is: " << xpos << " " << ypos << std::endl;
}


// in callbackul asta ma gandeam sa gestionam toate key eventurile, desi poate ar merge mutate in clase actiunile
// o sa las mai jos un exemplu la ce ma refer, la tasta W

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		//glfwTerminate();
		glfwSetWindowShouldClose(window,true);
	}
}

//Ex 4: Complete callback for adjusting the viewport when resizing the window
void window_callback(GLFWwindow* window, int new_width, int new_height)
{
	glViewport(0, 0, new_width, new_height);
}

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Project_Aladdin", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	//specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

        
	Mesh carpetMesh(carpet_vertices, sizeof(carpet_vertices), carpet_indices, sizeof(carpet_indices) / sizeof(GLuint));
	Mesh playerMesh(player_vertices, sizeof(player_vertices),player_indices, sizeof(player_indices) / sizeof(GLuint));
	Mesh lampMesh(lamp_vertices, sizeof(lamp_vertices), lamp_indices, sizeof(lamp_indices) / sizeof(GLuint));

	Carpet carpet(carpetMesh, glm::vec3(-0.7, -0.7, 0));
	Player player(playerMesh, &lampMesh ,&carpet);

	// Set a callback for handling mouse cursor position
	// Decomment for a callback example
	// glfwSetCursorPosCallback(window, cursor_position_callback);

	//Ex4 - Set callback for window resizing
	glfwSetFramebufferSizeCallback(window, window_callback);
	glfwSetKeyCallback(window, key_callback);

	
	// Check if the window was closed
	while (!glfwWindowShouldClose(window))
	{
		//calculate delta time 
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		

		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		carpet.update(deltaTime);
		player.update(deltaTime);

		carpet.draw(programID);
		player.draw(programID);
	}


	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}


