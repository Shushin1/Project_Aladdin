#pragma once
#include "game_object.hpp"

class Carpet : public GameObject {
    public:
        //instantiem covoru dar luam doar pozitia ca parametru, culoarea si scale sunt hardcodded
        Carpet(const Mesh& mesh, glm::vec3 position=glm::vec3(0.0f))
        : GameObject(mesh, position, glm::vec3(5.0f, 0.25f, 1.0f), glm::vec4(1.0f, 1.0f, 0.5f, 1.0)) 
        {

        }

        glm::vec3 getPosition() {
            return position;
        } 

        void update(GLfloat deltaTime) override {
            GLfloat direction = 0; //fac asta ca sa repar bug-ul de tii apasat pe W si daca apesi pe S se opreste si nu mai porneste
            if(glfwGetKey(window, GLFW_KEY_W)) {
                direction += 1;
            }
            if(glfwGetKey(window, GLFW_KEY_S)) {
                direction -= 1;
            }
            move(deltaTime, direction);
        }

        void move(GLfloat deltaTime, GLfloat direction) {
            position.y += direction * deltaTime * speed;
            if(position.y > 0.4)
                position.y = 0.4;
            if(position.y < -0.75)
                position.y = -0.75;
        }
    private:
        GLfloat speed = 1.5f;
};