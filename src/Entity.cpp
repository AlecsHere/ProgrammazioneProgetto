#include "../includes/Entity.hpp"

void Entity::construct(Board * board, int yc, int xc, chtype c){
    this->board = board;
    this->y = yc;
    this->x = xc;
    this->c = c;
}

Entity::Entity(){
    construct(NULL, -1, -1, '');  //Costruttore base
};
Entity::Entity(Board * board, int yc, int xc, chtype c){
    construct(board, yc, xc, c);
};

void Entity::display(){
    mvwaddch(board->getWindow(), y, x, c);  //Mostra l'entità in questione
}

void Entity::initialize(){
    refresh();
}

//Funzioni che ritronano variabili dell'entità

int Entity::getycoord(){
    return y;
}
int Entity::getxcoord(){
    return x;
}
chtype Entity::getChar(){
    return c;
}

//Funzioni che impostano variabili dell'entità

void Entity::setX(int n){
    x=n;
}
void Entity::setY(int n){
    y=n;
}
void Entity::setWindow(WINDOW *win){
    board->setWindow(win);
}