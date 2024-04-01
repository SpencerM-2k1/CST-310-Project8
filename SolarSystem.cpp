//g++ SolarSystem.cpp -o main -lglfw -lGLU -lGL -lglut

// Displays a model of the solar system with 8 planets, several moons, and varying orbit sizes & speeds

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "Planet.h"

// GLUT escape key id for readability
#define KEY_ESCAPE 27

//Planets (declared as global due to init function)
Planet sun(2, 56, 24);

Planet mercury(0.2, 28, 12);

Planet venus(0.45, 25, 15);

Planet earth(0.45, 28, 12);
Planet moon(0.15, 21, 9);

Planet mars(0.45, 15, 20);
Planet phobos(0.15, 21, 9);
Planet deimos(0.15, 21, 9);


Planet jupiter(1.1, 15, 25); 

Planet saturn(0.75, 30, 35); 

Planet uranus(0.65, 25, 30);  

Planet neptune(0.60, 20, 30); 


//Player-controlled variables

//Zoom control
GLint distance = 28;

const int MAX_STARS = 5000;
float starPositions[MAX_STARS][3];

// Function to generate star positions
void generateStarPositions(float starPositions[][3], int numStars, float sceneSize) {
    srand(time(nullptr));

    for (int i = 0; i < numStars; ++i) {
        starPositions[i][0] = (static_cast<float>(rand()) / RAND_MAX) * 2 * sceneSize - sceneSize;
        starPositions[i][1] = (static_cast<float>(rand()) / RAND_MAX) * 2 * sceneSize - sceneSize;
        starPositions[i][2] = (static_cast<float>(rand()) / RAND_MAX) * 2 * sceneSize - sceneSize;
    }
}


// Function to draw stars using pre-generated positions
void drawStars() {
    glPointSize(1.0f); // Set point size to draw stars

    // Set color to white for stars
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_POINTS);
    for (int i = 0; i < MAX_STARS; ++i) {
        glVertex3f(starPositions[i][0], starPositions[i][1], starPositions[i][2]);
    }
    glEnd();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glLoadIdentity();
  gluLookAt(0, distance, distance, 0, 0, 0, 0, -1, 0);

  mercury.orbitStep();
  venus.orbitStep();
  
  earth.orbitStep();
  moon.orbitStep();

  mars.orbitStep();
  phobos.orbitStep();
  deimos.orbitStep();

  jupiter.orbitStep();
  saturn.orbitStep();
  uranus.orbitStep();
  neptune.orbitStep();
  
  sun.draw();

  mercury.draw();
  venus.draw();

  earth.draw();
  moon.draw();

  mars.draw();
  phobos.draw();
  deimos.draw();

  jupiter.draw();
  saturn.draw();
  uranus.draw();
  neptune.draw();

  drawStars(); 

  glFlush();
  glutSwapBuffers();
}

// Timer function-- trigger a redisplay
void timer(int v) {
  glutPostRedisplay();
  glutTimerFunc(1000/60.0, timer, v);
}

// When the window is reshaped we have to recompute the camera settings to
// match the new window shape.  Set the viewport to (0,0)-(w,h).  Set the
// camera to have a 60 degree vertical field of view, aspect ratio w/h, near
// clipping plane distance 0.5 and far clipping plane distance 40.
void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, GLfloat(w) / GLfloat(h), 0.5, 120.0);
  glMatrixMode(GL_MODELVIEW);
}

