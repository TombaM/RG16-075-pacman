#include <string.h>
#include <math.h>
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

/* Struktura za igraca */
typedef struct
{
  float x_cord;
  float y_cord;
  float z_cord;
  char movement;
} m_Player;
/* Definisemo igraca sa inicijalnim koordinatama i pravcem kretanja */
m_Player Player = {3, 0, -3, 'w'};

/* Pomocne promenljive koje nam sluze kao indikator kretanja igraca */
int player_horizontal = 0;
int player_vertical = 0;

/* Struktura za neprijatelja */
typedef struct _enemy
{
  float x_cord;
  float y_cord;
  float z_cord;
  char movement;
} m_Enemy;
/* Definisiemo konkretne neprijatelje sa inicijalnim koordinatama i pravcima kretanja */
m_Enemy Enemy1 = {23, 0, -19, 'w'};
m_Enemy Enemy2 = {23, 0, -31, 's'};
m_Enemy Enemy3 = {23, 0, -25, 'a'};

/* Deklaracija pomocnih funkcija */
void get_map_pattern();
void get_map_over_pattern();
void light();
void show_score();
void show_exit();
void check_end();
void enemy_direction(m_Enemy* enemy);
void move_enemy(m_Enemy* enemy);
void move_player();

/* Deklaracija neophodnih parametara u vidu globalnih promenljivih */
/* Promenljiva koja cuva rezultat */
int score = 0;
/*Indikator da li je igra aktivna ili pauzirana */
int game_started = 0;
/* Parametar za animacije*/
int animation_parameter = 0;
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
  glutInitWindowPosition(200, 100);
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
  gluLookAt(20, 40, -5, 20, 5, -18, 0, 1, 0);

  /* Ukoliko je igra nije zavrsena */
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
            draw_food1(2*i+1, -1, -2*j-1.2, 0, 1, 1, animation_parameter*2, 0.5);
            draw_food2(2*i+1, -1, -2*j-1.2, 0, 1, 1, animation_parameter*2, 0.5);
          }
		    }
	    }
    }

    draw_player(Player.x_cord, Player.y_cord, Player.z_cord, 1, animation_parameter, animation_parameter);
    draw_enemy(Enemy1.x_cord, Enemy1.y_cord, Enemy1.z_cord, 2, .1, .9, .1, .5, 0, .9);
    draw_enemy(Enemy2.x_cord, Enemy2.y_cord, Enemy2.z_cord, 2, .8, 0, .5, .8, 0, .4);
    draw_enemy(Enemy3.x_cord, Enemy3.y_cord, Enemy3.z_cord, 2, .8, .8, .8, .8, .8, .8);
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

  /* Ukoliko je igra aktivna azuriramo parametar za animacije */
  if(game_started == 1)
    animation_parameter += 1;

  check_end();

  move_player();
  move_enemy(&Enemy1);
  move_enemy(&Enemy2);
  move_enemy(&Enemy3);

  glutPostRedisplay();

  if(game_started)
    glutTimerFunc(10, on_timer, 0);
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

static void on_keyboard(unsigned char key, int x, int y) {
  switch(key) {
    /* ESC */
    case 27:
      exit(0);
      break;

    /* Space */
    case 32:
      /* Start game */
      if(!game_started)
      {
        glutTimerFunc(10, on_timer, 0);
        game_started = 1;
      }
      /* Pause */
      else
        game_started = 0;
      break;

    case 'w':
    case 'W':
      if(game_started)
        Player.movement = 'w';
      break;

    case 's':
    case 'S':
      if(game_started)
        Player.movement = 's';
      break;

    case 'd':
    case 'D':
      if(game_started)
        Player.movement = 'd';
      break;

    case 'a':
    case 'A':
      if(game_started)
        Player.movement = 'a';
      break;
  }
  glutPostRedisplay();
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

  // for(i = 0; i < MAP_HEIGHT; i++) {
  //   for(j = 0; j < MAP_WIDTH - 1; j++) {
  //       printf("%i ", map[i][j]);
  //     }
  //     printf("\n");
  // }
}

/* Ucitavanje matrice kao sablona za mapu za kraj igre */
void get_map_over_pattern() {
  FILE* file;
  if((file = fopen("map_over.txt", "r")) == NULL) {
    printf("Greska pri otvaranju datoteke!\n");
    exit(1);
  }

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
  if(abs(Enemy1.x_cord - Player.x_cord) <=  1 && abs(Enemy1.z_cord - Player.z_cord) <= .8)
    over = 1;
  if(abs(Enemy2.x_cord - Player.x_cord) <=  1 && abs(Enemy2.z_cord - Player.z_cord) <= .8)
    over = 1;
  if(abs(Enemy3.x_cord - Player.x_cord) <= 1 && abs(Enemy3.z_cord - Player.z_cord) <= .8)
    over = 1;
}

