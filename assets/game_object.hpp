#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "mesh.hpp"

// Include GLEW
#include "../dependente/glew/glew.h"

// Include GLFW
#include "../dependente/glfw/glfw3.h"

// Include GLM
#include "../dependente/glm/core/type.hpp"
#include "../dependente/glm/glm.hpp"
#include "../dependente/glm/gtc/matrix_transform.hpp"
#include "../dependente/glm/gtc/type_ptr.hpp"

#include "../shader.hpp"

class GameObject {
    public:
        //asta e un initializer list, e o chestie cpp putin mai ciudata pt constructor dar mai scurta
        //si prima linie seteaza default la fiecare parametru, facandu-i optionali
        GameObject(const Mesh& mesh, glm::vec3 position=glm::vec3(0.0f), glm::vec3 scale=glm::vec3(1.0f), glm::vec4 color = glm::vec4(1.0f),  GLfloat angle=0.0f)
        : targetMesh(&mesh), position(position), scale(scale), color(color), angle(angle)
        {

        }

        //destructor virtual, daca asta nu e aici se pot intampla tampenii cand stergi obiectu
        virtual ~GameObject() {

        } 

        virtual void update(GLfloat deltaTime) = 0; //asa e in cpp o functie abstracta, asta o implementam in toate subclasele(gen lampa, aladin, etc) ca sa miste, etc, etc

        //asta e comuna intrea toate
        void draw(GLuint programID) {
            glm::mat4 matrix = glm::mat4(1.0f);
		    matrix = glm::translate(matrix, position);
            matrix = glm::rotate(matrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));
		    matrix = glm::scale(matrix, scale);
            
            unsigned int transformLoc = glGetUniformLocation(programID, "transform");
		    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));

		    unsigned int transformLoc2 = glGetUniformLocation(programID, "color");
		    glUniform4fv(transformLoc2, 1, glm::value_ptr(color));

            targetMesh->draw();
        }

        //in cpp functiile care nu editeaza obiectu(gettere) trb(e recomandat) sa fie const
        GLfloat getRadius() const {
            //scalam cercu cu maximu dintre x si y
            GLfloat max_scale = scale.x >= scale.y ? scale.x : scale.y;
            return collision_radius * max_scale;
        }

        bool isColliding(const GameObject& other) const {
            if (getRadius() == 0.0f || other.getRadius() == 0.0f) return false;
            return glm::distance(position, other.position) <= getRadius() + other.getRadius();
        }
    protected: //ca sa le vada subclasele
        const Mesh *targetMesh;
        glm::vec4 color;
        glm::vec3 position;
        glm::vec3 scale;
        GLfloat angle;
        GLfloat collision_radius = 0;
};