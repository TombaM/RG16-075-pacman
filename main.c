#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
static void on_timer(int parameter);

/* Deklaracija pomocnih funkcija */
void get_map_pattern();
void get_map_over_pattern();
void light();
void show_score();
void show_exit();
void check_end();
void enemy1_direction();
void enemy2_direction();
void enemy3_direction();

int player_horizontal = 0;
int player_vertical = 0;

/* Struktura za igraca */
typedef struct
{
  float x_cord;
  float y_cord;
  float z_cord;
  char movement;
} m_Player;
m_Player Player = {3, 0, -3, 'w'};

/* Struktura za neprijatelja */
typedef struct _enemy
{
  float x_cord;
  float y_cord;
  float z_cord;
  char movement;
} m_Enemy;
m_Enemy Enemy1 = {23, 0, -19, 'w'};
m_Enemy Enemy2 = {23, 0, -31, 's'};
m_Enemy Enemy3 = {23, 0, -25, 'a'};

int rotate = 0;
int score = 0;
int game_started = 0;
int camera = 0;

/* Indikator za kraj igre */
int over = 0;
int p;

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

  gluLookAt(20, 40, -5-camera, 20, 5, -18, 0, 1, 0);

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
          draw_wall(2*i+1, 0, -2*j-1.2);
        }
  			else if(map[i][j] == 2 || map[i][j] == 1)
        {
			     draw_floor(2*i+1, -1, -2*j-1.2);

          if(map[i][j] == 1)
          {
              draw_food(2*i+1, -1, -2*j-1.2, 0, 1, 1, p*2, 0.5);
          }
		    }
	    }
    }

    draw_player(Player.x_cord, Player.y_cord, Player.z_cord, 1, rotate + p, p);
    draw_enemy(Enemy1.x_cord, Enemy1.y_cord, Enemy1.z_cord, 2);
    draw_enemy(Enemy2.x_cord, Enemy2.y_cord, Enemy2.z_cord, 2);
    draw_enemy(Enemy3.x_cord, Enemy3.y_cord, Enemy3.z_cord, 2);
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

