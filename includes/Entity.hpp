#pragma once
#include <ncurses.h>
#include <Board.hpp>

class Entity
{
protected:  //protected per essere accessibili alle sottoclassi
    Board * board;    //finestra corrente
    int x, y;           //posizione x,y
    chtype c;     //char usato

    void construct(Board * board, int yc, int xc, chtype c);


public:

    Entity();
    Entity(Board * board, int yc, int xc, chtype c);


    void display();

    void initialize();

    int getycoord();
    int getxcoord();
    chtype getChar();
    void setX(int n);
    void setY(int n);
    void setWindow(WINDOW *win);
    
};