/* Funkcija koja odredjuje pravac kretanja neprijatelja */
void enemy_direction(m_Enemy* enemy)
{
  /* Promenljiva koja nam odredjuje kretanje neprijatelja */
  int enemy_movement;

  /* Pomocne promenljive za odredjivanje pozicije igraca u odnosu na poziciju neprijatelja */
  int levo = 0;
  int gore = 0;


  if(Player.x_cord > enemy->x_cord)
    levo = 0;
  else
    levo = 1;

  if(Player.z_cord < enemy->z_cord)
    gore = 1;
  else
    gore = 0;

  /* U zavisnosti od toga gde se nalaze neprijatelji u odnosu na igraca, razlicita je verovatnoca za naredno polje na koje ce otici */
  enemy_movement = rand() % 10;

  if(levo && gore)
  {
      if(enemy_movement < 4)
        enemy->movement = 'a';
      else if(enemy_movement >= 4 && enemy_movement < 8)
        enemy->movement = 'w';
      else if(enemy_movement == 8)
        enemy->movement = 's';
      else if(enemy_movement == 9)
        enemy->movement = 'd';
  } else if(levo && !gore)
  {
    if(enemy_movement < 4)
      enemy->movement = 'a';
    else if(enemy_movement >= 4 && enemy_movement < 8)
      enemy->movement = 's';
    else if(enemy_movement == 8)
      enemy->movement = 's';
    else if(enemy_movement == 9)
      enemy->movement = 'w';
  } else if(!levo && gore)
  {
    if(enemy_movement < 4)
      enemy->movement = 'd';
    else if(enemy_movement >= 4 && enemy_movement < 8)
      enemy->movement = 'w';
    else if(enemy_movement == 8)
      enemy->movement = 'a';
    else if(enemy_movement == 9)
      enemy->movement = 'd';
  } else if(!levo && !gore)
  {
    if(enemy_movement < 4)
      enemy->movement = 'd';
    else if(enemy_movement >= 4 && enemy_movement < 8)
      enemy->movement = 's';
    else if(enemy_movement == 8)
      enemy->movement = 'w';
    else if(enemy_movement == 9)
      enemy->movement = 'a';
  }
}

/* Funkcija koja omogucava kretanje igraca */
void move_player()
{
  /* Promenljive koje predstavljaju trenutne koordinate igraca na mapi */
  int player_current_x = floor(Player.x_cord) / 2;
  int player_current_z = floor(-1 * Player.z_cord) / 2;

  /* Update rezultata ukoliko se igrac pokupi hranu */
  if(map[player_current_x][player_current_z] == 1) {
    map[player_current_x][player_current_z] = 2;
    score++;
  }

  /* Promenljive koje ce nam sluziti za proveru validnosti narednog poteza */
  int player_next_x;
  int player_next_z;

  if(Player.movement == 'w')
  {
    /* Ovo nam omogucava da se igrac teleportuje ukoliko dodje do gornjeg portala */
    if(player_current_z == MAP_HEIGHT+2)
      Player.z_cord = 0;

    player_next_x = floor(Player.x_cord) / 2;
    player_next_z = floor(-1*(Player.z_cord - 1)) / 2;

    // printf("%i - %i\n%f.2 - %f.2\n%i - %i\n\n\n", player_current_x, player_current_z, Player.x_cord, Player.z_cord, player_next_x, player_next_z);
    if(map[player_next_x][player_next_z] != 0)
    {
      /* Ukoliko igrac moze da se krece na gore, azuriramo koordinate i postavljamo kretanje  na vertikalno */
      Player.z_cord -= .15;
      player_horizontal = 0;
      player_vertical = 1;
    }
    else
      /* Ukoliko se igrac krece vertikalno i dodje do zida, odbija se o zid i krece se na dole */
      if(player_vertical == 1)
        Player.movement = 's';
  } else if(Player.movement == 's')
  {
    /* Ovo nam omogucava da se igrac teleportuje ukoliko dodje do donjeg portala */
    if(player_current_z < 0)
      Player.z_cord -= 2*MAP_HEIGHT+4;

    player_next_x = floor(Player.x_cord) / 2;
    player_next_z = floor(-1*(Player.z_cord + 1)) / 2;

    if(map[player_next_x][player_next_z] != 0)
    {
      /* Ukoliko igrac moze da se krece na dole, azuriramo koordinate i postavljamo kretanje  na vertikalno */
      Player.z_cord += .15;
      player_horizontal = 0;
      player_vertical = 1;
    }else
      /* Ukoliko se igrac krece vertikalno i dodje do zida, odbija se o zid i krece se na gore */
      if(player_vertical == 1)
        Player.movement = 'w';
  } else if(Player.movement == 'd')
  {
    player_next_x = floor(Player.x_cord+1) / 2;
    player_next_z = floor(-1*Player.z_cord) / 2;

    if(map[player_next_x][player_next_z] != 0)
    {
        /* Ukoliko igrac moze da se krece na desno, azuriramo koordinate i postavljamo kretanje  na horizontalno */
        Player.x_cord += .15;
        player_horizontal = 1;
        player_vertical = 0;
    }
    else
      /* Ukoliko se igrac krece horizontalno i dodje do zida, odbija se o zid i krece se na levo */
      if(player_horizontal == 1)
        Player.movement = 'a';
  } else if(Player.movement == 'a')
  {
    player_next_x = floor(Player.x_cord - 1) / 2;
    player_next_z = floor(-1*Player.z_cord) / 2;

    if(map[player_next_x][player_next_z] != 0)
    {
      /* Ukoliko igrac moze da se krece na levo, azuriramo koordinate i postavljamo kretanje  na horizontalno */
      Player.x_cord -= .15;
      player_horizontal = 1;
      player_vertical = 0;
    }else
      /* Ukoliko se igrac krece horizontalno i dodje do zida, odbija se o zid i krece se na desno */
      if(player_horizontal == 1)
        Player.movement = 'd';
  }
}

