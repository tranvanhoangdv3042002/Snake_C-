#include <iostream>
#include <cassert>
#include "jeu.hpp"
#include <vector>

using namespace std;

Position::Position()
{
}

Position::Position(int a, int b) // Créer un constructeur de la postition
{
    x=a;
    y=b;
}

bool Position::operator==(const Position &pos) const // L'ajoute de la surcharge '==' pour la position
{
    return (x==pos.x && y==pos.y);
}

bool Position::operator!=(const Position &pos) const // L'ajoute de la surcharge '!=' pour la position
{
    return (x!=pos.x || y!=pos.y);
}

Jeu::Jeu() // Initialiser le contructeur de la classe Jeu
// Avec le terrain ="nullptr" désignant le pointeur nullpart
{
    terrain = nullptr; // Type "case" en notant que le terrain est le pointeur
    largeur = 0; hauteur = 0; // La dimension de l'écran en nombre de pixels
    dirSnake = DROITE; // Type "Direction"
}

Jeu::Jeu(const Jeu &jeu):snake(jeu.snake) // Mettre en valeur des paramètres (initialiser  avec le constructeur ayant des paramètres)
// Le paramètre est la classe "jeu", avec l'attribut est le "snake" de type "list"
{
    largeur = jeu.largeur;// C'est le constructeur de recopie, l'argument peut donc accéder à
    hauteur = jeu.hauteur; // des attributs protégés
    dirSnake = jeu.dirSnake; // Type de "direction"

    if (jeu.terrain!=nullptr) // si le pointeur "terrain" n'est pas null_pointeur
    {
        terrain = new Case[largeur*hauteur]; // On crée une tableau dynamique pour contenir les cases d'un terrain 2D de jeu
        for (int c=0; c<largeur*hauteur; c++) // On crée un terrain 2D de jeu (pour chaque case, soit le mur soit vide)
            terrain[c] = jeu.terrain[c];     // en notant que tout d'abord, on crée les cases sur la première ligne, et ensuite la 2e linge...
    }                                       // (ctd, dans le sens du gauche à droite, de haut en bas)
    else
        terrain = nullptr; // Si le pointeur "jeu.terrain" est null_ponteur, le terrain pointe à null_pointeur
}

eu::~Jeu() // Destructeur de la classe "Jeu"
{
    if (terrain!=nullptr) // Il faut détruire le tableau dynamique auquel le pointeur "terrain" pointe
        delete[] terrain; // dans le cas où le pointeur est non_null
}

Jeu &Jeu::operator=(const Jeu &jeu) // L'opération de l'affectation de la classe "Jeu"
{
    if (terrain!=nullptr) // On supprimera d'abord le tableau "terrain" si le terrain est non_null
        delete[] terrain;// ça sert à créer un nouveau terrain (on supprime l'ancien)

    largeur = jeu.largeur;
    hauteur = jeu.hauteur;
    dirSnake = jeu.dirSnake;
    snake = jeu.snake; // "snake" est une lise de position de chaque élement constituant le serpent (la tête, le corps)

    if (jeu.terrain!=nullptr) // On va faire une affectation de le terrain de jeu si le jeu.terrain n'est pas null
    {
        terrain = new Case[largeur*hauteur];
        for (int c=0; c<largeur*hauteur; c++)
            terrain[c] = jeu.terrain[c];
    } // Ces codes ci-dessus permet de créer le tableau dynamique "terrain" contenant tous les cases
     // de l'autre tableau qui est un attribur protégé de l'argument.
    else
        terrain = nullptr; //Sinon, le pointeur null est appliqué
    return *this; // Retourner le pointeur actuel car on voulait retourner les valeurs de la variable de type "Jeu"
}


bool Jeu::init() //Initialiser le "Jeu" en utilisant la fonction membre
{
	int x, y;
	// list<Position>::iterator itSnake;

	const char terrain_defaut[15][21] = { // On remarque la dimension du terrain n'est que 15x20 mais
	    // on devrait créer le tableau de taille 15x21 car il faut prendre en compte la caractère de fin "\0"
	    // pour chaque chaine de caractère
		"####..##############",
		"#........##........#",
		"#.#####..##...####.#",
		"#........##........#",
		"#..................#",
		"#..................#",
		"....................",
		"....................",
		"....................",
		"....................",
		"#..................#",
		"#..................#",
		"#.....#......#.....#",
		"#.....#......#.....#",
        "####..##############"
    };

	largeur = 20;
	hauteur = 15;

	terrain = new Case[largeur*hauteur]; // Un nouveau terrain sera défini par le tableau dynamique
// sachant que le terrain en un pointeur de type "Case" (la déclaration est déjà fait "Case *terrain")
	for(y=0;y<hauteur;++y)
		for(x=0;x<largeur;++x)
            if (terrain_defaut[y][x]=='#')
                terrain[y*largeur+x] = MUR; // Pour créer le MUR et le VIDE par défaut
            else
                terrain[y*largeur+x] = VIDE;
// L'idée des lignes de codes ci-dessus réside du fait que l'on crée un tableau "terrain" 1D qui contient l'état
// de chaque case du terrain, soit le MUR soit le VIDE en fonction du tableau "terrain_defaut"en 2D.


	
           /*
	    else
                {
                    terrain[y*largeur+x] = VIDE;
                    vecteurVide.push_back(y*largeur+x);
                }
                //for (int j=0; j<largeur; j++){
              }     // for (int j=0; j<largeur; j++){
                            //posVide.push_back(y*largeur+x)

               numFruit=rand()%vecteurVide.size();
                terrain[vecteurVide[numFruit]]=FRUIT;
*/



   int longueurSerpent = 5; // Définir longueur initiale du serpent
    snake.clear(); // Suprprimer le serpent ancien et préparer de démarrer le serpent actuel

    Position posTete; // Position (en 2D) intiale par défaut
    posTete.x = 15;
    posTete.y = 8;
	for (int i=0; i<longueurSerpent; i++) // Cette boucle permet d'établir la position initial des
        // parties du serpent en gardant la position y et en diminuant la position x une case.
    {
        snake.push_back(posTete); // Définir les positions du serpent (en diminuant une case suivant x
        posTete.x--;              // lorsque l'on rajouter un élément du corps du serpent)
    }

    return true;
}

