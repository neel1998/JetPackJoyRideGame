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
#include "boom.h"
#include "power.h"
#include "score.h"
#include "magnet.h"
#include "ring.h"
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/
int total_fire = 30;
vector < Coin > health;
vector < Prop > prop;
vector < Ring > rings;
int ringNo = 0;
Fire fire[30];
Ball ball1;
Boom boom;
Power power;
Ground ground;
Wall wall;
Coin coins[100];
Balloon balloon;
Magnet magnet;
glm::mat4 VP;
int shoot = 1;
int isBoom = 0;
int isPower2 = 0;
int isMag = 0;
int magCount = 0;
Ring ring;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
vector < Score > score;
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
    if (!boom.collided && ball1.stage == 2){
    	boom.draw(VP);
    }
    if (!power.collided){
    	power.draw(VP);
    }
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
   	for (int i = 0; i < total_fire ; i++){
   		if (!fire[i].collided){
   			fire[i].draw(VP);
   		}
   	}
   	for (int i = 0; i < prop.size(); i++){
   		prop[i].draw(VP);
   	}
   	for (int i = 0; i < score.size(); i ++){
   		score[i].draw(VP);
   	}
    for (int i = 0; i < rings.size(); i++){
        rings[i].draw(VP);
    }
   	magnet.draw(VP);
    for (int i = 0; i < health.size(); i++) {
        health[i].draw(VP);
    }
    // ring.draw(VP);
}

void tick_input(GLFWwindow *window) {

    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int q = glfwGetKey(window, GLFW_KEY_Q);
    if (q){
        printf("=========== GAME OVER =========\n");
        printf("COINS = %d\n",ball1.coins);
        quit(window);
    }
    if (left) {
        // Do something
        if (ball1.position.x > 0){
       		for (int i = 0; i < 100; i++) {
        		// if (detect_collision(ball1.bounding_box, coins[i].bounding_box)) {
          //   		coins[i].collided = true;
          //   		ball1.coins ++;
        		// }
    			coins[i].tick(-1);
    		}
    		for (int i = 0; i < total_fire; i++){
    			fire[i].tick(-1);
    		}
            for (int i = 0; i < rings.size(); i++){
                rings[i].tick(-1);
            }
            // ring.tick(-1);
            ball1.moveL();
            if (ball1.onRing){
                ball1.position.y = rings[ringNo].position.y + sqrt( abs(25 - pow( (ball1.position.x - rings[ringNo].position.x),2)) );
            }
            // else{
            // }
            magnet.tick(-1);
    	}
    	else if(ball1.position.x < -18.0f ) {
    		for (int i = 0; i < 100; i++) {
        		// if (detect_collision(ball1.bounding_box, coins[i].bounding_box)) {
          //   		coins[i].collided = true;
          //   		ball1.coins ++;
        		// }
    			coins[i].tick(-1);
    		}
    		for (int i = 0; i < total_fire; i++){
    			fire[i].tick(-1);
    		}
            magnet.tick(-1);
            for (int i = 0; i < rings.size(); i++){
                rings[i].tick(-1);
            }
    	}
    	else {
        	ball1.moveL();
            if (ball1.onRing){
                ball1.position.y = rings[ringNo].position.y + sqrt( abs(25 - pow( (ball1.position.x - rings[ringNo].position.x),2) ));
            }
            // else{
            // }
    	}
    	
    }
    if (right){
        if (ball1.position.x > 0 && ball1.position.x < 20){
    		for (int i = 0; i < 100; i++) {
        		// if (detect_collision(ball1.bounding_box, coins[i].bounding_box)) {
          //   		coins[i].collided = true;
          //   		ball1.coins ++;
        		// }
    			coins[i].tick(1);
    		}
    		for (int i = 0; i < total_fire; i++){
    			fire[i].tick(1);
    		}
            magnet.tick(1);
            for (int i = 0; i < rings.size(); i++){
                rings[i].tick(1);
            }
    	}
    	if (ball1.position.x > 20){
    		ball1.set_position(0.0f, ball1.position.y);
    		for (int i = 0; i < 100; i++) {
        		// if (detect_collision(ball1.bounding_box, coins[i].bounding_box)) {
          //   		coins[i].collided = true;
          //   		ball1.coins ++;
        		// }
    			coins[i].tick(100);
    		}
    		for (int i = 0; i < total_fire; i++){
    			fire[i].tick(100);
    		}	
            magnet.tick(100);
            for (int i = 0; i < rings.size(); i++){
                rings[i].tick(100);
            }
        }
    	ball1.moveR();
        if (ball1.onRing){
            ball1.position.y = rings[ringNo].position.y + sqrt( abs(25 - pow( (ball1.position.x - rings[ringNo].position.x),2)) );
        }
        // else{
        // }
    }
    if (up){
    	
    	float x = ball1.position.x + (rand()%2 - 1)*1.0f;
    	float y = ball1.position.y  - (rand()%2 + 1.0f);
    	prop.push_back(Prop(x, y, COLOR_RED));

    	x = ball1.position.x + (rand()%2 - 1)*1.0f;
    	y = ball1.position.y  - (rand()%2 + 1.0f);
    	prop.push_back(Prop(x, y, COLOR_RED));
        if (!ball1.onRing){
    	   ball1.jump();
        }
    	// propulsion();
    }
    if(space && shoot){
    	balloon = Balloon(ball1.position.x, ball1.position.y, COLOR_BALLOON);
    	shoot = 0;
    }

}