static void on_timer(int parameter)
{
  if(parameter != 0)
    return;

  if(game_started == 1)
    p += 1;


  check_end();

  int player_current_x = floor(Player.x_cord) / 2;
  int player_current_z = floor(-1 * Player.z_cord) / 2;

  if(map[player_current_x][player_current_z] == 1) {
    map[player_current_x][player_current_z] = 2;
    score++;
  }


  int player_next_x;
  int player_next_z;
  int enemy1_next_x;
  int enemy1_next_z;
  int enemy2_next_x;
  int enemy2_next_z;
  int enemy3_next_x;
  int enemy3_next_z;

  if(Player.movement == 'w')
  {
    if(player_current_z == MAP_HEIGHT+1)
      // Player.z_cord += 2*MAP_HEIGHT+4;
      Player.z_cord = 1;
    player_next_x = floor(Player.x_cord) / 2;
    player_next_z = floor(-1*(Player.z_cord - 1)) / 2;
    printf("%i - %i\n%f.2 - %f.2\n%i - %i\n\n\n", player_current_x, player_current_z, Player.x_cord, Player.z_cord, player_next_x, player_next_z);
    if(map[player_next_x][player_next_z] != 0)
    {
      Player.z_cord -= .15;
      player_horizontal = 0;
      player_vertical = 1;
    }
    else
      if(player_vertical == 1)
        Player.movement = 's';
  } else if(Player.movement == 's')
  {
    if(player_current_z == 0)
      Player.z_cord -= 2*MAP_HEIGHT+3;
    player_next_x = floor(Player.x_cord) / 2;
    player_next_z = floor(-1*(Player.z_cord + 1)) / 2;
    if(map[player_next_x][player_next_z] != 0)
    {
      Player.z_cord += .15;
      player_horizontal = 0;
      player_vertical = 1;
    }else
      if(player_vertical == 1)
        Player.movement = 'w';
  } else if(Player.movement == 'd')
  {
    player_next_x = floor(Player.x_cord+1) / 2;
    player_next_z = floor(-1*Player.z_cord) / 2;
    if(map[player_next_x][player_next_z] != 0)
    {
        Player.x_cord += .15;
        player_horizontal = 1;
        player_vertical = 0;
    }
    else
      if(player_horizontal == 1)
        Player.movement = 'a';
  } else if(Player.movement == 'a')
  {
    player_next_x = floor(Player.x_cord - 1) / 2;
    player_next_z = floor(-1*Player.z_cord) / 2;
    if(map[player_next_x][player_next_z] != 0)
    {
      Player.x_cord -= .15;
      player_horizontal = 1;
      player_vertical = 0;
    }else
      if(player_horizontal == 1)
        Player.movement = 'd';
  }

  if(Enemy1.movement == 'w')
  {
    enemy1_next_x = floor(Enemy1.x_cord) / 2;
    enemy1_next_z = floor(-1*(Enemy1.z_cord - 2)) / 2;

    if(map[enemy1_next_x-1][enemy1_next_z] != 0)
      enemy1_direction();

    if(map[enemy1_next_x][enemy1_next_z] != 0)
    {
      Enemy1.z_cord -= .15;
    } else
      enemy1_direction();
  } else if(Enemy1.movement == 's')
  {
    enemy1_next_x = floor(Enemy1.x_cord) / 2;
    enemy1_next_z = floor(-1*(Enemy1.z_cord + 2)) / 2;

    if(map[enemy1_next_x-1][enemy1_next_z+1] != 0 || map[enemy1_next_x+1][enemy1_next_z+1] != 0)
      enemy1_direction();

    if(map[enemy1_next_x][enemy1_next_z] != 0)
    {
      Enemy1.z_cord += .15;
    }
    else
      enemy1_direction();
  } else if(Enemy1.movement == 'd')
  {
    enemy1_next_x = floor(Enemy1.x_cord+2) / 2;
    enemy1_next_z = floor(-1*Enemy1.z_cord) / 2;

    if(map[enemy1_next_x][enemy1_next_z] != 0)
    {
        Enemy1.x_cord += .15;
    }
    else
      enemy1_direction();
  } else if(Enemy1.movement == 'a')
  {
    enemy1_next_x = floor(Enemy1.x_cord - 2) / 2;
    enemy1_next_z = floor(-1*Enemy1.z_cord) / 2;

    if(map[enemy1_next_x][enemy1_next_z] != 0)
    {
      Enemy1.x_cord -= .15;
    }
    else
      enemy1_direction();
  }

  if(Enemy2.movement == 'w')
  {
    enemy2_next_x = ceil(Enemy2.x_cord) / 2;
    enemy2_next_z = floor(-1*(Enemy2.z_cord - 2)) / 2;

    if(map[enemy2_next_x-1][enemy2_next_z-1] != 0 || map[enemy2_next_x+1][enemy2_next_z-1] != 0)
      enemy2_direction();

    if(map[enemy2_next_x][enemy2_next_z] != 0)
    {
      Enemy2.z_cord -= .15;
    } else
      enemy2_direction();
  } else if(Enemy2.movement == 's')
  {
    enemy2_next_x = ceil(Enemy2.x_cord) / 2;
    enemy2_next_z = floor(-1*(Enemy2.z_cord + 2)) / 2;

    if(map[enemy2_next_x-1][enemy2_next_z+1] != 0 || map[enemy2_next_x+1][enemy2_next_z+1] != 0)
      enemy1_direction();

    if(map[enemy2_next_x][enemy2_next_z] != 0)
    {
      Enemy2.z_cord += .15;
    }
    else
      enemy2_direction();
  } else if(Enemy2.movement == 'd')
  {
    enemy2_next_x = ceil(Enemy2.x_cord+2) / 2;
    enemy2_next_z = floor(-1*Enemy2.z_cord) / 2;

    if(map[enemy2_next_x][enemy2_next_z] != 0)
    {
        Enemy2.x_cord += .15;
    }
    else
      enemy2_direction();
  } else if(Enemy2.movement == 'a')
  {
    enemy2_next_x = ceil(Enemy2.x_cord - 2) / 2;
    enemy2_next_z = floor(-1*Enemy2.z_cord) / 2;

    if(map[enemy2_next_x][enemy2_next_z] != 0)
    {
      Enemy2.x_cord -= .15;
    }
    else
      enemy2_direction();
  }
  if(Enemy3.movement == 'w')
  {
    enemy3_next_x = ceil(Enemy3.x_cord) / 2;
    enemy3_next_z = floor(-1*(Enemy3.z_cord - 2)) / 2;

    if(map[enemy3_next_x-1][enemy3_next_z] != 0 || map[enemy3_next_x+1][enemy3_next_z] != 0)
      enemy3_direction();

    if(map[enemy3_next_x][enemy3_next_z] != 0)
    {
      Enemy3.z_cord -= .15;
    } else
      enemy3_direction();
  } else if(Enemy3.movement == 's')
  {
    enemy3_next_x = ceil(Enemy3.x_cord) / 2;
    enemy3_next_z = floor(-1*(Enemy3.z_cord + 2)) / 2;

    if(map[enemy3_next_x-1][enemy3_next_z+1] != 0 || map[enemy3_next_x+1][enemy3_next_z+1] != 0)
      enemy1_direction();

    if(map[enemy3_next_x][enemy3_next_z] != 0)
    {
      Enemy3.z_cord += .15;
    }
    else
      enemy3_direction();
  } else if(Enemy3.movement == 'd')
  {
    enemy3_next_x = ceil(Enemy3.x_cord+2) / 2;
    enemy3_next_z = floor(-1*Enemy3.z_cord) / 2;

    if(map[enemy3_next_x][enemy3_next_z] != 0)
    {
        Enemy3.x_cord += .15;
    }
    else
      enemy3_direction();
  } else if(Enemy3.movement == 'a')
  {
    enemy3_next_x = ceil(Enemy3.x_cord - 2) / 2;
    enemy3_next_z = floor(-1*Enemy3.z_cord) / 2;

    if(map[enemy3_next_x][enemy3_next_z] != 0)
    {
      Enemy3.x_cord -= .15;
    }
    else
      enemy3_direction();
  }
  glutPostRedisplay();

  if(game_started)
    glutTimerFunc(10, on_timer, 0);
}


