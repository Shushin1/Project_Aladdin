// Include standard headers
#define MINIAUDIO_IMPLEMENTATION

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

#include "assets/player.hpp"
#include "assets/lamp.hpp"
#include "assets/mesh_data.hpp"
#include "assets/enemy.hpp"


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

	if (ma_engine_init(NULL, &audioEngine) != MA_SUCCESS) {
		printf("Failed to initialize audio engine.");
		return -1;
	}

	ma_sound bgMusic;
	ma_sound_init_from_file(&audioEngine, "assets/audio/background.mp3", 0, NULL, NULL, &bgMusic);
	ma_sound_set_looping(&bgMusic, MA_TRUE); // Bonus: This makes the music loop endlessly!
	ma_sound_start(&bgMusic);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

        
	Mesh carpetMesh(carpet_vertices, sizeof(carpet_vertices), carpet_indices, sizeof(carpet_indices) / sizeof(GLuint));
	Mesh playerMesh(player_vertices, sizeof(player_vertices),player_indices, sizeof(player_indices) / sizeof(GLuint));
	Mesh lampMesh(lamp_vertices, sizeof(lamp_vertices), lamp_indices, sizeof(lamp_indices) / sizeof(GLuint));
	Mesh enemy1Mesh(enemy1_vertices, sizeof(enemy1_vertices), enemy1_indices, sizeof(enemy1_indices) / sizeof(GLuint));
	Mesh enemy2Mesh(enemy2_vertices, sizeof(enemy2_vertices), enemy2_indices, sizeof(enemy2_indices) / sizeof(GLuint));

	Carpet carpet(carpetMesh, glm::vec3(-2.5f, -0.3f, 0.0f));
	Player player(playerMesh, &lampMesh, &carpet);


	glm::vec3 positions[] = {
		glm::vec3(2.0f,  0.0f, 0.0f),  // [0] Front Center

		glm::vec3(1.2f,  0.0f, 0.0f),  // [1] Front Center
		glm::vec3(1.6f,  0.4f, 0.0f),  // [2] Behind Up
		glm::vec3(1.6f, -0.4f, 0.0f),  // [3] Behind Down

		glm::vec3(1.2f,  0.0f, 0.0f),  // [4] Front Center
		glm::vec3(1.6f,  0.3f, 0.0f),  // [5] Middle Up
		glm::vec3(1.6f, -0.3f, 0.0f),  // [6] Middle Down
		glm::vec3(2.0f,  0.6f, 0.0f),  // [7] Back Top
		glm::vec3(2.0f,  0.0f, 0.0f),  // [8] Back Center (Fills the gap!)
		glm::vec3(2.0f, -0.6f, 0.0f)   // [9] Back Bottom
	};

	Enemy* enemies[10];

	for (int i = 0;i < 10;i++) {
		int mesh = rand() % 2;

		switch (mesh) {
		case 0:
			enemies[i] = new Enemy(enemy1Mesh, positions[i]);
			break;
		case 1:
			enemies[i] = new Enemy(enemy2Mesh, positions[i]);
			break;
		}
	}

	int waves[] = { 1,3,6 };
	int maxWaves = sizeof(waves) / sizeof(waves[0]);

	int start = 0;
	int activeWave = 0;
	int kills = 0;

	bool gameOver = 0;


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

		if (!gameOver) {
			carpet.update(deltaTime);
			player.update(deltaTime);
		}


		carpet.draw(programID);
		player.draw(programID);



		if (activeWave < maxWaves && !gameOver) {
			int end = start + waves[activeWave];

			for (int i=start;i < end;i++) {
				if (!enemies[i]->isDead()) {
					enemies[i]->lookAt(player.getPosition());
					enemies[i]->update(deltaTime);
					enemies[i]->draw(programID);
				}
				if (player.getCurrentLamp() != NULL)
					if (player.getCurrentLamp()->isColliding(*enemies[i]) && !enemies[i]->isDead()) {
						player.removeCurrentLamp();
						enemies[i]->damage(1);
						if (enemies[i]->isDead()) kills++;
					}

				if (enemies[i]->getPosition().x <= player.getPosition().x) {
					gameOver = 1;
					ma_sound_stop(&bgMusic);
					player.die();
				}
			}
			}

			if (kills == waves[activeWave]) {
				start += waves[activeWave];
				activeWave++;
				kills = 0;
			}

			
		}


	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	ma_engine_uninit(&audioEngine);

	return 0;
}


