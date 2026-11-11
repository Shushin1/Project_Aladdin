#pragma once
#include "game_object.hpp"
#include <cmath>

class Enemy : public GameObject {
public:
    Enemy(const Mesh& mesh, glm::vec3 startPosition)
        : GameObject(mesh, startPosition, glm::vec3(0.8f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
    {
        collision_radius = 0.2f;
        useGradient = 1;
    }

    void update(GLfloat deltaTime) override {
        position.x -= moveSpeed * deltaTime;
    }

    void lookAt(glm::vec3 targetPos) {
        float deltaY = targetPos.y - position.y;
        float deltaX = targetPos.x - position.x;

        angle = glm::degrees(-atan2(deltaY, -deltaX));
    }

    bool isDead() {
        return dead;
    }

    void kill() {
        dead = true;
        hp = 0;
        ma_engine_play_sound(&audioEngine, "assets/audio/die.wav", NULL);
    }

    void damage(int amount) {
        hp -= amount;
        color = colors[3 - hp];
        if (hp <= 0) kill();
        else ma_engine_play_sound(&audioEngine, "assets/audio/hit.wav", NULL);
        
    }


private:
    int hp = 3;
    bool dead = false;
    GLfloat moveSpeed = 0.25f;
    GLfloat rotationSpeed = 120.0f; // Radians per second
    glm::vec4 colors[3] = {
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
    };
};