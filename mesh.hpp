#pragma once
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
#include <vector>



class Mesh {
    private:
    GLuint vbo, vao, ibo;

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
	
    public:
        Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices) {
            this->vertices = vertices;
            this->indices = indices;
            glGenVertexArrays(1, &vao);
	        glGenBuffers(1, &vbo);
	        glGenBuffers(1, &ibo);


	        //bind VAO
	        glBindVertexArray(vao);

	        //bind VBO
	        glBindBuffer(GL_ARRAY_BUFFER, vbo);
	        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	        //bind IBO
	        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLfloat), indices.data(), GL_STATIC_DRAW);

	        glVertexAttribPointer(
		        0,                  // attribute 0, must match the layout in the shader.
		        3,                  // size of each attribute
		        GL_FLOAT,           // type
		        GL_FALSE,           // normalized?
		        3 * sizeof(float),  // stride
		        (void*)0            // array buffer offset
	        );
	        glEnableVertexAttribArray(0);
        }

        ~Mesh() {
            glDeleteBuffers(1, &vbo);
	        glDeleteBuffers(1, &ibo);
	        glDeleteVertexArrays(1, &vao);
        }

        void draw(int startIndex) const { //daca nu e const nu pot sa o apelez din game object cand mesh e const pointer
            glBindVertexArray(vao);
            //aicea trb cast nebun si inmultire ptc e byte offset, se poate pune doar 0
            //scaderea la ultimu param e sa nu dea afara din buffer
            glDrawElements(GL_TRIANGLES, indices.size() - startIndex, GL_UNSIGNED_INT, (void*) (sizeof(GLuint)*startIndex));
        }
};