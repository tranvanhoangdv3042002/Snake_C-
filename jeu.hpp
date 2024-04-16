#ifndef JEU_HPP
#define JEU_HPP
#include <vector>
#include <list>

typedef enum {VIDE, MUR, FRUIT} Case;
typedef enum {GAUCHE, DROITE, HAUT, BAS} Direction;

class Position
{
  public:
    int x, y;
    Position();
    Position(int,int);
    bool operator==(const Position &) const;
    bool operator!=(const Position &) const;
};

class Jeu
{
  protected:
    //Case *terrain;
    int largeur, hauteur; // Nombre de cases en largeur et en hauteur
   std::list<Position> snake;  // c'est une liste
   // std::vector<int> vecteurVide;
    //int numFruit;

    //std::list<posVide> position;
    Direction dirSnake;

  public:
    Jeu(); // Le constructeur par défaut
    Jeu(const Jeu &); // Le constructeur de recopie
    ~Jeu();

   Case *terrain;
    Jeu &operator=(const Jeu &);

    bool init(); // Initialiser les valeurs des attributs du jeu
    void evolue(); // Mettre à jour le jeu en changeant la position du serpent
    std::vector<int> vecteurVide;
    int numFruit;

    // Retourne les dimensions (en nombre de cases)
    int getNbCasesX() const;
    int getNbCasesY() const;


    // Retourne la case à une position donnée
    Case getCase(const Position &) const;

    // Retourne la liste des éléments du serpent en lecture seule
    const std::list<Position> &getSnake() const;

    // Indique si la case à une position donnée existe et est libre
    bool posValide(const Position &) const;

    // Modifie la direction
    void setDirection(Direction);

    void ajoutMur();
    void suppressionMur();

    //


};

#endif
