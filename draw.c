#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "draw.h"

void draw_blue_cube(float x1, float x2, float y1, float y2, float z1, float z2) {
  glBegin(GL_QUADS);

	glColor3f(0, 0, 1);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y2, z1);
	glVertex3f(x1, y2, z1);

	glVertex3f(x1, y1, z2);
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y2, z2);
	glVertex3f(x1, y2, z2);

	glVertex3f(x1, y2, z1);
	glVertex3f(x2, y2, z1);
	glVertex3f(x2, y2, z2);
	glVertex3f(x1, y2, z2);

	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y2, z1);
	glVertex3f(x1, y2, z2);
	glVertex3f(x1, y1, z2);

	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y2, z1);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2, y1, z2);

	glEnd();
}
