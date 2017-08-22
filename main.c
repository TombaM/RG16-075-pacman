#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "draw.h"

#define WINDOW_HEIGHT 700
#define WINDOW_WIDTH 900

#define MAP_WIDTH 26
#define MAP_HEIGHT 23

#define PI 3.1415926535
#define EPSILON 0.01

static int window_width, window_height;
static char map[MAP_HEIGHT][MAP_WIDTH];

/* Deklaracija callback funkcija */
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);

/* Deklaracija pomocnih funkcija */
void get_map_pattern();

int x_cord = 1;
int y_cord = 1;

int main(int argc, char** argv) {
  /* Ucitavanje map patterna */
  get_map_pattern();

  /* GLUT inicijalizacija */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  /* Kreiranje prozora */
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(300, 100);
  glutCreateWindow("3D Pacman Game");

  /* Registracija callback funkcija */
  glutDisplayFunc(on_display);
  glutKeyboardFunc(on_keyboard);
  glutReshapeFunc(on_reshape);

  /* OpenGL inicijalizacija */
  glClearColor(0, 0, 0, 0);
  glEnable(GL_DEPTH_TEST);

  /* Pokretanje glavne petlje */
  glutMainLoop();

  return 0;
}

static void on_display(void) {

  /* Brisanje prethodnog sadrzaja prozora */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Podesavanje vidne tacke */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

			gluLookAt(2, 35, 0, 2, 0, -15, 0, 1, 0);


  int i, j;
  int pacman_drawn = 0;

  for(i = 1; i <= MAP_HEIGHT; i++) {
    for(j = 1; j <= MAP_WIDTH; j++) {
      if(map[i-1][j-1] == 'o') {
        //draw_blue_cube(0.4*i, 0.4*(i+1), -0.2, 0.2, -0.4*j, -0.4*(j+1));
        // draw_blue_cube(0.2*i, 0.2*(i+1), -0.1, 0.1, -0.2*j, -0.2*(j+1));
      	glPushMatrix();
                glTranslatef(i, 0, -j);
                glColor3f(0, 0, 1);
		glutSolidCube(1);
	glPopMatrix();
      }
      else if(map[i-1][j-1] == 'x') {
        //draw_floor(0.4*i, 0.4*(i+1), -0.2, -0.4*j, -0.4*(j+1));
        glPushMatrix();
        glTranslatef(i, 0, -j);
        glColor3f(256/226, 256/219, 256/172);
		glScalef(1, 0.001, 1);
		glutSolidCube(1);
	glPopMatrix();

    if(pacman_drawn == 0)
    {
      pacman_drawn = 1;
      glPushMatrix();
      glColor3f(256/196, 256/173, 0);
        glTranslatef(x_cord*i, 0, -j*y_cord);
        //glScalef(x_cord, 1, y_cord);
        glutSolidSphere(.5, 100, 100);
      glPopMatrix();
    } else {
      glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(i, 0, -j);
        glRotatef(30, 0, 1, 0);
        glutSolidCube(0.25);
      glPopMatrix();
    }
      }
    }
  }

  /* Update slike */
  glutSwapBuffers();
}

static void on_keyboard(unsigned char key, int x, int y) {
  switch(key) {
    case 27:
      exit(0);
      break;

      case 'w':
      case 'W':
        y_cord += .1;
        printf("w stisnuto\n");
        break;

        case 'd':
        printf("d stisnuto \n");
        x_cord += .1;
        break;
  }
}

static void on_reshape(int width, int height) {
  window_width =  width;
  window_height = height;

  /* Podesava se viewport. */
  glViewport(0, 0, window_width, window_height);
//   /* Podesavanje projekcije */
//   glMatrixMode(GL_PROJECTION);
//   glLoadIdentity();
//   gluPerspective(80, window_width/(float)window_height, 1, 100);

  	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, width/(double)height, 1, 100);
}
void get_map_pattern() {
  FILE* file;
  if((file = fopen("map.txt", "r")) == NULL) {
    printf("Greska pri otvaranju datoteke!\n");
    exit(1);
  }

  /* Pattern smestamo u matricu */
  int i, j;
  for(i = 0; i < MAP_HEIGHT; i++) {
    for(j = 0; j < MAP_WIDTH; j++) {
      fscanf(file, "%c", &map[i][j]);
    }
  }
}
