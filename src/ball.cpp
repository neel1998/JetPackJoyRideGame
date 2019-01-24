#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 0.3;
    this->bounding_box.x = this->position.x;
    this->bounding_box.y = this->position.y;
    this->bounding_box.height = 2.0f;
    this->bounding_box.width = 2.0f;
    this->coins = 0;
    this->health = 5;
    this->score = 0;
    this->stage = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -1.0f,-1.0f,0.0f, // triangle 1 : begin
         1.0f,-1.0f,0.0f,
        -1.0f, 1.0f,0.0f, // triangle 1 : end
        -1.0f, 1.0f, 0.0f, // triangle 2 : begin
         1.0f,1.0f,0.0f,
         1.0f, -1.0f,0.0f, // triangle 2 : end
        
    };

    this->object = create3DObject(GL_TRIANGLES, 18, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
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

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
	this->score ++;
    // this->rotation += speed;
    // this->position.x += speed/100;
    // this->position.y += speed/500;
    float temp = this->position.y - (speed+= 0.03);
    if (temp > -9){
    	this->bounding_box.y =  this->position.y = temp;
    }
    else{
    	this->bounding_box.y =  this->position.y = -9.0f;	
    }
}

void Ball::moveL() {
    // this->rotation += speed;
    this->bounding_box.x = this->position.x -= 0.3f;
    // this->position.y -= speed;
}

void Ball::moveR() {
    // this->rotation += speed;
    this->bounding_box.x = this->position.x += 0.3f;
    // this->position.y -= speed;
}

void Ball::moveD() {
    // this->rotation += speed;
    this->bounding_box.y = this->position.y -= 0.3f;
    // this->position.y -= speed;
}
void Ball::jump() {
	this->speed = 0.3f;
	if (this->position.y + 2 <= 10)
		this->bounding_box.y = this->position.y += 1;
}
