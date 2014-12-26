
/*===============================================================*\

    Vendredi 25 septembre 2013

	Arash Habibi

	main.c

	Un programme equivalent à 02_glut.c et qui ne prend en compte
	que trois événements pour quitter le programme.

\*===============================================================*/

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>

# include "Polygon.h"

/********************************* Bouleens. **********************************/
# define FALSE 0
# define TRUE  1
/*********************************** Mode. ************************************/
# define APPEND 2
# define VERTEX 3
# define EDGE   4
/******************************************************************************/

Image *img;
Polygon * p = NULL ; // Pour l'instant le polygone est vide.
Point * pointSelected = NULL ;

int closed = FALSE ; // Le polygone est ouvert.
int mode = APPEND ; // Par defaut, mode "append".
int selection = FALSE ; // Si un point est selectionne, pour le mode vertex.

//------------------------------------------------------------------
//	C'est le display callback. A chaque fois qu'il faut
//	redessiner l'image, c'est cette fonction qui est
//	appelee. Tous les dessins doivent etre faits a partir
//	de cette fonction.
//------------------------------------------------------------------

void display_CB()
{
    glClear(GL_COLOR_BUFFER_BIT);
    	
	I_draw(img);
	P_draw (img , p) ; // Tracer le polygone.
	if ((mode == VERTEX) && (selection == TRUE))
	{
		P_vertexSelected (P_closestVertex (p , pointSelected)) ; // Tracer un marqueur autour du point selectionne.
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
	pointSelected = P_newPoint (x , (img -> _height) - y , 0) ; // Creer un point d'apres le clic de souris, en arrangeant la coordonnee de y.	
	// P_deletePoint (bin) ;
	
	if((button==GLUT_LEFT_BUTTON) && (state==GLUT_DOWN) && (mode != VERTEX) &&
														   (mode != EDGE)) // En mode "append".
	{	
		selection = FALSE ; // On ne selectionne rien.
		if (p == NULL) // Si le polygone est vide.
		{
			p = P_newPolygon (pointSelected) ; // Creer un nouveau polygone.
		}
		else
		{
			P_addPoint (p , pointSelected) ; // Ajouter le point au polygone.
		}
		I_focusPoint(img,x,img->_height-y);
	}
	else if ((button==GLUT_LEFT_BUTTON) && (state==GLUT_DOWN) && (mode == VERTEX)) // En mode "vertex".
	{
		selection = TRUE ; // Un point a ete selectionne.
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
			break ;
		}
		case 'f' : // Remplir ou vider le polygone.
		{
		}
		case 'i' : I_zoomInit(img); break;
		case 'v' : // Mode "vertex". Un seul sommet est selectionne.
		{
			mode = VERTEX ; // Affecter la valeur du mode.
			break ;
		}
		case 'z' : I_zoom(img,2.0); break;
		case 'Z' : I_zoom(img,0.5); break;
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
	case GLUT_KEY_UP    : I_move(img,0,d); break;
	case GLUT_KEY_DOWN  : I_move(img,0,-d); break;
	case GLUT_KEY_LEFT  : I_move(img,d,0); break;
	case GLUT_KEY_RIGHT : I_move(img,-d,0); break;
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
