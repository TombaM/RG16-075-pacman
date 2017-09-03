#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "draw.h"

/* Funkcija za iscrtavanje zidova */
void draw_wall(float x, float y, float z)
{
	GLfloat diffuse_coeffs[] = { 0.094, 0.000, 0.510, 1 };
	GLfloat ambient_coeffs[] = {0.294, 0.000, 0.510, 1};
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		glColor3f(0.2, 0.2, 1);
		glTranslatef(x, -.5, z);
		glScalef(1, 0.5, 1);
		glutSolidCube(2);
	glPopMatrix();
}

/* Funkcija za iscrtavanje podloge */
void draw_floor(float x, float y, float z)
{
	GLfloat diffuse_coeffs[] = { 184, 134, 11, 1 };
	GLfloat ambient_coeffs[] = {184, 134, 11, 1};
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		glColor3f(1, 1, 0);
		glTranslatef(x, y, z);
		glScalef(1, 0, 1);
		glutSolidCube(2);
	glPopMatrix();
}

/* Funkcija za iscrtavanje hrane po mapi */
void draw_food1(float x_cord_translate, float y_cord_translate, float z_cord_translate,
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
		glRotatef(80, 1, 1, 1);
		glutSolidCube(size_of_cube);
	glPopMatrix();
}

void draw_food2(float x_cord_translate, float y_cord_translate, float z_cord_translate,
							 float x_cord_rotate, float y_cord_rotate, float z_cord_rotate,
							 float angle_of_rotation, float size_of_cube)
{
	GLfloat diffuse_coeffs[] = { .6, .2, 0, 1 };
	GLfloat ambient_coeffs[] = {.6, .2, 0, 1};

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		glColor3f(1, 1, 0);
		glTranslatef(x_cord_translate, y_cord_translate, z_cord_translate);
		glRotatef(angle_of_rotation, x_cord_rotate, y_cord_rotate, z_cord_rotate);
		glRotatef(0, 1, 1, 1);
		glutSolidCube(size_of_cube);
	glPopMatrix();
}

/* Funkcija za iscrtavanje igraca */
void draw_player(float x_cord, float y_cord, float z_cord, float size_of_player, int rotation, int parameter)
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
		/* Iscrtavanje usta */
		glPushMatrix();
			glDisable(GL_LIGHTING);
			glColor3f(0, 0, 0);
			glTranslatef(0, 1, .5);
			glScalef(1, .2, cos(parameter*.05)*.5);
			glutSolidSphere(size_of_player*.5, 100, 100);
			glEnable(GL_LIGHTING);
		glPopMatrix();
		/* Iscrtavanje ociju */
		glPushMatrix();
			glDisable(GL_LIGHTING);
			glColor3f(1, 1, 1);
			glTranslatef(-.5, 1, 0);
			glPushMatrix();
				glColor3f(0, 0, 0);
				glTranslatef(0, .3, 0);
				glutSolidSphere(size_of_player*.1, 100, 100);
				glColor3f(1, 1, 1);
			glPopMatrix();
			glutSolidSphere(size_of_player*.2, 100, 100);
			glEnable(GL_LIGHTING);
		glPopMatrix();
		/* Iscrtavanje ociju */
		glPushMatrix();
			glDisable(GL_LIGHTING);
			glTranslatef(.5, 1, 0);
			glPushMatrix();
				glColor3f(0, 0, 0);
				glTranslatef(0, .3, 0);
				glutSolidSphere(size_of_player*.1, 100, 100);
				glColor3f(1, 1, 1);
			glPopMatrix();
			glutSolidSphere(size_of_player*.2, 100, 100);
			glEnable(GL_LIGHTING);
		glPopMatrix();
		/* Iscrtavanje samog igraca */
		glutSolidSphere(size_of_player, 100, 20);
	glPopMatrix();
}

/* Funkcija za iscrtavanje neprijatelja */
void draw_enemy(float x_cord, float y_cord, float z_cord, float size_of_enemy)
{
	GLfloat diffuse_coeffs[] = { .1, .9, .1, 1 };
	GLfloat ambient_coeffs[] = {.5, 0, .9, 1};
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		glColor3f(1, 0, 0);
		glTranslatef(x_cord, y_cord, z_cord);
			glPushMatrix();
				glTranslatef(-.5, -.5, .5);
				glutSolidSphere(size_of_enemy*.2, 100, 100);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-.5, -.5, -.5);
				glutSolidSphere(size_of_enemy*.2, 100, 100);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0, -.5, .5);
				glutSolidSphere(size_of_enemy*.2, 100, 100);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0, -.5, -.5);
				glutSolidSphere(size_of_enemy*.2, 100, 100);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(.5, -.5, .5);
				glutSolidSphere(size_of_enemy*.2, 100, 100);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(.5, -.5, -.5);
				glutSolidSphere(size_of_enemy*.2, 100, 100);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(.8, -.5, 0);
				glutSolidSphere(size_of_enemy*.2, 100, 100);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-.8, -.5, 0);
				glutSolidSphere(size_of_enemy*.2, 100, 100);
			glPopMatrix();
			glPushMatrix();
				glDisable(GL_LIGHTING);
				glColor3f(0, 0, 0);
				glTranslatef(.3, 1.5, .5);
				glScalef(.5, 3, 1);
				glutSolidSphere(size_of_enemy*.1, 100, 100);
				glEnable(GL_LIGHTING);
			glPopMatrix();
			glPushMatrix();
				glDisable(GL_LIGHTING);
				glColor3f(0, 0, 0);
				glTranslatef(-.3, 1.5, .5);
				glScalef(.5, 3, 1);
				glutSolidSphere(size_of_enemy*.1, 100, 100);
				glEnable(GL_LIGHTING);
			glPopMatrix();
			glScalef(1, 2, 1);
			glutSolidSphere(size_of_enemy *.4, 100, 100);
		glPopMatrix();
	glPopMatrix();
}
