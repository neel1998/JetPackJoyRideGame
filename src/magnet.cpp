#include "magnet.h"
#include "main.h"

Magnet::Magnet(float x, float y, color_t color, int type) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;

    this->bounding_box.x = this->position.x;
    this->bounding_box.y = this->position.y;
    this->bounding_box.height = 1.0f;
    this->bounding_box.width = 1.0f;
    this->type = type;
    // speed = 0.3;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    if (type == 2){
        this->rotation = 180;
    }
    
    static const GLfloat vertex_buffer_data[] = {
        -1.0f,-1.0f,0.0f, 
         1.0f,-1.0f,0.0f,
        -1.0f, -0.5f,0.0f, 

        -1.0f, -0.5f, 0.0f, 
         1.0f, -0.5f,0.0f,
         1.0f, -1.0f,0.0f, 

         -1.0f,-1.0f + 1,0.0f, 
         1.0f,-1.0f + 1,0.0f,
        -1.0f, -0.5f + 1,0.0f, 

        -1.0f, -0.5f + 1, 0.0f, 
         1.0f, -0.5f + 1,0.0f,
         1.0f, -1.0f + 1,0.0f, 
                 

         -1.0f,-1.0f,0.0f, 
         -0.5f,-1.0f,0.0f,
         -1.0f, 0.5f,0.0f,

        -1.0f, 0.5f, 0.0f, 
         -0.5f,0.5f,0.0f,
         -0.5f, -1.0f,0.0f,     
    };
    this->object = create3DObject(GL_TRIANGLES, 18, vertex_buffer_data, color, GL_FILL);
    
}

void Magnet::draw(glm::mat4 VP) {
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

void Magnet::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Magnet::tick(int dir) {
    this->bounding_box.x = this->position.x -= (dir*0.2);
}
