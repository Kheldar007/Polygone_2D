/**
 * @file   Polygon.h
 * @author Isabelle Muller 
 * @brief  Un polygone.
 */


# ifndef POLYGON_H
# define POLYGON_H


# include <stdlib.h>
# include "Image.h"


typedef struct
{
	int x ; // L'abscisse.
	int y ; // L'ordonnee.
	int z ; // Z.
} Point ; // Les coordonnes d'un point.

typedef struct polygon
{
	Point p ; // Un point.
	struct polygon * next ; // Le point suivant.
} Polygon ; // Un ensemble de points.


/**
 * @brief  Creer un Point.
 * @param  xx L'abscisse.
 * @param  yy L'ordonnee.
 * @param  zz La coordonnee z.
 * @return Un point.
 */
Point P_newPoint (int xx , int yy , int zz) ;
/**
 * @brief Afficher les coordonnes d'un point.
 * @param p Le point.
 */
void P_printPoint (Point p) ;


/**
 * @brief  Creer un nouveau polygone, constitue d'un point ou bien creer un point en fin de polygone.
 * @param  p Le premier point.
 * @return Un polygone.
 */
Polygon * P_newPolygon (Point p) ;
/**
 * @brief Ajouter un point au polygone.
 * @param pol Le polygone.
 * @param poi Le point.
 */
void P_addPoint (Polygon * pol , Point poi) ;
/**
 * @brief Afficher la ligne fermant le polygone.
 * @param i L'image sur laquelle tracer le polygone.
 * @param p Le polygone.
 */
void P_close (Image * i , Polygon * p) ;
/**
 * @brief Ouvrir le polygone.
 * @param i L'image sur laquelle tracer le polygone.
 * @param p Le polygone.
 */
void P_open (Image * i , Polygon * p) ;
/**
 * @brief Tracer un polygone.
 * @param i L'image sur laquelle tracer le polygone.
 * @param p Le polygone.
 */
void P_draw (Image * i , Polygon * p) ;


# endif
