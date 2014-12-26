/**
 * @file    Polygon.c
 * @brief   Implementer les methodes du fichier "Polygon.h".
 * @author  Isabelle Muller
 */


# include "Polygon.h"


Point * P_newPoint (int xx , int yy , int zz)
{
	Point * result = (Point *) malloc (sizeof (Point)) ; // Point a renvoyer.
	result -> x = xx ; // Lui mettre son abscisse.
	result -> y = yy ; // Lui mettre son ordonnee.
	result -> z = zz ; // Lui mettre sa coordonne z.
	return result ; // Renvoyer le point.
}

void P_printPoint (Point * p)
{
	printf ("(%d , %d , %d)\n" , p -> x , p -> y , p -> z) ; // Afficher les coordonnes du point.
}

Polygon * P_newPolygon (Point * pp)
{
	Polygon * result = (Polygon *) malloc (sizeof (Polygon)) ; // Allouer l'espace memoire.
	result -> p = * pp ; // Mettre le point dans le polygone.
	result -> next = NULL ; // Pas d'autre point pour le moment.
	return result ; // Renvoyer le polynome.
}

void P_addPoint (Polygon * pol , Point * poi)
{
	Polygon * p_t = pol ; // Polygone temporaire.
	while (p_t -> next != NULL) // Tant qu'il reste des points dans le polygone.
	{
		p_t = p_t -> next ; // Regarder le point suivant.
	}
	Polygon * newPolygon = P_newPolygon (poi) ; // Creer une structure et mettre le point dedans.
	p_t -> next = newPolygon ; // Ajouter le point au polygone.
}

void P_close (Image * i , Polygon * p)
{
	P_open (i , p) ; // Nettoyer l'image afin de redessiner au propre.
	
	Polygon * p_t = p ; // Polygone temporaire.
	while (p_t -> next != NULL) // Tant qu'il reste des points dans le polygone.
	{
		p_t = p_t -> next ; // Regarder le point suivant.
	}
	// Maintenant le dernier point est atteint.
	I_bresenham (i , p_t -> p.x , p_t -> p.y , p -> p.x , p -> p.y) ; // Tracer une droite de Bresenham entre le dernier et le premier point.
}

void P_open (Image * i , Polygon * p)
{	
	I_fill (i , C_new (0 , 0 , 0)) ; // Peindre l'image en noir.
	P_draw (i , p) ; // Tracer le polygone.
}

void P_vertexSelected (Point * p)
{
	glColor3d (1 , 1 , 1) ; // Couleur du carre.
	
	glBegin (GL_LINE_LOOP) ; // Tracer des lignes.
		glVertex3d (p -> x - SQUARE , p -> y - SQUARE , 0) ;
		glVertex3d (p -> x + SQUARE , p -> y - SQUARE , 0) ;
		glVertex3d (p -> x + SQUARE , p -> y + SQUARE , 0) ;
		glVertex3d (p -> x - SQUARE , p -> y + SQUARE , 0) ;
	glEnd () ;
}

float distanceBetweenVertices (Point * point1 , Point * point2)
{
	float xBMinusXA = point2 -> x - point1 -> x ; // xB - xA
	float yBMinusYA = point2 -> y - point1 -> y ; // yB - yA
	
	xBMinusXA *= xBMinusXA ; // Au carre.
	yBMinusYA *= yBMinusYA ; // Au carre.
	
	float sum = xBMinusXA + yBMinusYA ; // Faire la somme.
	
	return sqrt (sum) ; // Racine carree de l'ensemble.
}

Point * closestVertex (Polygon * pol , Point * poi)
{
	float distance = -1 ; // Valeur de depart.
	Polygon * pol0 = pol ; // Pour parcourir le polygone.

	// Point *closest = (Point*)malloc(sizeof(Point));
	// Polygon *tmp = polygon;
	// Polygon *first = polygon;
	// int min;
// 
	// closest->x = tmp->pt->x;
	// closest->y = tmp->pt->y;
// 
	// min = sqrt(pow(tmp->pt->x-x, 2) + pow(tmp->pt->y-y, 2)); // distance entre 2 points
// 
	// tmp = tmp->next;
// 
	// while(tmp->pt->x != first->pt->x || tmp->pt->y != first->pt->y)
	// {
		// if(sqrt(pow(tmp->pt->x-x, 2) + pow(tmp->pt->y-y, 2)) < min)
		// {
			// min = sqrt(pow(tmp->pt->x-x, 2) + pow(tmp->pt->y-y, 2));
			// closest->x = tmp->pt->x;
			// closest->y = tmp->pt->y;
		// }
		// tmp = tmp->next;
	// }
	// 
	// return closest;
}

void P_draw (Image * i , Polygon * pp)
{
	if (pp != NULL) // Si le polygone contient quelque chose.
	{
		Polygon * p_t = pp ; // Polygone temporaire.
		while (p_t -> next != NULL) // Tant qu'il reste des points dans le polygone.
		{
			I_bresenham (i , p_t -> p.x , p_t -> p.y ,
							 p_t -> next -> p.x , p_t -> next -> p.y) ; // Tracer une droite de Bresenham entre le point courant et le suivant.
			p_t = p_t -> next ; // Regarder le point suivant.
		}
	}
}
