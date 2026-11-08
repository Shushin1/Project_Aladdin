#pragma once
#include "../dependente/glew/glew.h"


//CARPET

GLfloat carpet_vertices[] = {
            0.05f,  0.05f, 0.0f,  // top right
            0.05f, -0.05f, 0.0f,  // bottom right
            -0.05f, -0.05f, 0.0f,  // bottom left
            -0.05f,  0.05f, 0.0f   // top left 
};

GLuint carpet_indices[] = {
    0, 3, 1,  // first Triangle
    1, 3, 2   // second Triangle
};


//PLAYER

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
	-0.20f, -0.15f, 0.0f,  // 12: Bottom-left
	-0.16f, -0.15f, 0.0f,  // 13: Bottom-right
	-0.15f,  0.15f, 0.0f,  // 14: Top-right
	-0.20f,  0.15f, 0.0f,  // 15: Top-left


	// ---------------- RIGHT ARM (Rectangle) ------------
	 0.16f, -0.15f, 0.0f,  // 16: Bottom-left
	 0.20f, -0.15f, 0.0f,  // 17: Bottom-right
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
	0, 1, 2,     0, 2, 3,       // Torso     (Starts at index 0, length 6)
	4, 5, 6,     4, 6, 7,       // Head      (Starts at index 6, length 6)
	8, 9, 10,    8, 10, 11,     // Fez       (Starts at index 12, length 6)
	12, 13, 14,  12, 14, 15,    // Left Arm  (Starts at index 18, length 6)
	16, 17, 18,  16, 18, 19,    // Right Arm (Starts at index 24, length 6)
	20, 21, 22,                 // Left Leg  (Starts at index 30, length 3)
	23, 24, 25                  // Right Leg (Starts at index 33, length 3)
};

//LAMP

GLfloat lamp_vertices[] = {
	0.00f,  0.00f, 0.0f,  // 0: Center
	0.00f,  0.05f, 0.0f,  // 1: Top
	0.08f,  0.00f, 0.0f,  // 2: Right (Pipe)
	0.00f, -0.05f, 0.0f,  // 3: Bottom
   -0.06f,  0.00f, 0.0f   // 4: Left (Handle)
};

GLuint lamp_indices[] = {
	0, 1, 2,  // Top-Right triangle
	0, 2, 3,  // Bottom-Right triangle
	0, 3, 4,  // Bottom-Left triangle
	0, 4, 1   // Top-Left triangle
};