
/*===============================================================*\

    Vendredi 25 septembre 2013

	Arash Habibi

	main.c

	Un programme equivalent à 02_glut.c et qui ne prend en compte
	que trois événements pour quitter le programme.

\*===============================================================*/

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>


# include "Polygon.h"

/*********************************** Mode. ************************************/
enum enum_mode {APPEND , VERTEX , EDGE} ;
typedef enum enum_mode e_mode ;
/******************************************************************************/

Image *img;
Polygon * p = NULL ; // Pour l'instant le polygone est vide.
Point * pointSelected = NULL ;
Edge * edgeSelected = NULL ;

int closed = FALSE ; // Le polygone est ouvert.
int filled = FALSE ; // Le polygone est vide.
e_mode mode = APPEND ; // Par defaut, mode "append".
int selectionVertex = FALSE ; // Si un point est selectionne, pour le mode vertex.
int selectionEdge = FALSE ; // Si un point est selectionne, pour le mode vertex.
BoundingBox * boundingBox = NULL ; // Pour le remplissage.

//------------------------------------------------------------------
//	C'est le display callback. A chaque fois qu'il faut
//	redessiner l'image, c'est cette fonction qui est
//	appelee. Tous les dessins doivent etre faits a partir
//	de cette fonction.
//------------------------------------------------------------------

