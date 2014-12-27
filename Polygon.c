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

void P_setPoint (Point * p , int x , int y , int z)
{
	p -> x = x ;
	p -> y = y ;
	p -> z = z ;
}

void P_printPoint (Point * p)
{
	if (p != NULL)
	{
		printf ("(%d , %d , %d)\n" , p -> x , p -> y , p -> z) ; // Afficher les coordonnes du point.
	}
}

void P_deletePoint (Point * p)
{
	free (p) ;
}

Polygon * P_newPolygon (Point * pp)
{
	Polygon * result = (Polygon *) malloc (sizeof (Polygon)) ; // Allouer l'espace memoire.
	result -> p = pp ; // Mettre le point dans le polygone.
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
	I_bresenham (i , p_t -> p -> x , p_t -> p -> y , p -> p -> x , p -> p -> y) ; // Tracer une droite de Bresenham entre le dernier et le premier point.
}

void P_open (Image * i , Polygon * p)
{	
	I_fill (i , C_new (0 , 0 , 0)) ; // Peindre l'image en noir.
	P_draw (i , p) ; // Tracer le polygone.
}

void P_vertexSelected (Point * p)
{
	if (p != NULL)
	{
		glColor3d (1 , 1 , 1) ; // Couleur du carre.
		
		glBegin (GL_LINE_LOOP) ; // Tracer des lignes.
			glVertex3d (p -> x - SQUARE , p -> y - SQUARE , 0) ;
			glVertex3d (p -> x + SQUARE , p -> y - SQUARE , 0) ;
			glVertex3d (p -> x + SQUARE , p -> y + SQUARE , 0) ;
			glVertex3d (p -> x - SQUARE , p -> y + SQUARE , 0) ;
		glEnd () ;
	}
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

Point * P_closestVertex (Polygon * pol , Point * poi)
{
	Point * result  = NULL ;
	
	float distance  = -1 ; // Valeur de depart.
	float distance0 = -1 ;
	Polygon * pol0  = pol ; // Pour parcourir le polygone.
	while (pol0 != NULL) // Tant qu'il y a des sommets dans le polygone.
	{
		distance0 = distanceBetweenVertices (poi , pol0 -> p) ;
		if (distance == -1) // Premiere distance calculee.
		{
			distance = distance0 ;
			result = pol0 -> p ; // Conserver le point.
		}
		else if (distance0 <= distance) // Si l'on a trouve un point plus proche.
		{
			distance = distance0 ; // Conserver la nouvelle distance.
			result = pol0 -> p ; // Conserver le point.
		}
		
		pol0 = pol0 -> next ; // Passer au sommet suivant.
	}
	
	return result ; // Retourner le point le plus proche de poi.
}

Point * P_previousVertex (Polygon * pol , Point * poi)
{
	Polygon * pol0 = pol ; // Pour parcourir les points du polygone.
	
	if ((pol -> p != poi) && (pol != NULL)) // Si le point n'est pas le premier.
	{
		while (pol0 -> next != NULL) // Parcourir le polygone.
		{
			if (pol0 -> next -> p == poi) // Si le point selectionne est le suivant.
			{
				return pol0 -> p ; // Retourner le point courant.
			}
			
			pol0 = pol0 -> next ; // Point suivant.
		}
	}
	
	return poi ;
}

// Point * P_nextVertex (Polygon * pol , Point * poi)
// {
	// Polygon * pol0 = pol ; // Pour parcourir les points du polygone.
	// 
	// if ((pol -> p != poi) && (pol != NULL)) // Si le point n'est pas le premier.
	// {
		// while (pol0 -> next != NULL) // Parcourir le polygone.
		// {
			// if ((pol0 -> next -> p -> x == poi -> x) && (pol0 -> next -> p -> y == poi -> y)) // Si le point selectionne est le suivant.
			// {
				// return pol0 -> p ; // Retourner le point courant.
			// }
			// 
			// pol0 = pol0 -> next ; // Point suivant.
		// }
	// }
	// 
	// return poi ;
// }

void P_draw (Image * i , Polygon * pp)
{
	if (pp != NULL) // Si le polygone contient quelque chose.
	{
		Polygon * p_t = pp ; // Polygone temporaire.
		while (p_t -> next != NULL) // Tant qu'il reste des points dans le polygone.
		{
			I_bresenham (i , p_t -> p -> x , p_t -> p -> y ,
							 p_t -> next -> p -> x , p_t -> next -> p -> y) ; // Tracer une droite de Bresenham entre le point courant et le suivant.
			p_t = p_t -> next ; // Regarder le point suivant.
		}
	}
}