void Jeu::evolue()
{
    Position posTest;
	list<Position>::iterator itSnake; // C'est un intérateur qui est utilisé ensuite

    int depX[] = {-1, 1, 0, 0, 0}; // Chaque numéro correspond à l'augmentation de la coordonnée suivant x
                                // nous, on a rajouté le 5ème numéro comme "0" pour faire le serpent s'arrêter
    int depY[] = {0, 0, -1, 1, 0}; // Similairement comme ci-dessus mais suivant y (par exemple, -1 désigne
                                    // un recul 1 case suivant y (déplacer en haut)

    posTest.x = snake.front().x + depX[dirSnake]; // La position x de la tête augmente/diminue 1 case lorsque
                                            //la direction du serpent déplace à droite/ à gauche resp (rien de changer la psotion y)
    posTest.y = snake.front().y + depY[dirSnake];//La position y de la tête augmente/diminue 1 case lorsque
                                            //la direction du serpent déplace en bas/ en haut resp (rien de changer la position x)

    if (posValide(posTest)) // S'il n'y a pas de collision avec le corps du serpent ou les murs et le serpent est
                        //dans la zône de terrain, on supprimera la queue du serpent et on rajoute la tête pour le déplacement
    {
        snake.pop_back(); // Supprimer la position de la queue
        snake.push_front(posTest); // Ajouter la nouvelle position du tête lors que
                                // l'on change la variable "dirSnake" de type Direction
    }
}

int Jeu::getNbCasesX() const
{
    return largeur;// Obtenir la largeur du terrain équivalente au nombre de cases suivant x
}


int Jeu::getNbCasesY() const
{
    return hauteur;  // Obtenir la hauteur du terrain équivalente au nombre de cases suivant y
}

Case Jeu::getCase(const Position &pos) const
{
    assert(pos.x>=0 && pos.x<largeur && pos.y>=0 && pos.y<hauteur); // Retouner le case pointé soit MUR soit VID
    return terrain[pos.y*largeur+pos.x];
}

const list<Position> &Jeu::getSnake() const
{
    return snake; // Obtenir la liste d la position des éléments du serpent
}

bool Jeu::posValide(const Position &pos) const
{
    if (pos.x>=0 && pos.x<largeur && pos.y>=0 && pos.y<hauteur
        && terrain[pos.y*largeur+pos.x]==VIDE)// La condition nécessaire de déplacement du serpent :
        // il faut que le serpent déplace sur la zone vide (pas de mur) et dans la frontière du terrain de l'écran
    {
        list<Position>::const_iterator itSnake;
        itSnake = snake.begin();
        while (itSnake!=snake.end() && *itSnake!=pos) // Condition de bloquer le serpent : la tête touche le corps
            itSnake++;
        return (itSnake==snake.end()); // Si itSnake == snake.end, ctd, la tête ne touche pas le corps pour la nouvelle position
    }                                   // Sinon, pour le nouveau déplacement, la tête va toucher le corps ==> Le serpent est ploqué
    else
        return false;
}

void Jeu::setDirection(Direction dir)
{
    dirSnake = dir; // Obtenir la direction pour l'élément quelconque du serpent
}

// Ajoute d'un mur
void Jeu::ajoutMur()
{
    Position posMur;

    // Trouve une case libre
    /* Tout d'abord, on va créer aléatoirement une postion
	 Ensuite, on vérifie que si cette position est vide ou n'est pas supervisée
	sur le serpent, on va affectuer cette postion à un mur, sinon, on continue à prendre aléatoirement une autre position
     */
    do {
        posMur.x = rand()%largeur;
        posMur.y = rand()%hauteur;
    } while (!posValide(posMur));
    terrain[posMur.y*largeur+posMur.x]=MUR;
}

//Suppression d'un mur
    /* Tout d'abord, on va choisir aléatoirement une postion
	 Ensuite, on vérifie que si cette position est un mur,
 	 on va affectuer cette postion à une VIDE,
   	sinon, on continue à prendre aléatoirement une autre position
     */
void Jeu::suppressionMur()
{
    Position posMur;

    // Trouve un mur
    do {
        posMur.x = rand()%largeur;
        posMur.y = rand()%hauteur;
    } while (terrain[posMur.y*largeur+posMur.x]!=MUR);
    terrain[posMur.y*largeur+posMur.x]=VIDE;
}




