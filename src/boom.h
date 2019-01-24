#include "main.h"

#ifndef BOOM_H
#define BOOM_H


class Boom {
public:
    Boom() {}
    Boom(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void move();
    bounding_box_t bounding_box;
    bool collided;
private:
    VAO *object;
};

#endif 
