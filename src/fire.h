#include "main.h"

#ifndef FIRE_H
#define FIRE_H


class Fire {
public:
    Fire() {}
    Fire(float x, float y, float angle, color_t color, float y_speed, bool up);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(int dir);
    void move();
    bounding_box_t bounding_box;
    bool collided;
    bool up;
    float speed;
private:
    VAO *object;
};

#endif 
