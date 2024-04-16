#include <iostream>
#include "snakewindow.hpp"

using namespace std;

SnakeWindow::SnakeWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{ // les argurments "pParent" et "flags" sont passés au constructeur QWidget
    // en remarquant que if le parent = null_ptr, le widget deviendra une fenêtre pricipale,
    // sinon, il deviendra une fênêtre fille résidant la fenêtre pricipale.
    // If flags = Qt::WindowFlags(), le frame sera créé sans avoir le cadre

    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    if (pixmapCorps.load("./data/snake_corps.png")==false)
    {// La fonction membre "load" de la classe "APixmap" permet d'enregistrer l'image
        // dans la variable "pixmapCorps" si l'image est loaded avec le succès
        cout<<"Impossible d'ouvrir snake_corps.png"<<endl; // Afficher le message dans le cas d'échec
        exit(-1); // Arrêter immédiatement le programme
    }

    if (pixmapTete.load("./data/snake_tete.png")==false) // Similaire comme ci-dessus
    {
        cout<<"Impossible d'ouvrir snake_tete.png"<<endl;
        exit(-1);
    }

    if (pixmapMur.load("./data/mur.bmp")==false) // Similaire comme ci-dessus
    {
        cout<<"Impossible d'ouvrir mur.bmp"<<endl;
        exit(-1);
    }
    if (pixmapVide.load("./data/vide.png")==false)
    {
        cout << "Impossible d'ouvrir vide.png"<<endl;
        exit(-1);
    }

    jeu.init(); // Initialiser les attributs par défaut de la classe "jeu"
    // (comme la largeur, la hauteur de l'écran, les cases du terrain, la position et la direction du serpent...)

    QTimer *timer = new QTimer(this); // QTimer est une classe hériant de la classe "QObject"
    // qui permet de fournir l'interface pour le temps
    // Le constructeur âr défaut de QTimer : "QTimer::QTimer(QObject *parent = nullptr)"
    // Pour cette ligne de code, la pointeur "timer" est créé dont le type est QTimer
    // avec l'object courant pointé sur "this", dans ce cas c'est la fenêtre principale

    connect(timer, &QTimer::timeout, this, &SnakeWindow::handleTimer); // Lorsque le délais de temps est atteint, on va mettre a jour le jeu
    timer->start(100); // Démarrer (ou redémarrer si le temps est en train d'exécuter) le time avec le délais de 100 ms

    

    SnakeButton *btnAjout = new SnakeButton(this);
    btnAjout->setFixedSize(100, 25);
    btnAjout->setText("Ajout mur");
    btnAjout->move(10, 10);

    SnakeButton *btnSuppr = new SnakeButton(this);
    btnSuppr->setFixedSize(100, 25);
    btnSuppr->setText("Suppr mur");
    btnSuppr->move(120, 10);

    connect(btnAjout, &QPushButton::clicked, this, &SnakeWindow::handleButtonAjout);
    connect(btnSuppr, &QPushButton::clicked, this, &SnakeWindow::handleButtonSuppr);

    largeurCase = pixmapMur.width(); // Largeur de l'image MUR
    hauteurCase = pixmapMur.height(); // Hauteur de l'image MUR
	
    decalageY = 50;
    resize(jeu.getNbCasesX()*largeurCase, jeu.getNbCasesY()*hauteurCase+decalageY);
}

