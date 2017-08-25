#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

void draw_blue_cube(float x1, float x2, float y1, float y2, float z1, float z2/*float x_cord_translate, float y_cord_translate, float z_cord_translate, float size_of_cube*/);
void draw_floor(float x1, float x2, float y1, float z1, float z2/*float x_cord_translate, float y_cord_translate, float z_cord_translate,*/);


void draw_food(float x_cord_translate, float y_cord_translate, float z_cord_translate,
               float x_cord_rotate, float y_cord_rotate, float z_cord_rotate,
               float angle_of_rotation, float size_of_cube);

               void draw_player(float x_cord, float y_cord, float z_cord, float size_of_player, int rotation);
