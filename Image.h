/*====================================================*\
  Vendredi 8 novembre 2013
  Arash HABIBI
  Image.h
\*====================================================*/

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include "Ppm.h"

//-----------------------------------------------------

typedef struct
{
	float _red, _green, _blue;
} Color;

Color C_new(float red, float green, float blue);
void C_check(Color c, char *message);

//-----------------------------------------------------

typedef struct
{
	int _width, _height;
	int _xzoom, _yzoom;
	int _xoffset, _yoffset;
	double _zoom;
	Color _current_color;
	Color **_buffer;
} Image;

Image* I_new(int _width, int _height);
Image* I_read(char *ppmfilename);

void I_fill       (Image *img, Color c);
void I_checker    (Image *img, Color c1, Color c2, int step);

void I_changeColor(Image *img, Color c);
void I_plot       (Image *img, int x, int y);
void I_plotColor  (Image *img, int x, int y, Color c);

void I_focusPoint (Image *img, int x, int y);
void I_zoomInit   (Image *img);
void I_zoom       (Image *img, double zoom_coef);
void I_move       (Image *img, int x, int y);

/**
 * @brief Dessiner (en blanc sur fond noir) une droite de Bresenham reliant l’origine au pixel de coordonnées (x,y). On suppose que le pixel p (x,y) est dans le premier octant.
 * @param img L'image.
 * @param x   L'abscisse.
 * @param y   L'ordonnee.
 */
void I_bresenhamOrigin (Image * img , int x , int y) ;
/**
 * @brief Generaliser I_bresenhamOrigin.
 * @param img L'image.
 * @param xA  L'abscisse du point a.
 * @param yA  L'ordonnee du point a.
 * @param xB  L'abscisse du point b.
 * @param yB  L'ordonnee du point b.
 */
void I_bresenham (Image * img , int xA , int yA , int xB , int yB) ;

void I_draw       (Image *img);

#endif