static void on_keyboard(unsigned char key, int x, int y) {
  switch(key) {
    case 27:
      exit(0);
      break;

      case 32:
        if(!game_started)
        {
          glutTimerFunc(10, on_timer, 0);
          game_started = 1;
        }
        break;

      case 'h':
      case 'H':
        game_started = 0;
        break;

      case 'w':
      case 'W':
        Player.movement = 'w';
        break;

      case 's':
      case 'S':
        Player.movement = 's';
        break;

      case 'd':
      case 'D':
        Player.movement = 'd';
        break;

      case 'a':
      case 'A':
        Player.movement = 'a';
        break;
      case 'm':
        camera++;
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


  for(i = 0; i < MAP_HEIGHT; i++) {
    for(j = 0; j < MAP_WIDTH - 1; j++) {
        printf("%i ", map[i][j]);
      }
      printf("\n");
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

  GLfloat light_position[] = { 0, 0, 20, 0 };
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

/* Funkcija koja proverava da li se igrac i neprijatelj nalaze na istoj poziciji */
void check_end()
{
  if(abs(Enemy1.x_cord - Player.x_cord) <= 1 && abs(Enemy1.z_cord - Player.z_cord) <= 1)
    over = 1;
  if(abs(Enemy2.x_cord - Player.x_cord) <= 1 && abs(Enemy2.z_cord - Player.z_cord) <= 1)
    over = 1;
}

void enemy1_direction()
{
  /* Pomocna promenljiva koja se ponasa kao indikator da li se neprijatelj pomerio u trenitnom potezu ili ne */
  // int moved = 0;
  /* Promenljiva koja nam odredjuje kretanje neprijatelja */
  int enemy1_movement;

  int levo = 0;
  int gore = 0;

  if(Player.x_cord > Enemy1.x_cord)
    levo = 0;
  else
    levo = 1;

  if(Player.z_cord < Enemy1.z_cord)
    gore = 1;
  else
    gore = 0;

  enemy1_movement = rand() % 10;

  if(levo && gore)
  {
      if(enemy1_movement < 4)
        Enemy1.movement = 'a';
      else if(enemy1_movement >= 4 && enemy1_movement < 8)
        Enemy1.movement = 'w';
      else if(enemy1_movement == 8)
        Enemy1.movement = 's';
      else if(enemy1_movement == 9)
        Enemy1.movement = 'd';
  } else if(levo && !gore)
  {
    if(enemy1_movement < 4)
      Enemy1.movement = 'a';
    else if(enemy1_movement >= 4 && enemy1_movement < 8)
      Enemy1.movement = 's';
    else if(enemy1_movement == 8)
      Enemy1.movement = 's';
    else if(enemy1_movement == 9)
      Enemy1.movement = 'w';
  } else if(!levo && gore)
  {
    if(enemy1_movement < 4)
      Enemy1.movement = 'd';
    else if(enemy1_movement >= 4 && enemy1_movement < 8)
      Enemy1.movement = 'w';
    else if(enemy1_movement == 8)
      Enemy1.movement = 'a';
    else if(enemy1_movement == 9)
      Enemy1.movement = 'd';
  } else if(!levo && !gore)
  {
    if(enemy1_movement < 4)
      Enemy1.movement = 'd';
    else if(enemy1_movement >= 4 && enemy1_movement < 8)
      Enemy1.movement = 's';
    else if(enemy1_movement == 8)
      Enemy1.movement = 'w';
    else if(enemy1_movement == 9)
      Enemy1.movement = 'a';
  }
}

void enemy2_direction()
{
  /* Pomocna promenljiva koja se ponasa kao indikator da li se neprijatelj pomerio u trenitnom potezu ili ne */
  // int moved = 0;
  /* Promenljiva koja nam odredjuje kretanje neprijatelja */
  int enemy2_movement;

  int levo = 0;
  int gore = 0;

  if(Player.x_cord > Enemy2.x_cord)
    levo = 0;
  else
    levo = 1;

  if(Player.z_cord < Enemy2.z_cord)
    gore = 1;
  else
    gore = 0;

  enemy2_movement = rand() % 10;

  if(levo && gore)
  {
      if(enemy2_movement < 4)
        Enemy2.movement = 'a';
      else if(enemy2_movement >= 4 && enemy2_movement < 8)
        Enemy2.movement = 'w';
      else if(enemy2_movement == 8)
        Enemy2.movement = 's';
      else if(enemy2_movement == 9)
        Enemy2.movement = 'd';
  } else if(levo && !gore)
  {
    if(enemy2_movement < 4)
      Enemy2.movement = 'a';
    else if(enemy2_movement >= 4 && enemy2_movement < 8)
      Enemy2.movement = 's';
    else if(enemy2_movement == 8)
      Enemy2.movement = 's';
    else if(enemy2_movement == 9)
      Enemy2.movement = 'w';
  } else if(!levo && gore)
  {
    if(enemy2_movement < 4)
      Enemy2.movement = 'd';
    else if(enemy2_movement >= 4 && enemy2_movement < 8)
      Enemy2.movement = 'w';
    else if(enemy2_movement == 8)
      Enemy2.movement = 'a';
    else if(enemy2_movement == 9)
      Enemy2.movement = 'd';
  } else if(!levo && !gore)
  {
    if(enemy2_movement < 4)
      Enemy2.movement = 'd';
    else if(enemy2_movement >= 4 && enemy2_movement < 8)
      Enemy2.movement = 's';
    else if(enemy2_movement == 8)
      Enemy2.movement = 'w';
    else if(enemy2_movement == 9)
      Enemy2.movement = 'a';
  }
}

void enemy3_direction()
{
  /* Pomocna promenljiva koja se ponasa kao indikator da li se neprijatelj pomerio u trenitnom potezu ili ne */
  // int moved = 0;
  /* Promenljiva koja nam odredjuje kretanje neprijatelja */
  int enemy3_movement;

  int levo = 0;
  int gore = 0;

  if(Player.x_cord > Enemy3.x_cord)
    levo = 0;
  else
    levo = 1;

  if(Player.z_cord < Enemy3.z_cord)
    gore = 1;
  else
    gore = 0;

  enemy3_movement = rand() % 10;

  if(levo && gore)
  {
      if(enemy3_movement < 4)
        Enemy3.movement = 'a';
      else if(enemy3_movement >= 4 && enemy3_movement < 8)
        Enemy3.movement = 'w';
      else if(enemy3_movement == 8)
        Enemy3.movement = 's';
      else if(enemy3_movement == 9)
        Enemy3.movement = 'd';
  } else if(levo && !gore)
  {
    if(enemy3_movement < 4)
      Enemy3.movement = 'a';
    else if(enemy3_movement >= 4 && enemy3_movement < 8)
      Enemy3.movement = 's';
    else if(enemy3_movement == 8)
      Enemy3.movement = 's';
    else if(enemy3_movement == 9)
      Enemy3.movement = 'w';
  } else if(!levo && gore)
  {
    if(enemy3_movement < 4)
      Enemy3.movement = 'd';
    else if(enemy3_movement >= 4 && enemy3_movement < 8)
      Enemy3.movement = 'w';
    else if(enemy3_movement == 8)
      Enemy3.movement = 'a';
    else if(enemy3_movement == 9)
      Enemy3.movement = 'd';
  } else if(!levo && !gore)
  {
    if(enemy3_movement < 4)
      Enemy3.movement = 'd';
    else if(enemy3_movement >= 4 && enemy3_movement < 8)
      Enemy3.movement = 's';
    else if(enemy3_movement == 8)
      Enemy3.movement = 'w';
    else if(enemy3_movement == 9)
      Enemy3.movement = 'a';
  }
}
