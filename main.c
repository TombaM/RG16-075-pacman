#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include "draw.h"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 900

#define MAP_WIDTH 26
#define MAP_HEIGHT 23

#define PI 3.1415926535
#define EPSILON 0.01

static int window_width, window_height;
int map[MAP_HEIGHT][MAP_WIDTH];

/* Deklaracija callback funkcija */
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);

/* Deklaracija pomocnih funkcija */
void get_map_pattern();
void light();
void show_score();

int x_player = 3;
int y_player = 0;
int z_player = -3;
int rotate = 0;
int score = 0;

int main(int argc, char** argv) {
  /* Ucitavanje map patterna */
  get_map_pattern();

  /* GLUT inicijalizacija */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  /* Kreiranje prozora */
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("3D Pacman Game");

  /* Registracija callback funkcija */
  glutDisplayFunc(on_display);
  glutKeyboardFunc(on_keyboard);
  glutReshapeFunc(on_reshape);

  /* OpenGL inicijalizacija */
  glClearColor(0.25, .2, .2, 1);
  glEnable(GL_DEPTH_TEST);

  light();

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

  gluLookAt(20, 40, -3, 20, 5, -18, 0, 1, 0);
  			int i, j;
  			for(i = 0; i < MAP_HEIGHT; i++)
        {
  				for(j = 0; j < MAP_WIDTH; j++)
          {
  					if(map[i][j] == 0)
            {
              draw_wall(2*i+1, 0, -2*j-1);
            }
  					else if(map[i][j] == 2 || map[i][j] == 1)
            {
  					  draw_floor(2*i+1, -1, -2*j-1);

              if(map[i][j] == 1)
              {
                  draw_food(2*i+1, 0, -2*j-1, 1, 1, 1, 45, 0.5);
              }
  				 }
  			 }
       }

  draw_player(x_player, y_player, z_player, 0.9, rotate);

  show_score();

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
        /* Omogucavanje teleportovanja igraca, kada dodje do vrha mape */
        if(-1*(z_player)/2 == MAP_HEIGHT+1)
          z_player += 2*MAP_HEIGHT+4;

        if(map[x_player/2][-1*(z_player-2)/2] != 0)
        {
          z_player -= 2;

          /* Provera da li se na planiranom potezu nalazi hrana i azuriranje score-a */
          if(map[x_player/2][-1*(z_player)/2] == 1)
            score++;

          map[x_player/2][-1*(z_player)/2] = 2;
        }
        break;

        case 's':
        case 'S':
          /* Omogucavanje teleportovanja igraca, kada dodje do dna mape */
          if(-1*(z_player)/2 == 0)
            z_player -= 2*MAP_HEIGHT+4;

          if(map[x_player/2][-1*(z_player+2)/2] != 0)
          {
            z_player += 2;

            if(map[x_player/2][-1*(z_player)/2] == 1)
              score++;

            map[x_player/2][-1*(z_player)/2] = 2;
          }
          break;

        case 'd':
        case 'D':
          if(map[(x_player+2)/2][-1*(z_player)/2] !=0)
          {
            x_player += 2;

            if(map[x_player/2][-1*(z_player)/2] == 1)
              score++;

            map[x_player/2][-1*(z_player)/2] = 2;
        }
        break;

        case 'a':
        case 'A':
          if(map[(x_player-2)/2][-1*z_player/2] != 0)
          {
            x_player -= 2;

            if( map[x_player/2][-1*(z_player)/2] == 1)
              score++;

            map[x_player/2][-1*(z_player)/2] = 2;
        }
        break;
  }

  glutPostRedisplay();
}

static void on_reshape(int width, int height) {
  window_width =  width;
  window_height = height;

  /* Podesava se viewport. */
  glViewport(0, 0, window_width, window_height);

  /* Podesavanje projekcije */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, window_width / (float)window_height, 1, 100);
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
      char temp;
      fscanf(file, "%c", &temp);
      map[i][j] = temp - '0';
    }
  }
}


void light()
{
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1};
  GLfloat light_diffuse[] = {1, 1, 1, 1};
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

  GLfloat shiness = 20;
  glMaterialf(GL_FRONT, GL_SHININESS, shiness);


  GLfloat light_position[] = { 0, 0, 10, 0 };
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

}

void show_score()
{
  /* Ucitavamo trenutni score */
  char buff[256];
  sprintf(buff, "%i", score);

  glColor3f(1,1,1);
  glDisable(GL_LIGHTING); // Kako bi se dobila zeljena boja neophodno je iskljuciti osvetljenje
 	glRasterPos3f(48,5.7,-42);
	glutBitmapString(GLUT_BITMAP_9_BY_15, "SCORE: ");
  glutBitmapString(GLUT_BITMAP_9_BY_15, buff);
  glEnable(GL_LIGHTING); // Nakon sto zavrsimo sa prosledjivanjem zeljenog texta, ukljucujemo nazad osvetljenje
}
