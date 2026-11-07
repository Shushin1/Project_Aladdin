// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


// Include GLEW
#include "dependente/glew/glew.h"

// Include GLFW
#include "dependente/glfw/glfw3.h"

// Include GLM
#include "dependente/glm/glm.hpp"
#include "dependente/glm/gtc/matrix_transform.hpp"
#include "dependente/glm/gtc/type_ptr.hpp"

#include "shader.hpp"
#include "globals.hpp"
#include <vector>

#include "player.hpp"


const int width = 1024, height = 1024;

GLFWwindow* window;

float carpetX = -0.7f;
float carpetY = -0.7f;
float carpet_speed = 1.5f;

float moveX = 0.0f;
float moveY = 0.0f;

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


	// exact chestiile astea care definesc un obiect (vertices, indices), dar si variabile care sa tina minte pozitia si alte atribute ale obiectului
	// as vrea sa le bagam in clase (putem incerca sa facem o clasa generica Object care sa contina pozitiile X, Y si dupa sa le dam extend specific
	// gen clasa Carpet, Player, Lamp, Enemy)

	 //din motive sa nu fie cpp cancer, daca nu vrem sa fim pe cpp17 or newer(suntem pe 14), trb ca meshu sa fie declarat in main
        std::vector<GLfloat> carpet_vertices = {
            0.05f,  0.05f, 0.0f,  // top right
            0.05f, -0.05f, 0.0f,  // bottom right
            -0.05f, -0.05f, 0.0f,  // bottom left
            -0.05f,  0.05f, 0.0f   // top left 
        };

        std::vector<GLuint> carpet_indices = {
            0, 3, 1,  // first Triangle
            1, 3, 2   // second Triangle
        };

	std::vector<GLfloat> player_vertices = {
		// ---------------- TORSO (Rectangle) ----------------
		-0.15f, -0.20f, 0.0f,  // 0: Bottom-left
		 0.15f, -0.20f, 0.0f,  // 1: Bottom-right
		 0.15f,  0.20f, 0.0f,  // 2: Top-right
		-0.15f,  0.20f, 0.0f,  // 3: Top-left

		// ---------------- HEAD (Square) --------------------
		-0.10f,  0.20f, 0.0f,  // 4: Bottom-left
		 0.10f,  0.20f, 0.0f,  // 5: Bottom-right
		 0.10f,  0.40f, 0.0f,  // 6: Top-right
		-0.10f,  0.40f, 0.0f,  // 7: Top-left

		// ---------------- FEZ (Trapez) ---------------------
		-0.07f,  0.4f, 0.0f,  // 8:  Bottom-left
			0.02f,  0.4f, 0.0f,  // 9:  Bottom-right
		0.0f,  0.475f, 0.0f,  // 10: Top-right
		-0.05f,  0.475f, 0.0f,  // 11: Top-left

		// ---------------- LEFT ARM (Rectangle) -------------
		-0.20f, -0.1f, 0.0f,  // 12: Bottom-left
		-0.16f, -0.1f, 0.0f,  // 13: Bottom-right
		-0.15f,  0.15f, 0.0f,  // 14: Top-right
		-0.20f,  0.15f, 0.0f,  // 15: Top-left

		// ---------------- RIGHT ARM (Rectangle) ------------
		 0.16f, -0.1f, 0.0f,  // 16: Bottom-left
	 	0.20f, -0.1f, 0.0f,  // 17: Bottom-right
		 0.20f,  0.15f, 0.0f,  // 18: Top-right
		 0.15f,  0.15f, 0.0f,  // 19: Top-left

	 	// ---------------- LEFT LEG (Right Triangle) --------
	 	-0.15f, -0.20f, 0.0f,  // 20: Top-Outer (Right angle)
	 	 0.00f, -0.20f, 0.0f,  // 21: Top-Inner (Center point)
	 	-0.15f, -0.50f, 0.0f,  // 22: Bottom-Outer (Foot point)

	 	// ---------------- RIGHT LEG (Right Triangle) -------
	 	 0.15f, -0.20f, 0.0f,  // 23: Top-Outer (Right angle)
	 	 0.00f, -0.20f, 0.0f,  // 24: Top-Inner (Center point)
	 	 0.15f, -0.50f, 0.0f   // 25: Bottom-Outer (Foot point)
	};

	std::vector<GLuint> player_indices = {  // note that we start from 0!
		// Torso (2 triangles)
		0, 1, 2,   0, 2, 3,
		// Head (2 triangles)
		4, 5, 6,   4, 6, 7,
		// Fez (2 triangles)
		8, 9, 10,  8, 10, 11,
		// Left Arm (2 triangles)
		12, 13, 14,  12, 14, 15,
		// Right Arm (2 triangles)
		16, 17, 18,  16, 18, 19,
		// Left Leg (1 triangle)
		20, 21, 22,
		// Right Leg (1 triangle)
		23, 24, 25
	};


	//CARPET
	Mesh carpetMesh(carpet_vertices, carpet_indices);
	Mesh playerMesh(player_vertices, player_indices);
	Carpet carpet(carpetMesh, glm::vec3(-0.7, -0.7, 0));
	Player player(playerMesh, &carpet);


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

		// mult cod e comentat, era deja in lab4, dar am zis sa il comentez decat sa il sterg, poate ne trebuie la cv si sa nu cautam

		//rotation task based on glfw
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
		//with fixed angle
		//trans = glm::rotate(trans, 1.0f, glm::vec3(0.0, 0.0, 1.0));
	}

	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}


