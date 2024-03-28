//g++ SolarSystem.cpp -o main -lglfw -lGLU -lGL -lglut

// This program is a flyby around the RGB color cube.  One intersting note
// is that because the cube is a convex polyhedron and it is the only thing
// in the scene, we can render it using backface culling only. i.e., there
// is no need for a depth buffer.

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

#include "Planet.h"

// GLUT escape key id for readability
#define KEY_ESCAPE 27

//Planets (declared as global due to init function)
Planet sun(2, 56, 24);
Planet earth(1, 28, 12);
Planet moon(0.5, 21, 9);

/* = TODO =
* Add keyboard control (r) to rotate the whole image.
* With keyboard, add zoom in/zoom out (keys +: Zoom in, -: Zoom out).
* Add more cubes with different colors, illumination, and brightness.
* Add two vertical planes, one on the left and one on the right, and make the cubes bounce.
*/

// The cube has opposite corners at (0,0,0) and (1,1,1), which are black and
// white respectively.  The x-axis is the red gradient, the y-axis is the
// green gradient, and the z-axis is the blue gradient.  The cube's position
// and colors are fixed.
namespace Cube {

  const int NUM_VERTICES = 8;
  const int NUM_FACES = 6;

  GLint vertices[NUM_VERTICES][3] = {
    {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
    {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};

  GLint faces[NUM_FACES][4] = {
    {1, 5, 7, 3}, {5, 4, 6, 7}, {4, 0, 2, 6},
    {3, 7, 6, 2}, {0, 1, 3, 2}, {0, 4, 5, 1}};

  GLfloat vertexColors[NUM_VERTICES][3] = {
    {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
    {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0}};

  void draw() {
    glBegin(GL_QUADS);
    for (int i = 0; i < NUM_FACES; i++) {
      for (int j = 0; j < 4; j++) {
        glColor3fv((GLfloat*)&vertexColors[faces[i][j]]);
        glVertex3iv((GLint*)&vertices[faces[i][j]]);
      }
    }
    glEnd();
  }
}


//Player-controlled variables

//Figure movement
GLboolean stopFigure = false;

//Rotation control
GLfloat pitch = 0.0f,
        yaw =   0.0f,
        roll =  0.0f;

//Offset control
GLfloat figureX = 0.0f,
        figureY = 0.0f,
        figureZ = 0.0f;

//Zoom control
GLfloat scale = 1.0f;

// Display and Animation. To draw we just clear the window and draw the cube.
// Because our main window is double buffered we have to swap the buffers to
// make the drawing visible. Animation is achieved by successively moving our
// camera and drawing. The function nextAnimationFrame() moves the camera to
// the next point and draws. The way that we get animation in OpenGL is to
// register nextFrame as the idle function; this is done in main().
GLfloat counter = 0.0f;

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  
  
  earth.orbitStep();
  moon.orbitStep();
  
  counter += 0.10f;
  //sun.setOffset(5.0f*sin(counter),0.0f,0.0f);
  //sun.setOffset(0.0f,5.0f*sin(counter),0.0f);
  //sun.setOffset(0.0f,0.0f,5.0f*sin(counter));
  


  //earth.setOffset(-5.0f, 0.0f, 0.0f);

  sun.draw();
  earth.draw();
  moon.draw();
  //Cube::draw();
  glFlush();
  glutSwapBuffers();
}

// We'll be flying around the cube by moving the camera along the orbit of the
// curve u->(8*cos(u), 7*cos(u)-1, 4*cos(u/3)+2).  We keep the camera looking
// at the center of the cube (0.5, 0.5, 0.5) and vary the up vector to achieve
// a weird tumbling effect.
void timer(int v) {
  static GLfloat u = 0.0;
  // if(!stopFigure)
  //   u += 0.01;
  glLoadIdentity();
  //gluLookAt(8*cos(u), 7*cos(u)-1, 4*cos(u/3)+2, .5, .5, .5, cos(u), 1, 0);
  gluLookAt(0, 14, 14, 0, 0, 0, 0, -1, 0);
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
  gluPerspective(60.0, GLfloat(w) / GLfloat(h), 0.5, 40.0);
  glMatrixMode(GL_MODELVIEW);
}

// Application specific initialization:  The only thing we really need to do
// is enable back face culling because the only thing in the scene is a cube
// which is a convex polyhedron.
void init() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  //Planet data
  sun.setSecondaryColor(1.0f, 0.55f, 0.0f, 1.0f);
  sun.setPrimaryColor(1.0f, 0.78f, 0.0f, 1.0f);

  earth.setParent(&sun);  //Earth orbits around sun
  earth.setOrbit(8, 0.01f);
  earth.setPrimaryColor(0.17f, 1.0f, 1.0f, 1.0f);
  earth.setSecondaryColor(0.0f, 0.15f, 1.0f, 1.0f);

  moon.setParent(&earth); //Moon orbits around earth
  moon.setOrbit(3, 0.04f);
  moon.setPrimaryColor(0.5f, 0.5f, 0.5f, 1.0f);
  moon.setSecondaryColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void key_callback(unsigned char key, int dummy1, int dummy2)
{
  switch(key)
  {
    case (KEY_ESCAPE): //Quit
      exit(0);
    case ('s'):
      stopFigure = true;
      break;
    case ('c'):
      stopFigure = false;
      break;
    case ('u'): //Move up
      if (stopFigure)
        figureY += 0.1f;
      break;
    case ('d'): //Move down
      if (stopFigure)
        figureY -= 0.1f;
      break;
  }
}

// The usual main for a GLUT application.
int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutCreateWindow("The RGB Color Cube");
  glutReshapeFunc(reshape);
  glutTimerFunc(100, timer, 0);
  glutDisplayFunc(display);
  glutKeyboardFunc(key_callback); // Register the keyboard callback function
  init();
  glutMainLoop();
}