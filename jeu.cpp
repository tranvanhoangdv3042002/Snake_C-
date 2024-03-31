#include <iostream>
#include <cassert>
#include "jeu.hpp"

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
{
    terrain = nullptr; // Type "case" en notant que le terrain est le pointeur
    largeur = 0; hauteur = 0;
    dirSnake = DROITE; // Type "Direction"
}

Jeu::Jeu(const Jeu &jeu):snake(jeu.snake) // Mettre en valeur des paramètres (initialiser  avec le constructeur ayant des paramètres
{
    largeur = jeu.largeur;
    hauteur = jeu.hauteur;
    dirSnake = jeu.dirSnake;

    if (jeu.terrain!=nullptr) // si on le pointeur "terrain" n'est pas null_part
    {
        terrain = new Case[largeur*hauteur]; // On crée une tableau dunamique pour contenir les cases d'un terrain 2D de jeu
        for (int c=0; c<largeur*hauteur; c++) // On crée un terrain 2D de jeu (pour chaque case, soit le mur soit vide)
            terrain[c] = jeu.terrain[c];     // en notant que tout d'abord, on crée les cases sur la première ligne, et ensuite la 2e linge...
    }                                       // (ctd, dans le sens du gauche à droite, de haut en bas)
    else
        terrain = nullptr; // Si le pointeur "jeu.terrain" est null_part, le terrain pointe à null_part
}

Jeu::~Jeu() // Destructeur de la classe "Jeu"
{
    if (terrain!=nullptr) // Il faut détruire le tableau dynamique auquel le pointeur "terrain" pointe
        delete[] terrain; // dans le cas où le pointeur est non_null
}

Jeu &Jeu::operator=(const Jeu &jeu) // L'opération de l'affectation de la classe "Jeu"
{
    if (terrain!=nullptr) // On supprimera d'abord le tableau "terrain" si le le terrain est non_null
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
    }
    else
        terrain = nullptr; //Sinon, le pointeur null est appliqué
    return *this; // Retourner le pointeur actuel car on voulait retourner les valeurs de la variable de type "Jeu"
}

bool Jeu::init() //Initialiser le "Jeu" en utilisant la fonction membre
{
	int x, y;
	// list<Position>::iterator itSnake;

	const char terrain_defaut[15][21] = {
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

	for(y=0;y<hauteur;++y)
		for(x=0;x<largeur;++x)
            if (terrain_defaut[y][x]=='#')
                terrain[y*largeur+x] = MUR; // Pour créer le MUR et le VIDE par défaut
            else
                terrain[y*largeur+x] = VIDE;

    int longueurSerpent = 5; // Définir longueur initiale du serpent
    snake.clear(); // Redémarrer le serpent initiale

    Position posTete; // Position intiale par défaut
    posTete.x = 15;
    posTete.y = 8;
	for (int i=0; i<longueurSerpent; i++)
    {
        snake.push_back(posTete); // Définir les positions du serpent (en diminuant une case suivant x
        posTete.x--;              // lorsque l'on rajouter un élément du corps du serpent)
    }

    return true;
}

void Jeu::evolue()
{
    Position posTest;
	list<Position>::iterator itSnake;

    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};

    posTest.x = snake.front().x + depX[dirSnake]; // La position x de la tête augmente/diminue 1 case lorsque
                                            //la direction du serpent déplace à droite/ à gauche resp (rien de changer la psotion y)
    posTest.y = snake.front().y + depY[dirSnake];//La position y de la tête augmente/diminue 1 case lorsque
                                            //la direction du serpent déplace en bas/ en haut resp (rien de changer la psotion x)

    if (posValide(posTest)) // S'il n'y a pas de collision avec le corps du serpent ou les murs et le serpent est
                        //dans la zône de terrain, on supprimera la queue du serpent et on rajoute la tête pour le déplacement
    {
        snake.pop_back();
        snake.push_front(posTest);
    }
}

int Jeu::getNbCasesX() const
{
    return largeur;// Obtenir la largeur du terrain équivalente au nombre de cases suivant x
}
}

int Jeu::getNbCasesY() const
{
    return hauteur;  // Obtenir la hauteur du terrain équivalente au nombre de cases suivant y
}

Case Jeu::getCase(const Position &pos) const
{
    assert(pos.x>=0 && pos.x<largeur && pos.y>=0 && pos.y<hauteur); // Retouner le case pointé soit MUR soit VIDe
    return terrain[pos.y*largeur+pos.x];
}

const list<Position> &Jeu::getSnake() const
{
    return snake; // Obtenir la liste d la position des éléments du serpent
}

bool Jeu::posValide(const Position &pos) const
{
    if (pos.x>=0 && pos.x<largeur && pos.y>=0 && pos.y<hauteur
        && terrain[pos.y*largeur+pos.x]==VIDE)// La condition de déplacement  du serpent
        // il faut que le serpent dépalce sur la zône vide ( pas de mur) et sur le terrain de l'éceran
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
