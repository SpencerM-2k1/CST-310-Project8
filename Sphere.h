
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

#include <vector>

#include <iostream>

#include "SphereVertex.h"

//TODO: better color controls, will help differentiate "planets"

class Sphere {
	GLfloat _radius;
	GLint _sectorCount;
	GLint _stackCount;

	std::vector<std::vector<SphereVertex>> _vertices;
	GLfloat _color1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat _color2[4] = {1.0f, 1.0f, 1.0f, 1.0f};

public:
	Sphere(GLint radius, GLint numSectors, GLint numStacks);
	void draw();
	void usePrimaryColor();
	void useSecondaryColor();
	void setPrimaryColor(const GLfloat *newColor);
	void setSecondaryColor(const GLfloat *newColor);

};

Sphere::Sphere(GLint radius, GLint numSectors, GLint numStacks)
{
	_radius = radius;
	_sectorCount = numSectors;
	_stackCount = numStacks;

	for (int i = 0; i < _sectorCount + 1; i++) //_sectorCount + 1 required for final sector branching end and start of array
	{
		_vertices.push_back(std::vector<SphereVertex>());
		for (int j = 0; j < _stackCount + 1; j++) //_stackCount + 1 required for "south pole"
		{
			GLfloat theta = 2 * M_PI * ((float)i / _sectorCount);
			GLfloat phi = M_PI / 2 - M_PI * ((float)j / _stackCount);
			_vertices[i].push_back(SphereVertex(_radius, theta, phi));
		}
	}
}

void Sphere::draw()
{
	glPushMatrix();
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

void Sphere::usePrimaryColor()
{
	glColor4fv(_color1);
}

void Sphere::useSecondaryColor()
{
	glColor4fv(_color2);
}

void Sphere::setPrimaryColor(const GLfloat *newColor)
{
	for (int i = 0; i < 4; i++)
	{
		_color1[i] = newColor[i];
	}
}

void Sphere::setSecondaryColor(const GLfloat *newColor)
{
	for (int i = 0; i < 4; i++)
	{
		_color2[i] = newColor[i];
	}
}