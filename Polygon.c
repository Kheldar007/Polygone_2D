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

Point * P_firstVertex (Polygon * p)
{
	if (p != NULL) // Si p existe.
	{
		return p -> p ; // Retourner le premier point de p.
	}
	return NULL ;
}

Point * P_lastVertex (Polygon * p)
{
	if (p != NULL) // Si p existe.
	{
		Polygon * p_t = p ; // Pour parcourir le polygone.
		while (p_t -> next != NULL) // Chercher le dernier sommet.
		{
			p_t = p_t -> next ; // Point suivant.
		}
		return p_t -> p ; // Retourner le dernier point de p.
	}
	return NULL ;
}

void P_printPolygon (Polygon * p)
{
	if (p != NULL) // Si le polygone n'est pas vide.
	{
		int vertices = 0 ;
		Polygon * p_t = p ; // Pour parcourir les sommets.
		while (p_t != NULL)
		{
			vertices ++ ;
			printf ("Sommet %d " , vertices) ;
			P_printPoint (p -> p) ; // Afficher les coordonnes du sommet courant.
			
			p_t = p_t -> next ; // Sommet suivant.
		}
		printf ("\n") ;
	}
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

void P_deletePointFromPolygon (Image * image , Polygon * pol , Point * poi)
{
	if ((pol != NULL) && (poi != P_firstVertex (pol)) && (poi != P_lastVertex (pol))) // Le polygone doit exister et l'on ne peut pas supprimer le premier ou le dernier point.
	{
		if (pol -> p == poi) // Point au debut.
		{
			if (pol -> next == NULL) // Le polygone n'est constitue que d'un point.
			{
				Polygon * bin = pol ;
				pol = NULL ; // Supprimer le point.
				free (bin) ;
				P_open (image , pol) ;
			}
			else
			{
				Polygon * bin = pol ;
				pol = pol -> next ; // Supprimer le premier point.
				free (bin) ;
				P_open (image , pol) ;
			}
		}
		else
		{
			Polygon * p_t = pol ; // Polygone temporaire.
			while ((p_t -> next != NULL) && (p_t != NULL)) // Tant qu'il reste des points dans le polygone.
			{
				if (p_t -> next -> p == poi) // Le point a ete trouve.
				{
					Polygon * bin = p_t -> next ;
					p_t -> next = p_t -> next -> next ; // Supprimer le point.
					free (bin) ;
					P_open (image , pol) ;
				}
				
				p_t = p_t -> next ; // Regarder le point suivant.
			}
		}
	}
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
	if ((point1 != NULL) && (point2 != NULL)) // Les deux points doivent exister.
	{
		float xBMinusXA = point2 -> x - point1 -> x ; // xB - xA
		float yBMinusYA = point2 -> y - point1 -> y ; // yB - yA
		
		xBMinusXA *= xBMinusXA ; // Au carre.
		yBMinusYA *= yBMinusYA ; // Au carre.
		
		float sum = xBMinusXA + yBMinusYA ; // Faire la somme.
		
		return sqrt (sum) ; // Racine carree de l'ensemble.
	}
	else
	{
		return -1 ; // S'il y a une erreur.
	}
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

Point * P_previousVertex (Polygon * pol , Point * poi , int closed)
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
	else if ((poi == P_firstVertex (pol)) && (closed == TRUE)) // Si c'est le premier point et que le polygone est ferme.
	{
		return P_lastVertex (pol) ; // Retourner le dernier point.
	}
	
	return poi ;
}

Point * P_nextVertex (Polygon * pol , Point * poi , int closed)
{
	Polygon * pol0 = pol ; // Pour parcourir les points du polygone.
	
	if ((pol != NULL) && (pol -> next != NULL)) // Si le point n'est pas le dernier.
	{
		while (pol0 -> next != NULL) // Parcourir le polygone.
		{
			if (pol0 -> p == poi) // Si le point selectionne est le courant.
			{
				return pol0 -> next -> p ; // Retourner le point suivant.
			}
			
			pol0 = pol0 -> next ; // Point suivant.
		}
		if (closed == TRUE) // Si le polygone est ferme.
		{
			return P_firstVertex (pol) ; // Dans le cas du dernier point, retourner le premier point.
		}
	}
	
	return poi ;
}

void P_moveUp (Image * image , Polygon * pol , Point * poi)
{
	if (poi != NULL)
	{
		if (poi -> y < image -> _height - 1) // Si le point n'est pas deja tout en haut.
		{
			Polygon * pol0 = pol ; // Pour parcourir les points du polygone.
			
			while (pol0 != NULL) // Parcourir le polygone.
			{
				if (pol0 -> p == poi) // Le point a ete trouve.
				{
					P_setPoint (pol0 -> p , pol0 -> p -> x , pol0 -> p -> y + 1 , pol0 -> p -> z) ; // Deplacer le point vers le haut.
					P_open (image , pol) ;
				}
				pol0 = pol0 -> next ; // Deplacer le pointeur
			}
		}
	}
}

void P_moveDown (Image * image , Polygon * pol , Point * poi)
{
	if (poi != NULL)
	{
		if (poi -> y > 0) // Si le point n'est pas deja tout en bas.
		{
			Polygon * pol0 = pol ; // Pour parcourir les points du polygone.
			
			while (pol0 != NULL) // Parcourir le polygone.
			{
				if (pol0 -> p == poi) // Le point a ete trouve.
				{
					P_setPoint (pol0 -> p , pol0 -> p -> x , pol0 -> p -> y - 1 , pol0 -> p -> z) ; // Deplacer le point vers le haut.
					P_open (image , pol) ;
				}
				pol0 = pol0 -> next ; // Deplacer le pointeur
			}
		}
	}
}

void P_moveRight (Image * image , Polygon * pol , Point * poi)
{
	if (poi != NULL)
	{
		if (poi -> x < image -> _width - 1) // Si le point n'est pas deja tout a droite.
		{
			Polygon * pol0 = pol ; // Pour parcourir les points du polygone.
			
			while (pol0 != NULL) // Parcourir le polygone.
			{
				if (pol0 -> p == poi) // Le point a ete trouve.
				{
					P_setPoint (pol0 -> p , pol0 -> p -> x + 1 , pol0 -> p -> y , pol0 -> p -> z) ; // Deplacer le point vers le haut.
					P_open (image , pol) ;
				}
				pol0 = pol0 -> next ; // Deplacer le pointeur
			}
		}
	}
}

void P_moveLeft (Image * image , Polygon * pol , Point * poi)
{
	if (poi != NULL)
	{
		if (poi -> x > 0) // Si le point n'est pas deja tout a gauche.
		{
			Polygon * pol0 = pol ; // Pour parcourir les points du polygone.
			
			while (pol0 != NULL) // Parcourir le polygone.
			{
				if (pol0 -> p == poi) // Le point a ete trouve.
				{
					P_setPoint (pol0 -> p , pol0 -> p -> x - 1 , pol0 -> p -> y , pol0 -> p -> z) ; // Deplacer le point vers le haut.
					P_open (image , pol) ;
				}
				pol0 = pol0 -> next ; // Deplacer le pointeur
			}
		}
	}
}

Edge * P_closestEdge (Polygon * pol , Point * poi , int closed)
{
	if ((pol != NULL) && (pol -> next != NULL)) // Si le polygone existe et qu'il y a plus d'un point.
	{
		Edge * result = (Edge *) malloc (sizeof (Edge)) ; // Allouer la memoire.
		
		Point * p1 = P_closestVertex (pol , poi) ; // Le point le plus proche sera l'un des points de l'arete.
		Point * p2 ;
		
		/********* Choisir l'arete entre les deux adjacentes au point choisi. *********/
		float distance1 = distanceBetweenVertices (poi , P_previousVertex (pol , p1 , closed)) ;
		float distance2 = distanceBetweenVertices (poi , P_nextVertex (pol , p1 , closed)) ;
		
		if (p1 == P_firstVertex (pol)) // Si le point est le premier du polygone.
		{
			distance1 = distanceBetweenVertices (poi , P_lastVertex (pol)) ;
		}
		else if (p1 == P_lastVertex (pol)) // Si le point est le dernier du polygone.
		{
			distance2 = distanceBetweenVertices (poi , P_firstVertex (pol)) ;
		}
		if (distance1 <= distance2) // Le point precedent est plus proche.
		{
			if (p1 == P_firstVertex (pol)) // Si le point est le premier du polygone.
			{
				p2 = P_lastVertex (pol) ; // Prendre le dernier point.
			}
			else // Cas general.
			{
				p2 = P_previousVertex (pol , p1 , closed) ; // Prendre le point precedent.
			}
		}
		else // Le point suivant est plus proche.
		{
			if (p1 == P_lastVertex (pol)) // Si le point est le dernier du polygone.
			{
				p2 = P_firstVertex (pol) ; // Prendre le premier point.
			}
			else // Cas general.
			{
				p2 = P_nextVertex (pol , p1 , closed) ; // Prendre le point precedent.
			}
		}
		/******************************************************************************/
		
		if (p1 -> y <= p2 -> y) // p1 est en-dessous de p2.
		{
			result -> pMin = p1 ;
			result -> pMax = p2 ;
		}
		else // p1 est au-dessus de p2.
		{
			result -> pMin = p2 ;
			result -> pMax = p1 ;
		}
		
		return result ; // Retourner l'arete.
	}
	
	return NULL ;
}

void P_edgeSelected (Edge * e)
{
	if (e != NULL)
	{
		glColor3d (0 , 0 , 1) ; // Couleur bleue.		
		glBegin (GL_LINE_LOOP) ; // Tracer une ligne de couleur sur l'arete selectionnee.
			glVertex3d (e -> pMin -> x , e -> pMin -> y , e -> pMin -> z) ;
			glVertex3d (e -> pMax -> x , e -> pMax -> y , e -> pMax -> z) ;
		glEnd () ;
	}
}

Edge * P_previousEdge (Polygon * p , Edge * e , int closed)
{
	Edge * result = (Edge *) malloc (sizeof (Edge)) ; // Allouer la memoire.
	
	if (((e -> pMin == P_firstVertex (p)) || (e -> pMax == P_firstVertex (p))) && (closed == FALSE)) // Polygone ouvert.
	{
		return e ; // Arete courante.
	}
	else
	{
		// Trouver les deux points delimitant la nouvelle arete.
		Point * p1 = P_previousVertex (p , e -> pMin , closed) ;
		Point * p2 = P_previousVertex (p , e -> pMax , closed) ;
		
		if (p1 -> y <= p2 -> y) // p1 est en-dessous de p2.
		{
			result -> pMin = p1 ;
			result -> pMax = p2 ;
		}
		else // p1 est au-dessus de p2.
		{
			result -> pMin = p2 ;
			result -> pMax = p1 ;
		}
	}
	
	return result ; // Retourner l'arete trouvee.
}

Edge * P_nextEdge (Polygon * p , Edge * e , int closed)
{
	Edge * result = (Edge *) malloc (sizeof (Edge)) ; // Allouer la memoire.
	
	if (((e -> pMin == P_lastVertex (p)) || (e -> pMax == P_lastVertex (p))) && (closed == FALSE)) // Polygone ouvert.
	{
		return e ; // Arete courante.
	}
	else
	{
		// Trouver les deux points delimitant la nouvelle arete.
		Point * p1 = P_nextVertex (p , e -> pMin , closed) ;
		Point * p2 = P_nextVertex (p , e -> pMax , closed) ;
		
		if (p1 -> y <= p2 -> y) // p1 est en-dessous de p2.
		{
			result -> pMin = p1 ;
			result -> pMax = p2 ;
		}
		else // p1 est au-dessus de p2.
		{
			result -> pMin = p2 ;
			result -> pMax = p1 ;
		}
	}
	
	return result ; // Retourner l'arete trouvee.
}

void P_insertVertex (Polygon * p , Edge * e)
{
	if ((p != NULL) && (e != NULL))
	{
		Polygon * p_t = p ; // Pour parcourir le polygone.
		while ((p_t != NULL) && ((p_t -> p != e -> pMin) && (p_t -> p != e -> pMax))) // Parcourir le polygone.
		{			
			p_t = p_t -> next ; // Sommet suivant.
		}
		// p_t est adjacent a l'arete.
		/************************** Creer le nouveau point. ***************************/
		Point * newPoint = (Point *) malloc (sizeof (Point)) ;
		newPoint -> x = ((float) (e -> pMin -> x + e -> pMax -> x)) / 2 ;
		newPoint -> y = ((float) (e -> pMin -> y + e -> pMax -> y)) / 2 ;
		newPoint -> z = ((float) (e -> pMin -> z + e -> pMax -> z)) / 2 ;
		Polygon * newPolygon = (Polygon *) malloc (sizeof (Polygon)) ;
		newPolygon -> p = newPoint ;
		newPolygon -> next = p_t -> next ;
		p_t -> next = newPolygon ;
		/******************************************************************************/
	}
}

BoundingBox * P_createBoundingBox (Polygon * p)
{
	BoundingBox * result = (BoundingBox *) malloc (sizeof (BoundingBox)) ; // Allocation memoire.
	
	if (p != NULL) // Si le polygone n'est pas vide.
	{
		Polygon * p_t = p ; // Pour parcourir le polygone.
		int xMin0 = p -> p -> x , yMin0 = p -> p -> y , xMax0 = p -> p -> x ,
			yMax0 = p -> p -> y ; // Coordonnes de la bounding box.
		while (p_t != NULL)
		{
			if (p_t -> p -> x < xMin0)
			{
				xMin0 = p_t -> p -> x ;
			}
			if (p_t -> p -> y < yMin0)
			{
				yMin0 = p_t -> p -> y ;
			}
			if (p_t -> p -> x > xMax0)
			{
				xMax0 = p_t -> p -> x ;
			}
			if (p_t -> p -> y > yMax0)
			{
				yMax0 = p_t -> p -> y ;
			}
			
			p_t = p_t -> next ; // Deplacer le pointeur.
		}
		result -> xMin = xMin0 ;
		result -> xMax = xMax0 ;
		result -> yMin = yMin0 ;
		result -> yMax = yMax0 ;
	}
	else
	{
		result -> xMin = 0 ;
		result -> xMax = 0 ;
		result -> yMin = 0 ;
		result -> yMax = 0 ;
	}
	return result ;
}

int P_numberVertices (Polygon * p)
{
	int result = 0 ; // Resultat.
	
	if (p != NULL) // Si le polygone existe.
	{
		Polygon * p_t = p ; // Polygone temporaire.
		while (p_t != NULL)
		{
			result ++ ; // Un point de plus.
			
			p_t = p_t -> next ; // Point suivant.
		}
	}
	return result ; // Retourner le nombre de sommets.
}

void insertEdge (Edge * edges , Edge * newEdge) 
{
	Edge * e_t ;
	Edge * e_t2 = edges ;
	e_t = e_t2 -> next ; // Prendre l'arete suivante.
	while (e_t != NULL) // Tant qu'il y a des aretes.
	{
		if (newEdge -> xIntersection < e_t -> xIntersection) // Voir pour le sens.
		{
			e_t = NULL ; // Vider le pointeur.
		}
		else 
		{
			e_t2 = e_t ; // Replacer le pointeur.
			
			e_t = e_t -> next ; // Deplacer le pointeur
		}
	}
	newEdge -> next = e_t2 -> next ; // Placer l'arete au bon endroit.
	e_t2 -> next = newEdge ;
}

void makeEdgeRec (int yComp , Edge * e , Edge * edges [])
{
	e -> counter = (((float) (e -> pMax -> x - e -> pMin -> x)) /
					((float) (e -> pMax -> y - e -> pMin -> y))) ;
	e -> xIntersection = e -> pMin -> x ;
	if (e -> pMax -> y < yComp)
	{
		e -> yUpper = e -> pMax -> y - 1 ;
	}
	else
	{
		e -> yUpper = e -> pMax -> y ;
	}
	insertEdge (edges [e -> pMin -> y] , e) ; // Ajouter l'arete.
}

int yNext (int y , int nbVertices , Point * points) 
{
	int i ;
	if ((y + 1) > (nbVertices - 1))
	{
		i = 0 ;
	}
	else
	{
		i = y + 1 ;
	}
	while (points[y].y == points[i].y)
	{
		if ((i + 1) > (nbVertices - 1))
		{
			i = 0 ;
		}
		else
		{
			i ++ ;
		}
	}
	return (points[i].y);
}

void buildEdgeList (int nbVertices , Point * points , Edge * edges []) 
{
	Edge * newEdge ;
	Point vertice1 , vertice2 ;
	int yOld = points [nbVertices - 2].y ;
	vertice1.x = points [nbVertices - 1].x ;
	vertice1.y = points [nbVertices - 1].y ;
	int i = 0 ;
	while (i < nbVertices) 
	{
		vertice2 = points [i] ;
		if (vertice1.y != vertice2.y) // Si l'arete n'est pas horizontale.
		{
			newEdge = (Edge *) malloc (sizeof (Edge)) ; // Allouer la memoire.
			if (vertice1.y < vertice2.y)  // L'arete est croissante.
			{
				newEdge -> pMin = & vertice1 ; // Point en bas.
				newEdge -> pMax = & vertice2 ; // Point en haut.
				makeEdgeRec (yNext (i , nbVertices , points) , newEdge , edges) ;
			}
			else // L'arete est decroissante.
			{
				newEdge -> pMin = & vertice2 ; // Point en bas.
				newEdge -> pMax = & vertice1 ; // Point en haut.
				makeEdgeRec (yOld , newEdge , edges) ;
			}
		}
		yOld = vertice1.y ; // Mettre a jour.
		vertice1 = vertice2 ;
		
		i ++ ;
	}
}

void P_fill (Image * image , int vertices, Point * points , int closed ,
			 Color color , BoundingBox * bb) 
{
	if (closed == TRUE) // Il faut que le polygone soit ferme.
	{
		Edge * edges [image -> _height] ; // Aretes.
		Edge * active ; // Arete active.
		int i = 0 ;
		while (i < image -> _height) // Parcourir l'image.
		{
			edges [i] = (Edge *) malloc (sizeof (Edge)) ; // Allouer la memoire.
			edges [i] -> next = NULL ; // Pas d'aretes reliees entre elles.
			
			i ++ ;
		}
		
		buildEdgeList (vertices , points , edges) ; // Remplir les aretes.
		active = (Edge *) malloc (sizeof (Edge)) ; // Allouer la memoire.
		active -> next = NULL ; // Pas de suivante.
		i = 0 ;
		Edge * e_t , * e_t2 ; // Aretes temporaire.
		while (i < image -> _height) // Parcourir l'image.
		{			
			while (edges[i] -> next != NULL) // Examiner l'arete suivante.
			{
				e_t = edges [i] -> next -> next ; // Pour deplacer le pointeur.
				
				insertEdge (active , edges[i] -> next) ; // Inserer l'arete dans la liste des actives.
				edges[i] -> next = e_t ; // Deplacer le pointeur.
			}
			if (active -> next != NULL)  // Si l'on a trouve d'autres aretes actives.
			{
				Color current = image -> _current_color ; // Stocker la couleur courante associee a l'image.
				image -> _current_color = color ; // Affecter la couleur choisie au polygone.			
				e_t = active -> next ; // Regarder l'arete suivante, dans les actives.
				while (e_t != NULL) // Tant qu'il y en a.
				{
					e_t2 = e_t -> next ; // Regarder celle qui suit.
					float j = e_t -> xIntersection ; // A l'intersection d'une arete.
					while (j < e_t2 -> xIntersection) // Tant que l'on n'intersecte pas une autre arete.
					{
						I_plot (image , j , i) ; // Colorer le pixel.
						
						j ++ ;
					}
					
					e_t = e_t2 -> next ; // Deplacer le pointeur.
				}			
				image -> _current_color = current ; // Reaffecter la couleur courante de l'image de base.
				
				e_t = active ; // Replacer le pointeur.
				e_t2 = active -> next ; // Replacer le pointeur.
				while (e_t2 != NULL) // Tant que l'on n'est pas arrive au bout.
				{
					if (i >= e_t2 -> yUpper) // Chercher une arete non active.
					{
						e_t2 = e_t2 -> next ; // Deplacer le pointeur.
						
						Edge * bin = e_t -> next ; // Pointeur poubelle.
						e_t -> next = bin -> next ; // Deplacer un pointeur.
						free (bin) ; // Liberer le pointeur poubelle.
					} 
					else 
					{
						e_t2 -> xIntersection = e_t2 -> xIntersection + e_t2 -> counter ; // Mettre a jour l'intersection.
						e_t = e_t2 ; // Modifier le pointeur.
						
						e_t2 = e_t2 -> next ; // Deplacer le pointeur.
					}
				}
				
				e_t2 = active -> next ; // Replacer le pointeur.
				active -> next = NULL ; // Liberer l'arete active suivante.
				while (e_t2 != NULL)  // Tant qu'il y a des aretes.
				{
					e_t = e_t2 -> next ; // Replacer le pointeur.
					
					insertEdge (active , e_t2) ; // Inserer l'arete dans la liste des actives.
					e_t2 = e_t ; // Replacer le pointeur.
				}
			}
			i ++ ;
		}
	}
}

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
