#pragma once
#include "game_object.hpp"
#include "carpet.hpp"
#include "lamp.hpp"

class Player : public GameObject {
    public:
        Player(const Mesh& mesh,Mesh* lamp, Carpet* carpet, glm::vec3 position = glm::vec3(0.0f))
        : GameObject(mesh, position, glm::vec3(0.5f, 0.5f, 1.0f), glm::vec4(1.5f, 0.5f, 0.5f, 1.0))
        {
            this->lamp = lamp;
            this->carpet = carpet;
            this->activeLamp = NULL;
            this->lampTimer = 0.0f;
            this->armAngle = 0.0f;
        }

        void update(GLfloat deltaTime) override {
            position = carpet->getPosition();
            position.y = carpet->getPosition().y + 0.27f;

            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                if (activeLamp == NULL) {
                    attack();
                }
            }

            if (activeLamp != NULL) {
                activeLamp->update(deltaTime);
                lampTimer += deltaTime;

                if (lampTimer < 0.25f) {
                    armAngle = 90.0f;
                }
                else {
                    armAngle = 0.0f;
                }

                if (lampTimer >= 1.0f) {
                    delete activeLamp;
                    activeLamp = NULL;
                    armAngle = 0.0f;
                }
            }
        }

        void attack() {
            activeLamp = new Lamp(*lamp, glm::vec3(position.x + 0.15f, position.y, 0.0f));
            lampTimer = 0.0f;
            armAngle = 45.0f;
        }

        void draw(GLuint programID) {

            // am dat override la functia draw din game_object ca sa putem face pe bucati colorarea si animatiile

            glm::mat4 matrix = glm::mat4(1.0f);
            matrix = glm::translate(matrix, position);
            matrix = glm::scale(matrix, scale);

            unsigned int transformLoc = glGetUniformLocation(programID, "transform");
            unsigned int transformLoc2 = glGetUniformLocation(programID, "color");

            // intai ne ocupat de partile statice ale playerului si trimitem locatia
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));


            //acum ne ocupam de culori si o facem pe bucati, pe indici (valorile pentru indici sunt si in mesh_data, dar le-am trecut iar aici)

            // Torso (Index 0, Length 6)
            glUniform4fv(transformLoc2, 1, glm::value_ptr(glm::vec4(0.4f, 0.1f, 0.6f, 1.0f))); // Purple
            targetMesh->drawPartial(6, 0);

            // Head (Index 6, Length 6)
            glUniform4fv(transformLoc2, 1, glm::value_ptr(glm::vec4(0.9f, 0.7f, 0.5f, 1.0f))); // Skin Tone
            targetMesh->drawPartial(6, 6);

            // Fez (Index 12, Length 6)
            glUniform4fv(transformLoc2, 1, glm::value_ptr(glm::vec4(0.8f, 0.1f, 0.1f, 1.0f))); // Red
            targetMesh->drawPartial(6, 12);

            // Left Arm (Index 18, Length 6)
            glUniform4fv(transformLoc2, 1, glm::value_ptr(glm::vec4(0.9f, 0.7f, 0.5f, 1.0f))); // Skin Tone
            targetMesh->drawPartial(6, 18);

            // Left Leg (Index 30, Length 3)
            glUniform4fv(transformLoc2, 1, glm::value_ptr(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f))); // White
            targetMesh->drawPartial(3, 30);

            // Right Leg (Index 33, Length 3)
            glUniform4fv(transformLoc2, 1, glm::value_ptr(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f))); // White
            targetMesh->drawPartial(3, 33);

            // animare separat mana dreapta
            glm::mat4 armMat = matrix;

            armMat = glm::translate(armMat, glm::vec3(0.15f, 0.15f, 0.0f));
            armMat = glm::rotate(armMat, armAngle, glm::vec3(0.0f, 0.0f, 1.0f));
            armMat = glm::translate(armMat, glm::vec3(-0.15f, -0.15f, 0.0f));

            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(armMat));
            glUniform4fv(transformLoc2, 1, glm::value_ptr(glm::vec4(0.9f, 0.7f, 0.5f, 1.0f))); // Skin Tone
            targetMesh->drawPartial(6, 24);

            if (activeLamp != NULL) {
                activeLamp->draw(programID);
            }
        }
    private:
        Carpet* carpet;
        Mesh* lamp;
        Lamp* activeLamp;
        float lampTimer;
        float armAngle;
};