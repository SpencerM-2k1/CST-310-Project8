#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

class SphereVertex {
	GLfloat _x, _y, _z;
	
public:
	SphereVertex(GLfloat radius, GLfloat theta, GLfloat phi);
	void anglesToXYZ(GLfloat radius, GLfloat theta, GLfloat phi);
	GLfloat getX() {return _x;}
	GLfloat getY() {return _y;}
	GLfloat getZ() {return _z;}
};

SphereVertex::SphereVertex(GLfloat radius, GLfloat theta, GLfloat phi)
{
    _x = (radius * cos(phi)) * cos(theta);
    _y = (radius * cos(phi)) * sin(theta);
    _z = radius * sin(phi);
}

// void SphereVertex::anglesToXYZ(GLfloat radius, GLfloat theta, GLfloat phi)
// {
//     _x = (radius * cos(phi)) * cos(theta);
//     _y = (radius * cos(phi)) * sin(theta);
//     _z = radius * sin(phi);
// }