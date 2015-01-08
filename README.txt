Projet de Fondements et Algorithmes de l'Imagerie Numérique
Isabelle Muller
M1 ISI
Janvier 2015


Utilisation du programme
- pour compiler, entrer la commande "make"
- pour nettoyer, "make clean"
- pour lancer le programme directement dans une fenetre de dimensions 800 x 600,
entrer "make run"

Fonctionnalites du programme
- tracer un polygone en créant les points en faisant des clics gauche
- 'c' : fermer/ouvrir le polygone
- 'f' : remplir/vider le polygone
- 'a' : mode "append" -> pour ajouter des points au polygone
- 'v' : mode "vertex" -> pour selectionner un point du polygone
- 'e' : mode "edge"   -> pour selectionner une arete

Differents modes
- append : a chaque clic, un nouveau sommet est ajoute au polygone.
- vertex : chaque clic selectionne un sommet du polygone.
En cliquant sur "page precedente" ou "page suivante", on selectionne le sommet
precedent ou suivant.
Les fleches permettent de deplacer un sommet selectionne.
La touche "suppr" permet de supprimer un sommet du polygone.
- edge : claque clic selectionne une arete du polygone.
En cliquant sur "page precedente" ou "page suivante", on selectionne l'arete
precedente ou suivante.
En cliquant avec le bouton central de la souris sur une arete selectionnee, on
rejoute un sommet au milieu de cette arete.