// Initialization: Depth testing, planet attributes, star positions
void init() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0f, 0.0f, 0.0f, 0.1f);

  //Planet data
  sun.setSecondaryColor(1.0f, 0.55f, 0.0f, 1.0f);
  sun.setPrimaryColor(1.0f, 0.78f, 0.0f, 1.0f);

  mercury.setParent(&sun);
  mercury.setOrbit(3, 0.03f);
  mercury.setPrimaryColor(0.5f, 0.0f, 0.0f, 0.0f);
  mercury.setSecondaryColor(0.0f, 0.0f, 0.0f, 0.0f);

  venus.setParent(&sun);
  venus.setOrbit(6, 0.015f); 
  venus.setPrimaryColor(0.8f, 0.2f, 0.0f, 1.0f); 
  venus.setSecondaryColor(0.1f, 0.1f, 0.1f, 1.0f);

  earth.setParent(&sun);  //Earth orbits around sun
  earth.setOrbit(8, 0.01f);
  earth.setPrimaryColor(0.17f, 1.0f, 1.0f, 1.0f);
  earth.setSecondaryColor(0.0f, 0.15f, 1.0f, 1.0f);

  moon.setParent(&earth); //Moon orbits around earth
  moon.setOrbit(1, 0.04f);
  moon.setPrimaryColor(0.5f, 0.5f, 0.5f, 1.0f);
  moon.setSecondaryColor(0.0f, 0.0f, 0.0f, 1.0f);

  mars.setParent(&sun);
  mars.setOrbit(11, 0.007f);
  mars.setPrimaryColor(1.0f, 0.0f, 0.0f, 0.0f);
  mars.setSecondaryColor(0.0f, 0.0f, 0.0f, 0.0f);

  phobos.setParent(&mars); //Moon orbits around earth
  phobos.setOrbit(1, 0.04f);
  phobos.setPrimaryColor(0.5f, 0.5f, 0.5f, 1.0f);
  phobos.setSecondaryColor(0.0f, 0.0f, 0.0f, 1.0f);

  deimos.setParent(&mars); //Moon orbits around earth
  deimos.setOrbit(1.2, 0.05f);
  deimos.setPrimaryColor(0.5f, 0.5f, 0.5f, 1.0f);
  deimos.setSecondaryColor(0.0f, 0.0f, 0.0f, 1.0f);

  jupiter.setParent(&sun);
  jupiter.setOrbit(15, 0.005f); 
  jupiter.setPrimaryColor(0.9f, 0.6f, 0.3f, 1.0f); 
  jupiter.setSecondaryColor(0.1f, 0.1f, 0.1f, 1.0f); 

  saturn.setParent(&sun);
  saturn.setOrbit(18, 0.009f); 
  saturn.setPrimaryColor(0.8f, 0.7f, 0.2f, 1.0f); 
  saturn.setSecondaryColor(0.1f, 0.1f, 0.1f, 1.0f); 

  uranus.setParent(&sun);
  uranus.setOrbit(21, 0.0015f); 
  uranus.setPrimaryColor(0.5f, 0.8f, 0.9f, 1.0f); 
  uranus.setSecondaryColor(0.1f, 0.1f, 0.1f, 1.0f); 

  neptune.setParent(&sun);
  neptune.setOrbit(24, 0.001f); 
  neptune.setPrimaryColor(0.2f, 0.4f, 0.9f, 1.0f); 
  neptune.setSecondaryColor(0.1f, 0.1f, 0.1f, 1.0f); 

  generateStarPositions(starPositions, MAX_STARS, 50.0f);

}

void key_callback(unsigned char key, int dummy1, int dummy2)
{
  switch(key)
  {
    case (KEY_ESCAPE): //Quit
      exit(0);
    case ('+'):
    case ('='): //Zoom in works without shift/caps lock
    distance--;
    if (distance < 4)
      distance = 4;
      break;
    case ('-'):
    case ('_'): //Zoom out works with shift/caps lock
    distance++;
    if (distance > 35)
      distance = 35;
      break;
  }
}

// The usual main for a GLUT application.
int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(750, 750);
  glutCreateWindow("The Solar System");
  glutReshapeFunc(reshape);
  glutTimerFunc(100, timer, 0);
  glutDisplayFunc(display);
  glutKeyboardFunc(key_callback); // Register the keyboard callback function
  init();
  glutMainLoop();
}
