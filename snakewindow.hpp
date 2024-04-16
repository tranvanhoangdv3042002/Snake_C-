#ifndef SNAKEWINDOW_HPP
#define SNAKEWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.hpp"

class SnakeWindow : public QFrame // Une héritage de la classe QFrame de Qt
{
  protected:
    Jeu jeu;
    QPixmap pixmapCorps, pixmapTete, pixmapMur, pixmapVide;
    // QPixmap est une classe héritant de la classe "QPaintDevice" étant le classe de base d'objet
    // qui peut être dessiné avec "QPainter". "QPainter" founit les fonctions permettant de dessiner GUI

  public:
    SnakeWindow(QWidget *pParent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags()); //Qt::WindowFlags est le type de données (liste d'énumération),
    // comme Qt::Window, Qt::Dialog, Qt::Widget, Qt::Popup, Qt::Tool, Qt::SplashScreen, Qt::CustomizeWindowHint...
    // Qt::WindowFlags() : le constructeur par défaut (une fenêtre sans aucun attribut défini)
    void handleTimer(); // Permet de mettre à jout le jeu dan une intervalle de temps spécifique


  protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void handleTimer();
    void handleButtonAjout();
    void handleButtonSuppr();
    void supFruit();
    Position posFruit();
};

class SnakeButton : public QPushButton
{
  public:
    SnakeButton(QWidget *pParent=0):QPushButton(pParent) {}

  protected:
    void keyPressEvent(QKeyEvent *e)
    {
        if (parent()!=NULL)
            QCoreApplication::sendEvent(parent(), e);
    }
};

#endif
