#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

class Vertex {
	GLfloat _x, _y, _z;
	
public:
	Vertex();
	Vertex(GLfloat radius, GLfloat theta, GLfloat phi);
	void sphereVertex(GLfloat radius, GLfloat theta, GLfloat phi);
	GLfloat getX() const {return _x;}
	GLfloat getY() const {return _y;}
	GLfloat getZ() const {return _z;}

	friend Vertex operator+(Vertex const& v1, Vertex const& v2);
};

Vertex::Vertex()
{
    _x = 0;
    _y = 0;
    _z = 0;
}

Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z)
{
    _x = x;
    _y = y;
    _z = z;
}

void Vertex::sphereVertex(GLfloat radius, GLfloat theta, GLfloat phi) //Sets vertex location to a point on a sphere
{
	_x = (radius * cos(phi)) * cos(theta);
	_y = (radius * cos(phi)) * sin(theta);
	_z = radius * sin(phi);
}

// void Vertex::sphereVertex(GLfloat radius, GLfloat theta, GLfloat phi)
// {
//     _x = (radius * cos(phi)) * cos(theta);
//     _y = (radius * cos(phi)) * sin(theta);
//     _z = radius * sin(phi);
// }

Vertex operator+(Vertex const& v1, Vertex const& v2)
{
	GLfloat xTotal = v1.getX() + v2.getX();
	GLfloat yTotal = v1.getY() + v2.getY();
	GLfloat zTotal = v1.getZ() + v2.getZ();
	return Vertex(xTotal, yTotal, zTotal);
}