void tick_elements() {
    drawHealth(ball1.health);
    for (int i = 0; i < rings.size(); i++){
        if (ball1.position.x >= rings[i].position.x - 5.0f && ball1.position.x <= rings[i].position.x + 5.0f && ball1.position.y >= rings[i].position.y && ball1.position.y <= rings[i].position.y + 5.0f) {
            if ( abs(pow((ball1.position.y - rings[i].position.y),2) + pow( (ball1.position.x - rings[i].position.x),2) - 25 ) <= 1.0f ) {
                ball1.gravity = 0;
                ball1.onRing = true;
                ringNo = i;
                break;
            }
            else{
                ball1.gravity = 1;
                ball1.onRing = false;
            }
        }
        else{
            ball1.gravity = 1;
            ball1.onRing = false;
        }
    }

    if (isMag) {
        if (magnet.type == 1){
            if (abs(ball1.position.x - magnet.position.x) <= 2.0f && abs(ball1.position.y - magnet.position.y) <= 2.0f){
                ball1.position.x = magnet.position.x + 2.0f;
                ball1.position.y = 0;
                ball1.gravity = 0;
            }
            else{
                if (ball1.position.x > magnet.position.x + 1.0f){
                    ball1.gravity = 0;
                    // float angle = (ball1.position.y - magnet.position.y)/(ball1.position.x - magnet.position.x);
                    ball1.position.x -= 0.1f;
                    // ball1.position.y =
                    if (ball1.position.y > 0){
                        ball1.position.y -= 0.1f;
                    }
                    else{
                        ball1.position.y += 0.1f;
                    }
                }
                else{
                    ball1.gravity = 1;
                }
            }
        }
        else{
            if (abs(magnet.position.x - ball1.position.x ) <= 2.0f && abs(ball1.position.y - magnet.position.y) <= 2.0f){
                ball1.position.x = magnet.position.x - 2.0f;
                ball1.position.y = 0;
                ball1.gravity = 0;
            }
            else{
                if (ball1.position.x < magnet.position.x - 1.0f ){
                    ball1.gravity = 0;
                    // float angle = (ball1.position.y - magnet.position.y)/(ball1.position.x - magnet.position.x);
                    ball1.position.x += 0.1f;
                    // ball1.position.y =
                    if (ball1.position.y > 0){
                        ball1.position.y -= 0.1f;
                    }
                    else{
                        ball1.position.y += 0.1f;
                    }
                }
                else if (ball1.position.x == magnet.position.x + 1.0f && ball1.position.y == 0){
                    ball1.position.x = magnet.position.x - 1.0f;
                    ball1.position.y = 0;
                }
                else{
                    ball1.gravity = 1;
                }
            }   
        }
    }
	int c = ball1.coins;
	writeScore(c%10, (c/10)%10, (c/100)%10);
	if (ball1.coins >= 40) {
		ball1.stage = 2;
        isMag = 0;
	}
    if (ball1.health < 0) {
    	
    	quit(window);
    }
	power.tick();
    for (int i = 0; i < 100; i++) {
    	if (detect_collision(ball1.bounding_box, coins[i].bounding_box) && !coins[i].collided) {
       		coins[i].collided = true;
       		ball1.coins += coins[i].type;
    	}
   	}
    // if (!ball1.onRing){
        ball1.tick();
    // }
    ground.tick();
    wall.tick();
   
    if (isBoom && boom.position.y > -15.0f){
    	boom.tick();
    }else{
    	isBoom = 0;
    }
    if (isPower2 && power.position.x > -20.f){
    	power.tick();
    }
    else{
    	isPower2 = 0;
    }
    if(isMag){
    	magCount ++;
        if (magCount >= 500){
            magCount = 0;
            isMag = 0;
            ball1.gravity = 1;
            magnet.position.x = -1000;
        }
    }
    if (rand()%(497) == 0 && isBoom == 0 && ball1.stage == 2){
    	boom = Boom(15, 20, COLOR_RED);
    	isBoom = 1;
    } 
    if (rand()%(497) == 0 && isPower2 == 0){
    	if (rand()%10 > 5){
    		power = Power(20, 0, COLOR_POWER2, 2);
    	}
    	else{
    		power = Power(20, 0, COLOR_POWER1, 1);
    	}
    	isPower2 = 1;
    }

    if (rand()%(497) == 0 && isMag == 0 && ball1.stage == 1){
    	if (rand()%10 > 5){
    		magnet = Magnet(0, 0, COLOR_BLACK, 2);
    	}
    	else{
    		magnet = Magnet(0, 0, COLOR_BLACK, 1);
    	}
    	isMag = 1;
    }


    if (detect_collision(ball1.bounding_box, boom.bounding_box) && isBoom == 1 && !ball1.onRing) {
    	isBoom = 0;
    	ball1.health --;
    	boom.collided = true;
    }
    if (detect_collision(ball1.bounding_box, power.bounding_box) && isPower2 == 1) {
    	isPower2 = 0;
    	power.collided = true;
    	if (power.type == 1){
    		ball1.coins += 10;
    	}
    	else{
    		ball1.health ++;
    	}
    }
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
    	for (int i = 0; i < total_fire; i ++) {
    		// int i = 0;
    		if (!fire[i].collided) {
    			float dist1 = sqrt( pow(balloon.position.x - fire[i].position.x - 3*cos(fire[i].rotation * M_PI / 180.0f),2) + pow(balloon.position.y - fire[i].position.y - 3*sin(fire[i].rotation * M_PI / 180.0f) ,2) ); 
    			float dist2 = sqrt( pow(balloon.position.x - fire[i].position.x + 3*cos(fire[i].rotation * M_PI / 180.0f),2) + pow(balloon.position.y - fire[i].position.y + 3*sin(fire[i].rotation * M_PI / 180.0f) ,2) );
	    		if (abs(dist1 + dist2 - 6.0f) <= 0.05f && !fire[i].collided){
    				fire[i].collided = true;
    				balloon.position.x = 1000;
    				balloon.position.y = 1000;
    				shoot = 1;
    						
    			}
	    	}
	    }
    }
    for (int i = 0; i < total_fire; i ++) {
    	if (!fire[i].collided) {
   			float dist1 = sqrt( pow(ball1.position.x - fire[i].position.x - 3*cos(fire[i].rotation * M_PI / 180.0f),2) + pow(ball1.position.y - fire[i].position.y - 3*sin(fire[i].rotation * M_PI / 180.0f) ,2) ); 
    		float dist2 = sqrt( pow(ball1.position.x - fire[i].position.x + 3*cos(fire[i].rotation * M_PI / 180.0f),2) + pow(ball1.position.y - fire[i].position.y + 3*sin(fire[i].rotation * M_PI / 180.0f) ,2) );
    		if ( abs(dist1 + dist2 - 6.0f) <= 0.05f && !ball1.onRing) {
    			ball1.position.x = -18.0f;
    			ball1.health --;	
    		}
    	}
    }
	balloon.tick();
	for (int i = 0; i < 20 ; i++){
		fire[i].move();
	}
   	for (int i =0 ;i < prop.size(); i++){
   		prop[i].tick();
   	}
    camera_rotation_angle += 1;
}
void drawHealth(int h){
    health.clear();
    for (int i = 0; i < h; i++){
        health.push_back(Coin(-1.5 + i*1.0, 12, COLOR_RED, 1));
    }
}
void writeScore(int unit, int dec, int hund ){
	score.clear();
	Score s;
	int numb[] = {hund, dec, unit};
	for (int i = 0; i < 3; i++){
		if (numb[i] != 5 && numb[i] != 6){
			s = Score(-2 +0 + (i+1)*1.5 , 1 + 15, COLOR_RED); // right top
	    	score.push_back(s);
		}
		if ( numb[i] != 2){
			Score s = Score(-2 +0 + (i+1)*1.5, 0 + 15, COLOR_RED); //right bottom
	    	score.push_back(s);
		}

		if ( numb[i] != 1 && numb[i] != 2 && numb[i] != 7  && numb[i] != 3){
			s = Score(-2 +-1 + (i+1)*1.5, 1 + 15, COLOR_RED); // left top
	    	score.push_back(s);
		}
	    if ( numb[i] == 2 || numb[i] == 8 || numb[i] == 6 || numb[i] == 0 ){
			s = Score(-2 +-1 + (i+1)*1.5, 0 + 15, COLOR_RED); // left bottom
	    	score.push_back(s);
		}
	    if ( numb[i] != 1 && numb[i] != 7 && numb[i] != 0){
			s = Score(-2 +-0.5 + (i+1)*1.5, 0.5f + 15, COLOR_RED); //middle
	    	s.rotation = 90;
	   	 	score.push_back(s);
		}
		if ( numb[i] != 1 && numb[i] != 4 ){
			s = Score(-2 +-0.5 + (i+1)*1.5, 1.5f + 15, COLOR_RED); // top
	    	s.rotation = 90;
	    	score.push_back(s);
		}
	    if ( numb[i] != 1 && numb[i] != 7  && numb[i] != 4){
			s = Score(-2 +-0.5 + (i+1)*1.5, -0.5f + 15, COLOR_RED); //bottom
	    	s.rotation = 90;
	    	score.push_back(s);
		}    
	    	        
	}
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
    boom = Boom(-1000, -1000, COLOR_RED);
    power = Power(-1000, -1000, COLOR_POWER2, 2);
    
    magnet = Magnet(-1000, -1000, COLOR_BLACK, 1);
    
    for (int i =0 ;i < 10; i++){
        rings.push_back(Ring(i*100, 0, COLOR_RED, 1));
        
    }

    for (int i = 0; i < 100; i++) {
    	if (rand()%10 < 7){
        	coins[i] = Coin(1.0*(rand()%1000 - 20), 1.0*(rand()%10), COLOR_COIN, 1);
        }
        else{
        	coins[i] = Coin(1.0*(rand()%1000 - 20), 1.0*(rand()%10), COLOR_COIN2, 2);	
        }
    }
    for (int i = 0; i < total_fire; i++){
    	fire[i] = Fire(rand()%1000 - 20, rand()%10 - 5, 1.0f*(rand()%90) - 45.0f, COLOR_FIRE, 0.0f, false);	
    }
    for (int i = 0; i < 10; i+=2){
    	fire[i] = Fire(i*80.0f, 0.0f, 0.0f, COLOR_FIRE, 0.1f, true);
    	fire[i + 1] = Fire(i*80.0f, -2.0f, 0.0f, COLOR_FIRE, 0.1f, false);
    }

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
        reset_screen();
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
