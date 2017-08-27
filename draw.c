#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "draw.h"

void draw_wall(float x, float y, float z)
{
	GLfloat diffuse_coeffs[] = { 0, .2, .9, 1 };
	GLfloat ambient_coeffs[] = {0, 1, 1, 1};
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		glColor3f(0.2, 0.2, 1);
		glTranslatef(x, -.5, z);
		glScalef(1, 0.5, 1);
		glutSolidCube(2);
	glPopMatrix();
}

void draw_floor(float x, float y, float z)
{
	GLfloat diffuse_coeffs[] = { 1, 1, 1, 1 };
	GLfloat ambient_coeffs[] = {229, 255, 204, 1};
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		glColor3f(1, 1, 0);
		glTranslatef(x, y, z);
		glScalef(1, 0, 1);
		glutSolidCube(2);
	glPopMatrix();
}

void draw_food(float x_cord_translate, float y_cord_translate, float z_cord_translate,
							 float x_cord_rotate, float y_cord_rotate, float z_cord_rotate,
							 float angle_of_rotation, float size_of_cube)
{
	GLfloat diffuse_coeffs[] = { 1, 0, 0, 1 };
	GLfloat ambient_coeffs[] = {1, 0, 0, 1};

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		glColor3f(1, 0, 0);
		glTranslatef(x_cord_translate, y_cord_translate, z_cord_translate);
		glRotatef(angle_of_rotation, x_cord_rotate, y_cord_rotate, z_cord_rotate);
		glutSolidCube(size_of_cube);
	glPopMatrix();
}

void draw_player(float x_cord, float y_cord, float z_cord, float size_of_player, int rotation)
{
	GLfloat diffuse_coeffs[] = { 1, 1, 0, 1 };
	GLfloat ambient_coeffs[] = {1, 1, 0, 1};
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		glColor3f(1, 1, 0);
		glPushMatrix();
		glRotatef(rotation, 0, 1, 0);
		glPopMatrix();
		glTranslatef(x_cord, y_cord, z_cord);
		glutSolidSphere(size_of_player, 100, 100);
	glPopMatrix();
}
