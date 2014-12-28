/**
 * @file   Polygon.h
 * @author Isabelle Muller 
 * @brief  Un polygone.
 */


# ifndef POLYGON_H
# define POLYGON_H


# include <stdlib.h>
# include "Image.h"

# define SQUARE 5 // Pour la taille du carre de selection.


typedef struct
{
	int x ; // L'abscisse.
	int y ; // L'ordonnee.
	int z ; // Z.
} Point ; // Les coordonnes d'un point.

typedef struct edge
{
	Point pMin ; // Exremite de l'arete donc l'ordonnee est inferieure ou egale a celle de l'autre extremite.
	Point pMax ; // Autre extremite.
	int xIntersection ; // Absisse de l'intersection avec le scan-line courant.
	int counter ; // Compteur permettant d'incrmenter l'absisse du point d'intersection.
} Edge ;

typedef struct polygon
{
	Point * p ; // Un point.
	struct polygon * next ; // Le point suivant.
} Polygon ; // Un ensemble de points.


/**
 * @brief  Creer un Point.
 * @param  xx L'abscisse.
 * @param  yy L'ordonnee.
 * @param  zz La coordonnee z.
 * @return Un point.
 */
Point * P_newPoint (int xx , int yy , int zz) ;
/**
 * @brief Mettre a jour les coordonnees du point.
 * @param p Le point.
 * @param x La nouvelle abscisse du point.
 * @param y La nouvelle ordonnee du point.
 * @param z La nouvelle coordonnee z.
 */
void P_setPoint (Point * p , int x , int y , int z) ;
/**
 * @brief Afficher les coordonnes d'un point.
 * @param p Le point.
 */
void P_printPoint (Point * p) ;
/**
 * @brief Liberer la memoire.
 * @param p Le point.
 */
void P_deletePoint (Point * p) ;


/**
 * @brief  Creer un nouveau polygone, constitue d'un point ou bien creer un point en fin de polygone.
 * @param  p Le premier point.
 * @return Un polygone.
 */
Polygon * P_newPolygon (Point * p) ;
/**
 * @brief  Retourner le premier point d'un polygone.
 * @param  p Le polygone.
 */
Point * P_firstVertex (Polygon * p) ;
/**
 * @brief  Retourner le dernier point d'un polygone.
 * @param  p Le polygone.
 */
Point * P_lastVertex (Polygon * p) ;
/**
 * @brief Afficher les coordonnes des sommets d'un polygone.
 * @param p Le polygone.
 */
void P_printPolygon (Polygon * p) ;
/**
 * @brief Ajouter un point au polygone. Le polygone doit exister.
 * @param pol Le polygone.
 * @param poi Le point.
 */
void P_addPoint (Polygon * pol , Point * poi) ;
/**
 * @brief Supprimer un point d'un polygone. Le polygone doit contenir au moins trois points.
 * @param image L'image sur laquelle est dessine le polygone.
 * @param pol   Le polygone.
 * @param poi   Le point a supprimer de pol.
 */
void P_deletePointFromPolygon (Image * image , Polygon * pol , Point * poi) ;
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
 * @brief Dessiner un petit carre autour du vertex selectionne.
 * @param p Un point.
 */
void P_vertexSelected (Point * p) ;
/**
 * @brief  Distance entre deux points.
 * @param  point1 Le premier point.
 * @param  point2 Le second point.
 * @result Distance cartesienne entre les points.
 */
float distanceBetweenVertices (Point * point1 , Point * point2) ;
/**
 * @brief  Trouver le point le plus proche appartenant a un polygone.
 * @param  pol Le polygone.
 * @param  poi Le point.
 * @result Le point le plus proche de poi dans pol.
 */
Point * P_closestVertex (Polygon * pol , Point * poi) ;


/************************ Vertex precedent ou suivant. ************************/

/**
 * @brief  Trouver le point precedent un point selectionne dans un polygone.
 * @param  pol Le polygone.
 * @param  poi Le point selectionne.
 * @return Le point precedent poi dans pol.
 */
Point * P_previousVertex (Polygon * pol , Point * poi) ;
/**
 * @brief  Trouver le point suivant un point selectionne dans un polygone.
 * @param  pol Le polygone.
 * @param  poi Le point selectionne.
 * @return Le point suivant poi dans pol.
 */
Point * P_nextVertex (Polygon * pol , Point * poi) ;

/******************************************************************************/


/***************************** Deplacer un point. *****************************/

/**
 * @brief Deplacer un point vers le haut.
 * @param image L'image ou se situe le polygone.
 * @param pol   Le polygone contenant le point.
 * @param p     Le point a modifier.
 */
void P_moveUp (Image * image , Polygon * pol , Point * p) ;
/**
 * @brief Deplacer un point vers le bas.
 * @param image L'image ou se situe le polygone.
 * @param pol   Le polygone contenant le point.
 * @param p     Le point a modifier.
 */
void P_moveDown (Image * image , Polygon * pol , Point * p) ;
/**
 * @brief Deplacer un point vers la droite.
 * @param image L'image ou se situe le polygone.
 * @param pol   Le polygone contenant le point.
 * @param p     Le point a modifier.
 */
void P_moveRight (Image * image , Polygon * pol , Point * p) ;
/**
 * @brief Deplacer un point vers la droite.
 * @param image L'image ou se situe le polygone.
 * @param pol   Le polygone contenant le point.
 * @param p     Le point a modifier.
 */
void P_moveLeft (Image * image , Polygon * pol , Point * p) ;

/******************************************************************************/


/**
 * @brief Tracer un polygone.
 * @param i L'image sur laquelle tracer le polygone.
 * @param p Le polygone.
 */
void P_draw (Image * i , Polygon * p) ;


# endif
