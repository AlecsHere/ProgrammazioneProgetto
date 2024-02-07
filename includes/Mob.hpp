#pragma once
#include <ncurses.h>
#include "Board.hpp"
#include "Entity.hpp"


class Mob : public Entity
{
protected:
    int max_hp;         //vita massima
    int hp;             //vita attuale
    int exp;              //esperienza/punteggio
    int atk;              //valore di attacco
    int decay = 0;        //valore di ricarica attacco


    void construct(Board * board, int yc, int xc, chtype c, int max_hp, int exp, int atk);


public:
    
    Mob();
    Mob(Board * board, int yc, int xc, chtype c, int max_hp, int exp, int atk);

    //movimenti (mvwaddch fatti per eliminare la "traccia")
    void mvup();
    void mvdown();
    void mvleft();
    void mvright();

    void increaseAtk(int n);
    void damageCalc(int dmg);
    void healCalc(int heal);
    void setMaxHp(int newMaxHp);

    bool checkIfDead();

    void attack(Mob * p);

    void display();

    void setHP(int x);

    //funzioni informative
    int getMaxHp();
    int getHp();
    int getExp();
    void setExp(int x);
    int getAtk();
    
};