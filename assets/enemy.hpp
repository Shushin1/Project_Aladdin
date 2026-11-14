#pragma once
#include "game_object.hpp"
#include <cmath>
#include <cstdlib>

class Enemy : public GameObject {
public:
    Enemy(const Mesh& mesh, glm::vec3 startPosition)
        : GameObject(mesh, startPosition, glm::vec3(0.8f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
    {
        collision_radius = 0.2f;
        useGradient = 1;
        //random speed! rand da un nr intre 0 si rand_max, impartim sa fie intre 0 si 1, apoi inmultesti cu range si adaugi minimu
        GLfloat min = 0.15;
        GLfloat max = 0.4;
        moveSpeed = (GLfloat) rand() / RAND_MAX * (max-min) + min;
        //linie mediana pt sinus
        startY = startPosition.y;
        srand(rand()); //change seed sa fie diferit next time

        //chestii pt wooooobly
        GLfloat min_amp = 0.15, max_amp = 0.25;
        GLfloat min_freq = 5.0, max_freq = 10.0;
        frequency = (GLfloat) rand() / RAND_MAX * (max_freq-min_freq) + min_freq;
        amplitude = (GLfloat) rand() / RAND_MAX * (max_amp-min_amp) + min_amp;
    }

    void update(GLfloat deltaTime) override {
        position.x -= moveSpeed * deltaTime;
        position.y = startY + amplitude * sin(frequency * position.x);
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
    GLfloat startY;
    GLfloat frequency;
    GLfloat amplitude;
};