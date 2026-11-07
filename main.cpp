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

//variables
GLFWwindow* window;
const int width = 1024, height = 1024;

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

	if (key == GLFW_KEY_W) {
		if(action == GLFW_PRESS) moveY = carpet_speed*deltaTime; //de ex, in loc de   carpet_speed*deltaTime;   sa avem carpet.move() sau ceva de genu 
																												//si functia sa fie implementata in clasa Carpet
		else if (action == GLFW_RELEASE)
		{
			moveY = 0.0f;
		}
	}

	if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS) moveY = -carpet_speed*deltaTime;
		else if(action == GLFW_RELEASE)
		{
			moveY = 0.0f;
		}
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

	GLfloat carpet_vertices[] = {
		0.05f,  0.05f, 0.0f,  // top right
		0.05f, -0.05f, 0.0f,  // bottom right
		-0.05f, -0.05f, 0.0f,  // bottom left
		-0.05f,  0.05f, 0.0f   // top left 
	};

	GLuint carpet_indices[] = {  // note that we start from 0!
		0, 3, 1,  // first Triangle
		1, 3, 2,   // second Triangle
	};

	GLfloat player_vertices[] = {
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

	GLuint player_indices[] = {  // note that we start from 0!
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


	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information for a complete rendered object. 
	GLuint vbo1, vao1, ibo1;
	glGenVertexArrays(1, &vao1);
	glGenBuffers(1, &vbo1);
	glGenBuffers(1, &ibo1);

	//bind VAO
	glBindVertexArray(vao1);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(carpet_vertices), carpet_vertices, GL_STATIC_DRAW);

	//bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(carpet_indices), carpet_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	//again, e foarte mult cod la buffere (trebuie definit cate un block de cod cu VBO,VAO,IBO, attribute pointer etc. si binding pt fiecare), poate facem
	//o functie care sa initializeze automat toate astea (trebuie doar sa ii dai niste parametri)(actually cred o clasa merge mai bine ca trebuie sa si 
	// instantieze bufferele sa fie accesibile global, nu doar in functie)

	// PLAYER 

	GLuint vbo2, vao2, ibo2;
	glGenVertexArrays(1, &vao2);
	glGenBuffers(1, &vbo2);
	glGenBuffers(1, &ibo2);

	//bind VAO
	glBindVertexArray(vao2);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(player_vertices), player_vertices, GL_STATIC_DRAW);

	//bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(player_indices), player_indices, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	//create matrices for transforms
	glm::mat4 carpet(1.0f);
	glm::mat4 player(1.0f);

	//maybe we can play with different positions
	glm::vec3 positions[] = {
		glm::vec3(0.0f,  0.0f,  0),
		glm::vec3(0.2f,  0.5f, 0),
		glm::vec3(-0.15f, -0.22f, 0),
		glm::vec3(-0.38f, -0.2f, 0),
		glm::vec3(0.24f, -0.4f, 0),
		glm::vec3(-0.17f,  0.3f, 0),
		glm::vec3(0.23f, -0.2f, 0),
		glm::vec3(0.15f,  0.2f, 0),
		glm::vec3(0.15f,  0.7f, 0),
		glm::vec3(-0.13f,  0.1f, 0)
	};

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

		// mult cod e comentat, era deja in lab4, dar am zis sa il comentez decat sa il sterg, poate ne trebuie la cv si sa nu cautam

		//rotation task based on glfw
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
		//with fixed angle
		//trans = glm::rotate(trans, 1.0f, glm::vec3(0.0, 0.0, 1.0));

		carpetX += moveX;
		carpetY += moveY;

		// cele 2 if-uri sunt pentru da bound la movement, sa nu iasa din ecran

		if (carpetY > 0.4f) carpetY = 0.4f;
		if (carpetY < -0.75f) carpetY = -0.75f;

		carpet = glm::mat4(1.0f);
		carpet = glm::translate(carpet, glm::vec3(carpetX, carpetY, 0.0f));
		carpet = glm::scale(carpet, glm::vec3(5.0f, 0.25f, 1.0f));
		
		
			//bind VAO
		glBindVertexArray(vao1);

			// send variables to shader
		unsigned int transformLoc = glGetUniformLocation(programID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(carpet));

		unsigned int transformLoc2 = glGetUniformLocation(programID, "color");
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 0.5f, 1.0);
		glUniform4fv(transformLoc2, 1, glm::value_ptr(color));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		//pana aici ne-am ocupat de covor (transformari, shader, desenat triunghiuri), de acum facem aceleasi lucruri pentru player


		player = glm::mat4(1.0f);
		player = glm::translate(player, glm::vec3(carpetX, carpetY + 0.27f, 0.0f));
		player = glm::scale(player, glm::vec3(0.5f, 0.5f, 1.0f));

		glBindVertexArray(vao2);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(player));
		color = glm::vec4(1.5f, 0.5f, 0.5f, 1.0);
		glUniform4fv(transformLoc2, 1, glm::value_ptr(color));

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

	// Cleanup
	glDeleteBuffers(1, &vbo1);
	glDeleteBuffers(1, &ibo1);
	glDeleteVertexArrays(1, &vao1);

	glDeleteBuffers(1, &vbo2);
	glDeleteBuffers(1, &ibo2);
	glDeleteVertexArrays(1, &vao2);

	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}


