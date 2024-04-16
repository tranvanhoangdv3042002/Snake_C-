#include <ctime>
#include "snakewindow.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])// "argc" est l'argument count, ça sert à compter le nombre d'arguments
// dans le command-ligne qui sont passés au program lorsqu'il est exécuté
// "*argv[]" est le tableau dont les éléments sont des pointeurs sur "char" et ces pointeurs, ils pointent
// des noms d'arguments et le premier pointeur pointe le nom du programme exécuté.
{
    QApplication app(argc, argv); // QApplication est une classe, dans ce cas, cette fonction permet d'initialiser
    // le système de window et de construire un object d'application avec la commande ligne argc dans argv

    srand(time(0)); // time(0) retourne le nombre de secondes à partir de 00:00 heurs, 01/01/1970 UTC
                    // Donc, lorsque l'on lance le programme, l'ensemble de nombres aléatoires est généré
    SnakeWindow wnd; // Une variable de type SnakeWindow crée par défaut
    wnd.show(); // QWidget::show() permet de représenter le widget et son widget fils

    return app.exec(); // appartenant à la classe "QApplication", la fonction membre statique permet
                    // d'exécuter le boucle d'événement jusqu'à la fonction "exit()" est appelée
}


