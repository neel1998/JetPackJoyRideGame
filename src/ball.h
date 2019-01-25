#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void moveL();
    void moveR();
    void moveD();
    void jump();
    double speed;
    bounding_box_t bounding_box;
    int coins;
    int health;
    int stage;
    int gravity;
    bool onRing;
private:
    VAO *object;
};

#endif // BALL_H
