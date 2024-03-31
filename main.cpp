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
    // le système de window et de construire un object d'application

    srand(time(0));

    SnakeWindow wnd;
    wnd.show();

    return app.exec(); // Execution du programme
}



