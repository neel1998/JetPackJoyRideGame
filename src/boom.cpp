#include "boom.h"
#include "main.h"

Boom::Boom(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;

    this->bounding_box.x = this->position.x;
    this->bounding_box.y = this->position.y;
    this->bounding_box.height = 2.0f;
    this->bounding_box.width = 2.0f;
    // speed = 0.3;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    
    GLfloat vertex_buffer_data[] = {
        -1.0f,-1.0f,0.0f,
         1.0f,-1.0f,0.0f,
         0.0f, 1.0f,0.0f, 
    }; 

    
    this->object = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, color, GL_FILL);
}

void Boom::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    

    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    // glm::mat4 MVP = Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);

}

void Boom::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Boom::tick() {
    this->rotation += 20;
    this->bounding_box.y = this->position.y -= 0.2;
    this->bounding_box.x = this->position.x = pow(this->position.y,2)/8;
}

void Boom::move() {
    this->bounding_box.y = this->position.y -= 0.2;
    this->bounding_box.x = this->position.x = pow(this->position.y,2)/10;
   
}