void display_CB()
{
    glClear(GL_COLOR_BUFFER_BIT);

	P_draw (img , p) ; // Tracer le polygone.
	if ((mode == VERTEX) && (selectionVertex == TRUE))
	{
		P_vertexSelected (pointSelected) ; // Tracer un marqueur autour du point selectionne.
	}
	else if ((mode == EDGE) && (selectionEdge == TRUE))
	{
		P_edgeSelected (edgeSelected) ; // Colorer l'arete selectionnee.
	}
	else if (filled == TRUE)
	{
		if (p != NULL)
		{
			Polygon * p0 = p ; // Pour parcourir le polygone.
			Point vertices [P_numberVertices (p)] ; // Tableau contenant les points du polygone.
	
			int i = 0 ;
			while (p0 != NULL) // Parcourir le polygone.
			{
				vertices [i] = * (p0 -> p) ; // Prendre le point.
				
				i ++ ;
				p0 = p0 -> next ;
			}
			boundingBox = P_createBoundingBox (p) ; // Mettre a jour la bounding box.
			Color color = C_new (0 , 0.5 , 1) ; // Choisir la couleur pour remplir le polygone.
			P_fill (img , i , vertices , closed , color , boundingBox) ; // Remplir le polygone.
			P_draw (img , p) ; // Tracer le polygone.
		}
	}
	
    glutSwapBuffers();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir en fonction de la position de
// la souris (x,y), en fonction du bouton de la souris qui a été
// pressé ou relaché.
//------------------------------------------------------------------

void mouse_CB(int button, int state, int x, int y)
{
	// Point * bin = pointSelected ;
	Point * point = P_newPoint (x , (img -> _height) - y , 0) ; // Creer un point d'apres le clic de souris, en arrangeant la coordonnee de y.	
	// P_deletePoint (bin) ;
	
	if((button==GLUT_LEFT_BUTTON) && (state==GLUT_DOWN) && (mode == APPEND)) // En mode "append".
	{	
		selectionVertex = FALSE ; // On ne selectionne aucun point.
		selectionEdge = FALSE ; // On ne selectionne aucune arete.
		if (p == NULL) // Si le polygone est vide.
		{
			p = P_newPolygon (point) ; // Creer un nouveau polygone.
		}
		else
		{
			P_addPoint (p , point) ; // Ajouter le point au polygone.
			P_open (img , p) ; // Ouvrir le polygone.
			closed = FALSE ; // Polygone ouvert.
		}
		I_focusPoint(img,x,img->_height-y);
	}
	else if ((button==GLUT_LEFT_BUTTON) && (state==GLUT_DOWN) && (mode == VERTEX)) // En mode "vertex".
	{
		selectionVertex = TRUE ; // Un point a ete selectionne.
		pointSelected = P_closestVertex (p , point) ; // Point du polygone.
	}
	else if ((button==GLUT_LEFT_BUTTON) && (state==GLUT_DOWN) && (mode == EDGE)) // En mode "edge".
	{
		selectionEdge = TRUE ; // Une arete a ete selectionnee.
		edgeSelected = P_closestEdge (p , point , closed) ; // Trouver l'arete selectionnee.
	}
	else if ((button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN) && (closed == FALSE) && (selectionEdge == TRUE)) // Clique avec le bouton du milieu de la souris. Le polygone doit etre ouvert. Une arete doit etre selectionnee.
	{
		P_insertVertex (p , edgeSelected) ; // Inserer un sommet au milieu de l'arete.
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche (non-spéciale) du clavier.
//------------------------------------------------------------------

void keyboard_CB(unsigned char key, int x, int y)
{
	// fprintf(stderr,"key=%d\n",key);
	switch(key)
	{
		case 27 : exit(1); break;
		case 'a' : // Mode "append". Chaque clic ajoute un point au polygone.
		{
			mode = APPEND ; // Affecter la valeur du mode.
			if (closed == FALSE)
			{
				selectionVertex = FALSE ;
				selectionEdge = FALSE ;
			}
			else
			{
				selectionVertex = FALSE ;
				selectionEdge = FALSE ;
			}
			break ;
		}
		case 'c' : // Ouvrir ou fermer le polygone.
		{
			if (closed == TRUE) // Si le polygone etait ferme, on l'ouvre.
			{
				closed = FALSE ; // Le polygone est maintenant ouvert.
				P_open (img , p) ; // Ouvrir le polygone.
			}
			else // Si le polygone etait ouvert, on le ferme.
			{
				closed = TRUE ;
				P_close (img , p) ; // Fermer le polygone.
			}
			break ;
		}
		case 'e' : // Mode "edge". Une seule arete est selectionnee.
		{
			mode = EDGE ; // Affecter la valeur du mode.
			selectionVertex = FALSE ;
			break ;
		}
		case 'f' : // Remplir ou vider le polygone.
		{
			if (closed == TRUE)
			{
				if (filled == TRUE)
				{
					filled = FALSE ;
					P_close (img , p) ;
				}
				else
				{
					filled = TRUE ;
					P_close (img , p) ;
				}
			}
			selectionVertex = FALSE ;
			selectionEdge = FALSE ;
			break ;
		}
		case 'i' : I_zoomInit(img); break;
		case 'r' : // Faire un reset.
		{
			p = P_reset (img , p) ;
			P_open (img , p) ;
			mode = APPEND ;
			selectionVertex = FALSE ;
			selectionEdge = FALSE ;
			filled = FALSE ;
			pointSelected = NULL ;
			edgeSelected = NULL ;
			closed = FALSE ; // Le polygone est ouvert.
			break ;
		}
		case 'v' : // Mode "vertex". Un seul sommet est selectionne.
		{
			mode = VERTEX ; // Affecter la valeur du mode.
			selectionEdge = FALSE ;
			if (closed == TRUE) {P_close (img , p) ;}
			break ;
		}
		case 'z' : I_zoom(img,2.0); break;
		case 'Z' : I_zoom(img,0.5); break;
		case 127 : // Suppr.
		{
			if ((mode == VERTEX) && (selectionVertex == TRUE))
			{
				p = P_deletePointFromPolygon (img , p , pointSelected) ; 
		
				if (p != NULL)
				{
					pointSelected = P_closestVertex (p , pointSelected) ;
				}
				else
				{
					mode = APPEND ;
					pointSelected = NULL ;
				}
			}
			break ;
		}
		default : fprintf(stderr,"keyboard_CB : %d : unknown key.\n",key);
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche spéciale (F1, F2 ... F12, home, end, insert,
// haut, bas, droite, gauche etc).
//------------------------------------------------------------------

void special_CB(int key, int x, int y)
{
	// int mod = glutGetModifiers();

	int d = 10;

	switch(key)
	{
		case GLUT_KEY_UP :
		{
			if (mode == APPEND) // Par defaut.
			{
				I_move(img,0,d);
			}
			else if ((mode == VERTEX) && (selectionVertex == TRUE)) // En mode "vertex".
			{
				closed = FALSE ;
				P_moveUp (img , p , P_closestVertex (p , pointSelected)) ;
			}
			break;
		}
		case GLUT_KEY_DOWN :
		{
			if (mode == APPEND) // Par defaut.
			{
				I_move(img,0,-d);
			}
			else if ((mode == VERTEX) && (selectionVertex == TRUE)) // En mode "vertex".
			{
				closed = FALSE ;
				P_moveDown (img , p , P_closestVertex (p , pointSelected)) ;
			}
			break;
		}
		case GLUT_KEY_LEFT :
		{
			if (mode == APPEND) // Par defaut.
			{
				I_move(img,d,0);
			}
			else if ((mode == VERTEX) && (selectionVertex == TRUE)) // En mode "vertex".
			{
				closed = FALSE ;
				P_moveLeft (img , p , P_closestVertex (p , pointSelected)) ;
			}
			break;
		}
		case GLUT_KEY_RIGHT :
		{
			if (mode == APPEND) // Par defaut.
			{
				I_move(img,-d,0);
			}
			else if ((mode == VERTEX) && (selectionVertex == TRUE)) // En mode "vertex".
			{
				closed = FALSE ;
				P_moveRight (img , p , P_closestVertex (p , pointSelected)) ;
			}
			break;
		}
		case 104 : // Page precedente.
		{
			if ((mode == VERTEX) && (pointSelected != NULL)) // Dans le mode "vertex", et il faut qu'un point soit selectionne.
			{
				pointSelected = P_previousVertex (p , P_closestVertex (p , pointSelected) , closed) ; // Selectionner le point precedent.
			}
			else if ((mode == EDGE) && (edgeSelected != NULL)) // Dans le mode "edge", il faut qu'une arete soit selectionnee.
			{
				edgeSelected = P_previousEdge (p , edgeSelected , closed) ; // Selectionner l'arete precedente.
			}
			break ;
		}
		case 105 : // Page suivante.
		{
			if ((mode == VERTEX) && (pointSelected != NULL)) // Dans le mode "vertex", et il faut qu'un point soit selectionne.
			{
				pointSelected = P_nextVertex (p , P_closestVertex (p , pointSelected) , closed) ; // Selectionner le point suivant.
			}
			else if ((mode == EDGE) && (edgeSelected != NULL)) // Dans le mode "edge", il faut qu'une arete soit selectionnee.
			{
				edgeSelected = P_nextEdge (p , edgeSelected , closed) ; // Selectionner l'arete suivante.
			}
			break ;
		}
		default : fprintf(stderr,"special_CB : %d : unknown key.\n",key);
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if((argc!=3)&&(argc!=2))
	{
		fprintf(stderr,"\n\nUsage \t: %s <width> <height>\nou",argv[0]);
		fprintf(stderr,"\t: %s <ppmfilename> \n\n",argv[0]);
		exit(1);
	}
	else
	{
		int largeur, hauteur;
		if(argc==2)
		{
			img = I_read(argv[1]);
			largeur = img->_width;
			hauteur = img->_height;
		}
		else
		{
			largeur = atoi(argv[1]);
			hauteur = atoi(argv[2]);
			img = I_new(largeur,hauteur);
			// Color rouge = C_new(100,0,0);
			// Color blanc = C_new(200,200,255);
			// I_checker(img,rouge,blanc,50);
		}
		
		boundingBox = P_createBoundingBox (p) ;
		
		int windowPosX = 100, windowPosY = 100;

		glutInitWindowSize(largeur,hauteur);
		glutInitWindowPosition(windowPosX,windowPosY);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE );
		glutInit(&argc, argv);
		glutCreateWindow(argv[0]);

		glViewport(0, 0, largeur, hauteur);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glOrtho(0,largeur,0,hauteur,-1,1);

		glutDisplayFunc(display_CB);
		glutKeyboardFunc(keyboard_CB);
		glutSpecialFunc(special_CB);
		glutMouseFunc(mouse_CB);
		// glutMotionFunc(mouse_move_CB);
		// glutPassiveMotionFunc(passive_mouse_move_CB);

		glutMainLoop();

		return 0;
	}
}
