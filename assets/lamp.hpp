#pragma once
#include "game_object.hpp"

class Lamp : public GameObject {
public:
    Lamp(const Mesh& mesh, glm::vec3 startPosition)
        : GameObject(mesh, startPosition, glm::vec3(1.0f), glm::vec4(1.0f, 0.8f, 0.0f, 1.0f)) 
    {
        collision_radius = 0.02f;
    }

    void update(GLfloat deltaTime) override {
        position.x += moveSpeed * deltaTime;
        angle -= rotationSpeed * deltaTime; 
    }

private:
    GLfloat moveSpeed = 1.5f;
    GLfloat rotationSpeed = 120.0f; // Radians per second
};