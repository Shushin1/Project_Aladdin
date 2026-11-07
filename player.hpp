#pragma once
#include "game_object.hpp"
#include "dependente/glfw/glfw3.h"
#include "globals.hpp"
#include "mesh.hpp"
#include "carpet.hpp"

class Player : public GameObject {
    public:
        Player(const Mesh& mesh, Carpet* carpet, glm::vec3 position=glm::vec3(0.0f))
        : GameObject(mesh, position, glm::vec3(0.5f, 0.5f, 1.0f), glm::vec4(1.5f, 0.5f, 0.5f, 1.0))
        {
            this->carpet = carpet;
        }

        void update(GLfloat deltaTime) override {
            position = carpet->getPosition();
            position.y = carpet->getPosition().y + 0.27f;
        }
    private:
        Carpet* carpet;
};