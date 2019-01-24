#include "main.h"

#ifndef POWER_H
#define POWER_H


class Power {
public:
    Power() {}
    Power(float x, float y, color_t color, int type);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bounding_box;
    bool collided;
    int type;
private:
    VAO *object;
};

#endif 
