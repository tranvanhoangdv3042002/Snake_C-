#include <iostream>
#include "snakewindow.hpp"

using namespace std;

SnakeWindow::SnakeWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    if (pixmapCorps.load("./data/snake_corps.png")==false)
    {
        cout<<"Impossible d'ouvrir snake_corps.png"<<endl;
        exit(-1);
    }

    if (pixmapTete.load("./data/snake_tete.png")==false)
    {
        cout<<"Impossible d'ouvrir snake_tete.png"<<endl;
        exit(-1);
    }

    if (pixmapMur.load("./data/mur.bmp")==false)
    {
        cout<<"Impossible d'ouvrir mur.bmp"<<endl;
        exit(-1);
    }

    jeu.init();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SnakeWindow::handleTimer); // Lorsque le délais de temps est atteint, on va mettre a jour le jeu
    timer->start(100); // Démarrer le time avec le délais de 100 ms

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    resize(jeu.getNbCasesX()*largeurCase, jeu.getNbCasesY()*hauteurCase); // "Resize" fonction appartient à la classe "QWidget"
}           // Cette fonction permet de redimensionner la taille de l'écran du jeu


void SnakeWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    Position pos;

    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    // Dessine les cases
    for (pos.y=0;pos.y<jeu.getNbCasesY();pos.y++)
        for (pos.x=0;pos.x<jeu.getNbCasesX();pos.x++)
			if (jeu.getCase(pos)==MUR)
                painter.drawPixmap(pos.x*largeurCase, pos.y*hauteurCase, pixmapMur); // Pour dessiner le MUR
    // Dessine le serpent
    const list<Position> &snake = jeu.getSnake();
    if (!snake.empty())
    {
        list<Position>::const_iterator itSnake;
        const Position &posTete = snake.front();
        painter.drawPixmap(posTete.x*largeurCase, posTete.y*hauteurCase, pixmapTete); // Pour dessiner la tête

        for (itSnake=++snake.begin(); itSnake!=snake.end(); itSnake++) // Pour dessiner le corps
            painter.drawPixmap(itSnake->x*largeurCase, itSnake->y*hauteurCase, pixmapCorps);
    }
}

void SnakeWindow::keyPressEvent(QKeyEvent *event) // Permet de faire le déplacement du serpent
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
    jeu.evolue();
    update();
}
void SnakeWindow::handleButtonAjout()
{
    jeu.ajoutMur();
    update();
}

void SnakeWindow::handleButtonSuppr()
{
    jeu.suppressionMur();
    update();
}
