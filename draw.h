#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>


void draw_food1(float x_cord_translate, float y_cord_translate, float z_cord_translate,
               float x_cord_rotate, float y_cord_rotate, float z_cord_rotate,
               float angle_of_rotation, float size_of_cube);
 void draw_food2(float x_cord_translate, float y_cord_translate, float z_cord_translate,
                float x_cord_rotate, float y_cord_rotate, float z_cord_rotate,
                float angle_of_rotation, float size_of_cube);
void draw_player(float x_cord, float y_cord, float z_cord, float size_of_player, int rotation, int parameter);
void draw_wall(float x, float y, float z);
void draw_floor(float x, float y, float z);
void draw_enemy(float x_cord, float y_cord, float z_cord, float r_diff, float g_diff, float b_diff,
float r_amb, float g_amb, float b_amb,float size_of_enemy);