/* Funkcija koja omogucava kretanje neprijatelja po mapi */
void move_enemy(m_Enemy* enemy)
{
  int enemy_next_x;
  int enemy_next_z;

  int enemy_current_z = floor(-1 * enemy->z_cord) / 2;

  if(enemy->movement == 'w')
  {
    /* Teleportovanje neprijatelja ukoliko dodje do gornjeg portala */
    if(enemy_current_z >= MAP_HEIGHT+2)
      enemy->z_cord = 0;

    enemy_next_x = floor(enemy->x_cord) / 2;
    enemy_next_z = floor(-1*(enemy->z_cord - 2)) / 2;

    /* Ukoliko se krece na gore i naidje na put i sa leve i sa desne strane, poziva se funkcija za odredjivanje pravca kretanja neprijatelja kako ne bi isao samo gore-dole */
    if(map[enemy_next_x-1][enemy_next_z] != 0)
      enemy_direction(enemy);

    /* Ukoliko je pravac kretanja gore, pokusava gore, u suprotnom se ponovo poziva funkcija za odredjivanja pravca kretanja neprijatelja */
    if(map[enemy_next_x][enemy_next_z] != 0)
    {
      enemy->z_cord -= .15;
    } else
      enemy_direction(enemy);
  } else if(enemy->movement == 's')
  {
    /* Teleportovanje neprijatelja ukoliko dodje do donjeg portala */
    if(enemy_current_z < 0)
      enemy->z_cord = 2*MAP_HEIGHT+4;

    enemy_next_x = floor(enemy->x_cord) / 2;
    enemy_next_z = floor(-1*(enemy->z_cord + 2)) / 2;

    /* Ukoliko se krece na dole i naidje na put i sa leve i sa desne strane, poziva se funkcija za odredjivanje pravca kretanja neprijatelja kako ne bi isao samo gore-dole */
    if(map[enemy_next_x-1][enemy_next_z+1] != 0 )
      enemy_direction(enemy);

    /* Ukoliko je pravac kretanja dole, pokusava dole, u suprotnom se ponovo poziva funkcija za odredjivanja pravca kretanja neprijatelja */
    if(map[enemy_next_x][enemy_next_z] != 0)
    {
      enemy->z_cord += .15;
    }
    else
      enemy_direction(enemy);
  } else if(enemy->movement == 'd')
  {
    enemy_next_x = floor(enemy->x_cord+2) / 2;
    enemy_next_z = floor(-1*enemy->z_cord) / 2;

    /* Ukoliko je pravac kretanja desno, pokusava desno, u suprotnom se ponovo poziva funkcija za odredjivanja pravca kretanja neprijatelja */
    if(map[enemy_next_x][enemy_next_z] != 0)
    {
        enemy->x_cord += .15;
    }
    else
      enemy_direction(enemy);
  } else if(enemy->movement == 'a')
  {
    enemy_next_x = floor(enemy->x_cord - 2) / 2;
    enemy_next_z = floor(-1*enemy->z_cord) / 2;

    /* Ukoliko je pravac kretanja levo, pokusava levo, u suprotnom se ponovo poziva funkcija za odredjivanja pravca kretanja neprijatelja */
    if(map[enemy_next_x][enemy_next_z] != 0)
    {
      enemy->x_cord -= .15;
    }
    else
      enemy_direction(enemy);
  }
}
