#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>


void draw_food(float x_cord_translate, float y_cord_translate, float z_cord_translate,
               float x_cord_rotate, float y_cord_rotate, float z_cord_rotate,
               float angle_of_rotation, float size_of_cube);
void draw_player(float x_cord, float y_cord, float z_cord, float size_of_player, int rotation);
void draw_wall(float x, float y, float z);
void draw_floor(float x, float y, float z);
