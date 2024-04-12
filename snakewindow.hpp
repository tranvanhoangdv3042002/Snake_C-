#ifndef SNAKEWINDOW_HPP
#define SNAKEWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.hpp"

class SnakeWindow : public QFrame // Une héritage de la classe QFrame de Qt
{
  protected:
    Jeu jeu;
    QPixmap pixmapCorps, pixmapTete, pixmapMur;

  public:
    SnakeWindow(QWidget *pParent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
   //Qt::WindowFlags est le type de données (liste d'énumération),
    // comme Qt::Window, Qt::Dialog, Qt::Widget, Qt::Popup, Qt::Tool, Qt::SplashScreen, Qt::CustomizeWindowHint...
    // Qt::WindowFlags() : le constructeur par défaut (une fenêtre sans aucun attribut défini)
void handleTimer();

  protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void handleButtonAjout();
    void handleButtonSuppr();
    clickMonBouton();
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