void SnakeWindow::paintEvent(QPaintEvent *) // La classe "QPaintEvent" appartient à la classe "QEvent"
{
    QPainter painter(this); // Appeller le constructeur "QPainter(QPaintDevice *device)" pour commencer de dessiner
                            // le "paint device" immédiatement
// Information : un "paint device" est un espace abstrait de 2D sur lequel on peut dessiner en utilisant QPainter
 // QPainter fournit des fonctions optimisées pour la plupart des requis de programmes GUI
 // En utilisant QPainter, l'on peut dessiner beaucoup de choses en 2 D comme les lignes, rectangles...

    Position pos;

    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    // Dessine les cases
        // L'idéé des lignes de codes ci-dessous est que l'on exécute toutes les positions de l'écran,
        // et si le case de la position = "MUR" on va dessiner à cette position (x,y sont obtenus grâce à sa position)
        // le MUR, et si le case de la position = "VIDE", on va dessiner le VIDE pour cette position.
    for (pos.y=0;pos.y<jeu.getNbCasesY();pos.y++)
        for (pos.x=0;pos.x<jeu.getNbCasesX();pos.x++)
			if (jeu.getCase(pos)==MUR)
                // La méthode "drawPixmap(int x, int y, const QPixmap &pixmap)" permet de dessiner le pixmap donné
                // à la position (x,y)
                painter.drawPixmap(pos.x*largeurCase, (pos.y)*hauteurCase, pixmapMur); // Pour dessiner le MUR
            else if (jeu.getCase(pos)==VIDE)
                painter.drawPixmap(pos.x*largeurCase, (pos.y)*hauteurCase, pixmapVide); // Pour dessiner le MUR)
            else if (jeu.getCase(pos)==FRUIT)
                //painter.drawPixmap((jeu.vecteurVide[jeu.numFruit]%jeu.getNbCasesX())*largeurCase,((jeu.vecteurVide[jeu.numFruit]-(jeu.vecteurVide[jeu.numFruit]%jeu.getNbCasesX())/jeu.getNbCasesX())*hauteurCase), pixmapFruit);
                  painter.drawPixmap(pos.x*largeurCase, pos.y*hauteurCase+decalageY, pixmapFruit);
       
	
    // Dessine le serpent
        // L'idée est le suivant : on prend la liste de direction du serpent en appellant la fonction "getSnake()" du jeu,
        // après, on va faire préalablement dessiner la tête du serpent en utilisant la fonction "front" de la liste,
        // en sachant que "snake" est une liste aui enregistre toutes les potions des partie du serpent. Ensuite, on va 
        //dessiner le coprs en utilisant l'itéraeur de "snake" qui pointe aux positions du corps
    const list<Position> &snake = jeu.getSnake();
    if (!snake.empty())
    {
        list<Position>::const_iterator itSnake;
        const Position &posTete = snake.front();
        painter.drawPixmap(posTete.x*largeurCase, posTete.y*hauteurCase+decalageY, pixmapTete); // Pour dessiner la tête


        for (itSnake=++snake.begin(); itSnake!=snake.end(); itSnake++)
            painter.drawPixmap(itSnake->x*largeurCase, itSnake->y*hauteurCase+decalageY, pixmapCorps); // Pour dessiner le corps
    }
    // Disparition du fruit


}

void SnakeWindow::keyPressEvent(QKeyEvent *event)// Permet de faire le déplacement du serpent
{
    if (event->key()==Qt::Key_Left)
        jeu.setDirection(GAUCHE);
    else if (event->key()==Qt::Key_Right)
        jeu.setDirection(DROITE);
    else if (event->key()==Qt::Key_Up)
        jeu.setDirection(HAUT);
    else if (event->key()==Qt::Key_Down)
        jeu.setDirection(BAS);
    update();
}

void SnakeWindow::handleTimer()  // Mettre a jour le serpent (la position)
{
    jeu.evolue(); // La direction (position) des perties du serpents va changer en focntion
            // de la valeur de la variable "dirSnake" de type "Direction", et la direction est
            // ensuite enregistrée dans la variable "snake" qui est une liste des positions
    update();
}
void SnakeWindow::handleButtonAjout()
{
    jeu.ajoutMur();
    update(); // permet de mettre à jour le widget (sauf le widget est caché)
}

void SnakeWindow::handleButtonSuppr()
{
    jeu.suppressionMur();
    update();
}
void SnakeWindow::supFruit()
{

    if (jeu.getSnake().front()==posFruit())
    {
        jeu.terrain[posFruit().x*15+posFruit().y]= VIDE;
        //update();

    }

    update();
}

 Position SnakeWindow::posFruit()
 {
     Position pos;
      for (pos.y=0;pos.y<jeu.getNbCasesY();pos.y++)
        for (pos.x=0;pos.x<jeu.getNbCasesX();pos.x++)
            if (jeu.getCase(pos)==FRUIT)
                return pos;

 }
