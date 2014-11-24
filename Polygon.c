/**
 * @file    Polygon.c
 * @brief   Implementer les methodes du fichier "Polygon.h".
 * @author  Isabelle Muller
 */


# include "Polygon.h"


Point P_newPoint (int xx , int yy , int zz)
{
	Point result ; // Point a renvoyer.
	result.x = xx ; // Lui mettre son abscisse.
	result.y = yy ; // Lui mettre son ordonnee.
	result.z = zz ; // Lui mettre sa coordonne z.
	return result ; // Renvoyer le point.
}

void P_printPoint (Point p)
{
	printf ("(%d , %d , %d)\n" , p.x , p.y , p.z) ; // Afficher les coordonnes du point.
}

Polygon * P_newPolygon (Point pp)
{
	Polygon * result = (Polygon *) malloc (sizeof (Polygon)) ; // Allouer l'espace memoire.
	result -> p = pp ; // Mettre le point dans le polygone.
	result -> next = NULL ; // Pas d'autre point pour le moment.
	return result ; // Renvoyer le polynome.
}

void P_addPoint (Polygon * pol , Point poi)
{
	Polygon * p_t = pol ; // Polygone temporaire.
	while (p_t -> next != NULL) // Tant qu'il reste des points dans le polygone.
	{
		p_t = p_t -> next ; // Regarder le point suivant.
	}
	Polygon * newPolygon = P_newPolygon (poi) ; // Creer une structure et mettre le point dedans.
	p_t -> next = newPolygon ; // Ajouter le point au polygone.
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
