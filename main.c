#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include "draw.h"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 900

#define MAP_WIDTH 26
#define MAP_HEIGHT 23

static int window_width, window_height;
/* Mapa za igru */
int map[MAP_HEIGHT][MAP_WIDTH];
/* Mapa za kraj igre */
int map_over[MAP_HEIGHT][MAP_WIDTH];

/* Deklaracija callback funkcija */
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);

/* Deklaracija pomocnih funkcija */
void get_map_pattern();
void get_map_over_pattern();
void light();
void show_score();
void show_exit();
void enemy_move();
void check_end();

/* Pocetne koordinate za igraca */
int x_player = 3;
int y_player = 0;
int z_player = -3;

int rotate = 0;
int score = 0;

/* Pocetne koordinate za neprijatelja */
int x_cord_enemy = 23;
int y_cord_enemy = 0;
int z_cord_enemy = -19;

/* Indikator za kraj igre */
int over = 0;

int main(int argc, char** argv) {
  /* Ucitavanje map patterna */
  get_map_over_pattern();
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

  /* Podesavanje parametara za osvetljenje */
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

  /* Ukoliko je igra aktivna */
  if(over == 0)
  {
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

    draw_enemy(x_cord_enemy, y_cord_enemy, z_cord_enemy, 2);
    draw_player(x_player, y_player, z_player, 0.9, rotate);
    show_score();
  }
  /* Ukoliko se igrac i neprijatelj nalaze na istoj poziciji i ukoliko vise nema hrane na mapi, igra je gotova */
  else if(over == 1 || score == 237)
  {
    int i, j;
    for(i = 0; i < MAP_HEIGHT; i++)
    {
      for(j = 0; j < MAP_WIDTH; j++)
      {
        if(map_over[j][i] == 1 || map_over[j][i] == 0)
        {
          draw_floor(2*i+1, -1, -2*j-1);
        } else
        {
          draw_wall(2*i+1, 0, -2*j-1);
        }
      }
    }
    show_exit();
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
        enemy_move();
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
        check_end();
        break;

      case 's':
      case 'S':
        enemy_move();
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
        check_end();
        break;

        case 'd':
        case 'D':
          enemy_move();
          if(map[(x_player+2)/2][-1*(z_player)/2] !=0)
          {
            x_player += 2;

            if(map[x_player/2][-1*(z_player)/2] == 1)
              score++;

            map[x_player/2][-1*(z_player)/2] = 2;
        }
        check_end();
        break;

        case 'a':
        case 'A':
          enemy_move();
          if(map[(x_player-2)/2][-1*z_player/2] != 0)
          {
            x_player -= 2;

            if( map[x_player/2][-1*(z_player)/2] == 1)
              score++;

            map[x_player/2][-1*(z_player)/2] = 2;
        }
        check_end();
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

/* Ucitavanje matrice kao sablona za mapu za igru */
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

/* Ucitavanje matrice kao sablona za mapu za kraj igre */
void get_map_over_pattern() {
  FILE* file;
  if((file = fopen("map_over.txt", "r")) == NULL) {
    printf("Greska pri otvaranju datoteke!\n");
    exit(1);
  }

  /* Ucitavamo trenutni score */
  char buff[256];
  sprintf(buff, "%i", score);

  /* Pattern smestamo u matricu */
  int i, j;
  for(i = 0; i < MAP_HEIGHT; i++) {
    for(j = 0; j < MAP_WIDTH; j++) {
      char temp;
      fscanf(file, "%c", &temp);
      map_over[i][j] = temp - '0';
    }
  }
}

/* Funkcija u kojoj se podesavaju parametri osvetljenja*/
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

/* Funkcija koja nam prikazuje trenutni score na mapi */
void show_score()
{
  /* Ucitavamo trenutni score */
  char buff[256];
  sprintf(buff, "%i", score);

  glColor3f(1,1,1);
  /* Iskljucujemo osvetljenje kako bi se se prikazala zeljena boja */
  glDisable(GL_LIGHTING);
 	glRasterPos3f(48,5.7,-42);
	glutBitmapString(GLUT_BITMAP_9_BY_15, "SCORE: ");
  glutBitmapString(GLUT_BITMAP_9_BY_15, buff);
  /* Ukljucujemo osvetljenje posto smo prikazali zeljeni text */
  glEnable(GL_LIGHTING);
}

/* Funkcija koja nam prikazuje text za zatvaranje prozora po zavrsetku igre */
void show_exit()
{
  glColor3f(1,1,1);
  /* Iskljucujemo osvetljenje kako bi se se prikazala zeljena boja */
  glDisable(GL_LIGHTING);
 	glRasterPos3f(48,5.7,-42);
	glutBitmapString(GLUT_BITMAP_9_BY_15, "Press 'Esc' to\nexit the game");
  /* Ukljucujemo osvetljenje posto smo prikazali zeljeni text */
  glEnable(GL_LIGHTING);
}

/* Funkcija koja generise kretanje neprijatelja */
void enemy_move()
{
  /* Pomocna promenljiva koja se ponasa kao indikator da li se neprijatelj pomerio u trenitnom potezu ili ne */
  int moved = 0;
  /* Promenljiva koja nam odredjuje kretanje neprijatelja */
  int enemy_movement;

  while(moved != 1)
  {
    enemy_movement = rand() % 10;

    /* Ispituje se da li je moguce da ide levo */
    if(enemy_movement == 0 || enemy_movement == 1)
    {
      if(map[(x_cord_enemy-2)/2][-1*z_cord_enemy/2] != 0)
      {
        x_cord_enemy -= 2;
        moved = 1;
      }
    }
    /* Ispituje se da li je moguce da ide desno */
    else if(enemy_movement == 2 || enemy_movement == 3 || enemy_movement == 4)
    {
      if(map[(x_cord_enemy+2)/2][-1*(z_cord_enemy)/2] !=0)
      {
        x_cord_enemy += 2;
        moved = 1;
      }
    }
    /* Ispituje se da li je moguce da ide gore */
    else if(enemy_movement == 5 || enemy_movement == 6 || enemy_movement == 7 || enemy_movement == 8)
    {
      if(-1*(z_cord_enemy)/2 == MAP_HEIGHT+1)
        z_cord_enemy += 2*MAP_HEIGHT+4;

      if(map[x_cord_enemy/2][-1*(z_cord_enemy-2)/2] != 0)
      {
        z_cord_enemy -= 2;
        moved = 1;
      }
    }
    /* Ispituje se da li je moguce da ide dole */
    else
    {
      if(-1*(z_cord_enemy)/2 == 0)
        z_cord_enemy -= 2*MAP_HEIGHT+4;

      if(map[x_cord_enemy/2][-1*(z_cord_enemy+2)/2] != 0)
      {
        z_cord_enemy += 2;
        moved = 1;
      }
    }
  }
}

/* Funkcija koja proverava da li se igrac i neprijatelj nalaze na istoj poziciji */
void check_end()
{
  if(x_cord_enemy == x_player && z_cord_enemy == z_player)
    over = 1;
}
