#include "main.h"
#include "timer.h"
#include "ball.h"
#include "ground.h"
#include "prop.h"
#include "coin.h"
#include "wall.h"
#include "balloon.h"
#include <cstdlib>
#include "fire.h"
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/
Fire fire[20];
Ball ball1;
Ground ground;
Wall wall;
Coin coins[100];
Balloon balloon;
Prop prop[10];
glm::mat4 VP;
int shoot = 1;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    glm::vec3 eye ( 0, 0, 1 );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ball1.draw(VP);
    ground.draw(VP);
    wall.draw(VP);
    for (int i = 0; i < 100; i++) {
        if (!coins[i].collided){
    	   coins[i].draw(VP);
        }
    }
    // if (ball1.position.y != -9){
    // 	for (int i = 0; i < 5; i++){
    // 		prop[i].draw(VP);
    // 	}
    // }
   	balloon.draw(VP);
   	for (int i = 0; i < 20 ; i++){
   		if (!fire[i].collided){
   			fire[i].draw(VP);
   		}
   	}
}

void tick_input(GLFWwindow *window) {

    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    if (left) {
        // Do something
        if (ball1.position.x > 0 && ball1.position.x < 20){
       		for (int i = 0; i < 100; i++) {
        		if (detect_collision(ball1.bounding_box, coins[i].bounding_box)) {
            		coins[i].collided = true;
        		}
    			coins[i].tick(-1);
    		}
    		for (int i = 0; i < 20; i++){
    			fire[i].tick(-1);
    		}
    	}
    	
        ball1.moveL();
    }
    if (right){
        if (ball1.position.x > 0 && ball1.position.x < 20){
    		for (int i = 0; i < 100; i++) {
        		if (detect_collision(ball1.bounding_box, coins[i].bounding_box)) {
            		coins[i].collided = true;
        		}
    			coins[i].tick(1);
    		}
    		for (int i = 0; i < 20; i++){
    			fire[i].tick(1);
    		}
    	}
    	if (ball1.position.x > 20){
    		ball1.set_position(0.0f, ball1.position.y);
    		for (int i = 0; i < 100; i++) {
        		if (detect_collision(ball1.bounding_box, coins[i].bounding_box)) {
            		coins[i].collided = true;
        		}
    			coins[i].tick(100);
    		}
    		for (int i = 0; i < 20; i++){
    			fire[i].tick(100);
    		}	
    	}
    	ball1.moveR();
    }
    if (up){
    	ball1.jump();
    	// propulsion();
    }
    if(space && shoot){
    	balloon = Balloon(ball1.position.x, ball1.position.y, COLOR_BLACK);
    	shoot = 0;
    }

}
void propulsion() {
	for (int i = 0; i < 5; i ++){
		float dir;
		if (i%2) {
			dir = -1;
		}
		else{
			dir = 1;
		}
		prop[i] = Prop( ball1.position.x + dir, ball1.position.y - i - 1 , COLOR_RED );
	}
}
void tick_elements() {
    
    // printf("ball 1 x = %f ball 2 x=%f\n", ball1.position.x, ball2.position.x);
    ball1.tick();
    ground.tick();
    wall.tick();
    for (int i = 0; i < 100; i++) {
    	if (detect_collision(ball1.bounding_box, coins[i].bounding_box)) {
       		coins[i].collided = true;
    	}
    }
    if (balloon.position.y > 10 || balloon.position.y < -10 || balloon.position.x > 20){
    	balloon.position.x = 1000;
    	balloon.position.y = 1000;
    	shoot = 1;
    }
    else {
    	for (int i = 0; i < 20; i ++) {
    		// int i = 0;
    		float dist1 = sqrt( pow(balloon.position.x - fire[i].position.x - 3*cos(fire[i].rotation * M_PI / 180.0f),2) + pow(balloon.position.y - fire[i].position.y - 3*sin(fire[i].rotation * M_PI / 180.0f) ,2) ); 
    		float dist2 = sqrt( pow(balloon.position.x - fire[i].position.x + 3*cos(fire[i].rotation * M_PI / 180.0f),2) + pow(balloon.position.y - fire[i].position.y + 3*sin(fire[i].rotation * M_PI / 180.0f) ,2) );
    		// printf("%d : %.2f\n",i,dist1 + dist2 );
    		if (abs(dist1 + dist2 - 6.0f) <= 0.05f){
    			// printf("collided\n");
    			fire[i].collided = true;
    			balloon.position.x = 1000;
    			balloon.position.y = 1000;
    			shoot = 1;			
    		}
    		
    	}
    }
    for (int i = 0; i < 20; i ++) {
   		float dist1 = sqrt( pow(ball1.position.x - fire[i].position.x - 3*cos(fire[i].rotation * M_PI / 180.0f),2) + pow(ball1.position.y - fire[i].position.y - 3*sin(fire[i].rotation * M_PI / 180.0f) ,2) ); 
    	float dist2 = sqrt( pow(ball1.position.x - fire[i].position.x + 3*cos(fire[i].rotation * M_PI / 180.0f),2) + pow(ball1.position.y - fire[i].position.y + 3*sin(fire[i].rotation * M_PI / 180.0f) ,2) );
    	if ( abs(dist1 + dist2 - 6.0f) <= 0.05f) {
    		ball1.position.x = -10.0f;
    	}
    }
	balloon.tick();
    // for (int i = 0; i < 5; i++){
    // 	prop[i].tick();
    // }
    // propulsion();
    // if (detect_collision())
    camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball1  = Ball(-15, -9, COLOR_GREEN);
    ground = Ground(-20, -15, COLOR_BLACK);
    wall = Wall(-20, 15,COLOR_BLACK);
    balloon = Balloon(-100, -100, COLOR_BLACK);
    for (int i = 0; i < 100; i++) {
        coins[i] = Coin(1.0*(rand()%1000 - 20), 1.0*(rand()%10), COLOR_COIN);
    }
    for (int i = 0; i < 20; i++){
    	fire[i] = Fire(rand()%1000 - 20, rand()%10 - 5, 1.0f*(rand()%90) - 45.0f, COLOR_COIN);	
    }
    fire[0] = Fire(0.0f, 0.0f, 45.0f, COLOR_COIN);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // propulsion();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }
        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 20 / screen_zoom;
    float bottom = screen_center_y - 20 / screen_zoom;
    float left   = screen_center_x - 20 / screen_zoom;
    float right  = screen_center_x + 20 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
