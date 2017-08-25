#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "draw.h"


void draw_blue_cube(float x1, float x2, float y1, float y2, float z1, float z2/*float x_cord_translate, float y_cord_translate, float z_cord_translate, float size_of_cube*/)
{
		GLfloat diffuse_coeffs[] = { 0, 0, 1, 1 };
		GLfloat ambient_coeffs[] = {0, 0, 0, 1};
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

	//
	// glPushMatrix();
	// 	glColor3f(0, 0, 1);
	// 	glTranslatef(x_cord_translate, y_cord_translate, z_cord_translate);
	// 	glutSolidCube(size_of_cube);
	// glPopMatrix();
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


void draw_floor(float x1, float x2, float y1, float z1, float z2/*float x_cord_translate, float y_cord_translate, float z_cord_translate,
							  float x_cord_scale, float y_cord_scale, float z_cord_scale, float size_of_cube*/)
{

	GLfloat diffuse_coeffs[] = { 0.8, 0.7, 0.3, 1 };

	GLfloat ambient_coeffs[] = {0.8, 0.7, 0.3, 1};
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

	//
	// glPushMatrix();
	// 	glColor3f(256/226, 256/219, 256/172);
	// 	glScalef(x_cord_scale, y_cord_scale, z_cord_scale);
	// 	glTranslatef(x_cord_translate, y_cord_translate, z_cord_translate);
	// 	glutSolidCube(size_of_cube);
	// glPopMatrix();

	glBegin(GL_QUADS);

	glColor3f(256/226, 256/219, 256/172);
	glVertex3f(x2, y1, z1);
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y1, z2);
	glVertex3f(x2, y1, z2);

glEnd();
}

void draw_food(float x_cord_translate, float y_cord_translate, float z_cord_translate,
							 float x_cord_rotate, float y_cord_rotate, float z_cord_rotate,
							 float angle_of_rotation, float size_of_cube)
{
	GLfloat diffuse_coeffs[] = { 1, 0, 0, 1 };
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

	glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslatef(x_cord_translate, y_cord_translate, z_cord_translate);
		glRotatef(angle_of_rotation, x_cord_rotate, y_cord_rotate, z_cord_rotate);
		glutSolidCube(size_of_cube);
	glPopMatrix();
}

void draw_player(float x_cord, float y_cord, float z_cord, float size_of_player, int rotation)
{
	glPushMatrix();
		glColor3f(1, 1, 0);
		glPushMatrix();
		glRotatef(rotation, 0, 1, 0);
		glPopMatrix();
		glTranslatef(x_cord, y_cord, z_cord);
		glutSolidSphere(size_of_player, 100, 100);
	glPopMatrix();
}
