#pragma once
#include "game_object.hpp"

class Enemy : public GameObject {
public:
    Enemy(const Mesh& mesh, glm::vec3 startPosition)
        : GameObject(mesh, startPosition, glm::vec3(1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)) 
    {
        collision_radius = 0.3f;
    }

    void update(GLfloat deltaTime) override {
       
    }

    bool isDead() {
        return dead;
    }

    void kill() {
        dead = true;
    }
private:
    bool dead = false;
    GLfloat moveSpeed = 1.5f;
    GLfloat rotationSpeed = 120.0f; // Radians per second
};