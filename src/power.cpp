#include "coin.h"
#include "power.h"

Power::Power(float x, float y, color_t color, int type) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;

    this->bounding_box.x = this->position.x;
    this->bounding_box.y = this->position.y;
    this->bounding_box.height = 3.0f;
    this->bounding_box.width = 3.0f;
    this->type = type;
    // speed = 0.3;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    int total;
    if (type == 1){
       GLfloat vertex_buffer_data[] = {
            -1.0f,-1.0f,0.0f, // triangle 1 : begin
             1.0f,-1.0f,0.0f,
             0.0f, 2.0f,0.0f, // triangle 1 : end
            

             1.0f, 1.0f, 0.0f, // triangle 2 : begin
            -1.0f, 1.0f,0.0f,
             0.0f, -2.0f,0.0f, // triangle 2 : end
            
        };
        this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
    }
    else{
        total = 30;
        float x = 1.0f;
        float y = 1.0f;
        GLfloat vertex_buffer_data[] = {
            -0.5f,-0.5f,0.0f, // triangle 1 : begin
             0.5f,-0.5f,0.0f,
            -0.5f, 0.5f,0.0f, // triangle 1 : end
            -0.5f, 0.5f, 0.0f, // triangle 2 : begin
             0.5f,0.5f,0.0f,
             0.5f, -0.5f,0.0f, // triangle 2 : end
            
            -0.5f,-0.5f + y,0.0f, // triangle 1 : begin
             0.5f,-0.5f + y,0.0f,
            -0.5f, 0.5f + y,0.0f, // triangle 1 : end
            -0.5f, 0.5f + y, 0.0f, // triangle 2 : begin
             0.5f,0.5f + y,0.0f,
             0.5f, -0.5f + y,0.0f,

             -0.5f,-0.5f -y,0.0f, // triangle 1 : begin
             0.5f,-0.5f - y,0.0f,
            -0.5f, 0.5f - y,0.0f, // triangle 1 : end
            -0.5f, 0.5f - y, 0.0f, // triangle 2 : begin
             0.5f,0.5f - y,0.0f,
             0.5f, -0.5f - y,0.0f,

             -0.5f -x ,-0.5f ,0.0f, // triangle 1 : begin
             0.5f -x,-0.5f ,0.0f,
            -0.5f -x, 0.5f ,0.0f, // triangle 1 : end
            -0.5f -x, 0.5f , 0.0f, // triangle 2 : begin
             0.5f -x,0.5f ,0.0f,
             0.5f -x, -0.5f ,0.0f,

             -0.5f + x,-0.5f,0.0f, // triangle 1 : begin
             0.5f + x,-0.5f,0.0f,
            -0.5f + x, 0.5f,0.0f, // triangle 1 : end
            -0.5f + x, 0.5f, 0.0f, // triangle 2 : begin
             0.5f + x,0.5f ,0.0f,
             0.5f + x, -0.5f,0.0f,
        };
        this->object = create3DObject(GL_TRIANGLES, total, vertex_buffer_data, color, GL_FILL);
    }
}

void Power::draw(glm::mat4 VP) {
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

void Power::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Power::tick() {
    this->bounding_box.x = this->position.x -= 0.05;
    this->bounding_box.y = this->position.y = 3.0f*sin(this->position.x);
}

