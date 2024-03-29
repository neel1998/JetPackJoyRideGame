#include "prop.h"
#include "main.h"

Prop::Prop(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->Ptime = 0;
    // speed = 0.3;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    int n = 100;
    float r = 0.2f;
    float phi = 0;
    float theta = (2*3.14)/n;
    GLfloat vertex_buffer_data[n*9]; 

    for (int i = 0; i < 9*n; i+=9){
        vertex_buffer_data[i] = 0.0f;
        vertex_buffer_data[i + 1] = 0.0f;
        vertex_buffer_data[i + 2] = 0.0f;

        vertex_buffer_data[i + 3] = r*cos(phi);
        vertex_buffer_data[i + 4] = r*sin(phi);
        vertex_buffer_data[i + 5] = 0.0f;

        vertex_buffer_data[i + 6] = r*cos(phi + theta);
        vertex_buffer_data[i + 7] = r*sin(phi + theta);
        vertex_buffer_data[i + 8] = 0.0f;

        phi += theta;            
    }
    this->object = create3DObject(GL_TRIANGLES, n*3, vertex_buffer_data, color, GL_FILL);
}

void Prop::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    

    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    // glm::mat4 MVP = Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Prop::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Prop::tick() {
    this->position.y -= 0.5f;
    this->Ptime ++;
    if (this->Ptime >= 5){
        this->position.x = -10000;
    }
}


