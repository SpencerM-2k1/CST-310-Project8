
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

#include <vector>

#include <iostream>

#include "Vertex.h"

//TODO: better color controls, will help differentiate "planets"
//TODO: Hierarchy system: allow planets to orbit other planets

class Planet {
	//Sphere display attributes
	GLfloat _radius;
	GLint _sectorCount;
	GLint _stackCount;

	//Orbit parent + relative offset (offset is relative to origin if parent is null)
	Planet* _parent = nullptr;
	GLfloat _orbitSpeed = 0.0f;
	GLfloat _orbitRadius = 0.0f;
	GLfloat _orbitProgress = 0.0f;
	Vertex _offset;

	std::vector<std::vector<Vertex>> _vertices;
	GLfloat _color1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat _color2[4] = {1.0f, 1.0f, 1.0f, 1.0f};

public:
	Planet();
	Planet(GLfloat radius, GLint numSectors, GLint numStacks);
	void draw();

	void usePrimaryColor();
	void useSecondaryColor();
	
	void setParent(Planet* orbitParent); //Sets planet to orbit around
	void setOrbit(GLfloat speed, GLfloat radius, GLfloat progress);	//Defines radius and speed of orbit
	void setOffset(GLfloat x, GLfloat y, GLfloat z);
	void orbitStep();
	void setSize(GLfloat radius);

	void setPrimaryColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void setSecondaryColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

	Vertex getAbsolutePos();
};

//Default Constructor
Planet::Planet() {}

//Constructor
Planet::Planet(GLfloat radius, GLint numSectors, GLint numStacks)
{
	_radius = radius;
	_sectorCount = numSectors;
	_stackCount = numStacks;
	//setOffset(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < _sectorCount + 1; i++) //_sectorCount + 1 required for final sector branching end and start of array
	{
		_vertices.push_back(std::vector<Vertex>());
		for (int j = 0; j < _stackCount + 1; j++) //_stackCount + 1 required for "south pole"
		{
			GLfloat theta = 2 * M_PI * ((float)i / _sectorCount);
			GLfloat phi = M_PI / 2 - M_PI * ((float)j / _stackCount);

			Vertex spherePoint;
			spherePoint.sphereVertex(_radius, theta, phi);
			_vertices[i].push_back(spherePoint);
		}
	}
}

//Output
void Planet::draw()
{
	glPushMatrix();
	Vertex absOffset = getAbsolutePos();
	glTranslatef(absOffset.getX(),absOffset.getY(),absOffset.getZ());



	glBegin(GL_TRIANGLES);
	//Iterate through all horizontal indices
	for (int j = 0; j < _stackCount; j++)
	{
		for (int i = 0; i < _sectorCount; i++)
		{
 			usePrimaryColor(); //Color 1
			
			glVertex3f(_vertices[i][j].getX(),_vertices[i][j].getY(),_vertices[i][j].getZ());
			glVertex3f(_vertices[i][j + 1].getX(),_vertices[i][j + 1].getY(),_vertices[i][j + 1].getZ());
			glVertex3f(_vertices[i + 1][j].getX(),_vertices[i + 1][j].getY(),_vertices[i + 1][j].getZ());
  			
			useSecondaryColor(); //Color 2

			glVertex3f(_vertices[i + 1][j].getX(),_vertices[i + 1][j].getY(),_vertices[i + 1][j].getZ());
			glVertex3f(_vertices[i][j + 1].getX(),_vertices[i][j + 1].getY(),_vertices[i][j + 1].getZ());
			glVertex3f(_vertices[i + 1][j + 1].getX(),_vertices[i + 1][j + 1].getY(),_vertices[i + 1][j + 1].getZ());
		}
	}
	glEnd();
	glPopMatrix();
}

//Call colors
void Planet::usePrimaryColor()
{
	glColor4fv(_color1);
}

void Planet::useSecondaryColor()
{
	glColor4fv(_color2);
}

//Setters
void Planet::setParent(Planet* orbitParent)
{
	_parent = orbitParent;
}

void Planet::setOffset(GLfloat x, GLfloat y, GLfloat z)
{
	_offset = Vertex(x, y, z);
}

void Planet::setOrbit(GLfloat radius, GLfloat speed, GLfloat progress = 0.0f)
{
	_orbitSpeed = speed;
	_orbitRadius = radius;
	_orbitProgress = progress;
}

void Planet::orbitStep()
{
	_orbitProgress += _orbitSpeed;
	GLfloat x = _orbitRadius * cos(_orbitProgress);
	GLfloat y = _orbitRadius * sin(_orbitProgress);
	GLfloat z = _offset.getZ();

	// std::cout << "orbitProgress: " << _orbitProgress << std::endl;
	// std::cout << "orbitSpeed: " << _orbitSpeed << std::endl;
	// std::cout << "x: " << x << std::endl;
	// std::cout << "y: " << y << std::endl;
	// std::cout << "z: " << z << std::endl;
	

	setOffset(x, y, z);
	//_offset = Vertex(x, y, z);
}

void Planet::setSize(GLfloat radius) {
	_radius = radius;
}

//Polygon rgb values
void Planet::setPrimaryColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	_color1[0] = r;
	_color1[1] = g;
	_color1[2] = b;
	_color1[3] = a;
}

void Planet::setSecondaryColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	_color2[0] = r;
	_color2[1] = g;
	_color2[2] = b;
	_color2[3] = a;
}

//Finds the cumulative offset of planet & all parents in hierarchy
Vertex Planet::getAbsolutePos()
{
	//If planet does not have an orbit parent, offset is relative to origin
	if (_parent == nullptr)
		return _offset;
	
	//Otherwise, add the offset of the parent (recursive)
	Vertex parentOffset = _parent->getAbsolutePos();

	return _offset + parentOffset;
}