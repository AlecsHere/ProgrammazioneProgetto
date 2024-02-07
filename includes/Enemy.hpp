#pragma once
#include <ncurses.h>
#include "Board.hpp"
#include "Player.hpp"
#include "Item.hpp"

class Enemy : public Mob
{
protected:
Player * player; //Player da cercare
int movement=1; //Intelligenza movimento
bool stillAlive = true;
int CD=0;   //Cooldown
int CDValue = 0; //Valore Cooldown specifico per nemico
Item drop;

public:
    
    Enemy();
    Enemy(Board * board, int yc, int xc, chtype c, int max_hp, int exp, int atk, int movement, Player * player, Item drop);

    void randomMovement();

    void tryToFollow();
    void cases(int y, int x);
    void setPlayer(Player * p);

    bool checkIfDead();
    void followUp(int y, int x);
    bool wallCollision(int cy,int cx);
    void display();
    void gunShot(int y, int x);
    
    bool hasItem();
    Item getItem();
    
};