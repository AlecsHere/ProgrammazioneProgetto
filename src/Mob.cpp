//#pragma once
#include <stdlib.h>
#include "../includes/Mob.hpp"


void Mob::construct(Board * board, int yc, int xc, chtype c, int max_hp, int exp, int atk){
    this->max_hp = max_hp;
    this->hp = max_hp;    //vita attuale inizializzata a max_hp;
    this->exp = exp;
    this->atk = atk;
}


Mob::Mob(){   //prototipo vuoto, usa quello della superclasse
this->exp=0;
};
Mob::Mob(Board * board, int yc, int xc, chtype c, int max_hp, int exp, int atk):Entity(board, yc, xc, c){
    construct(board, yc, xc, c, max_hp, exp, atk);
};

//movimenti (mvwaddch fatti per eliminare la "traccia")
void Mob::mvup(){
    mvwaddch(board->getWindow(), y, x, ' ');
    y -= 1;
    display();
}
void Mob::mvdown(){
    mvwaddch(board->getWindow(), y, x, ' ');
    y += 1;
    display();
}
void Mob::mvleft(){
    mvwaddch(board->getWindow(), y, x, ' ');
    x -= 1;
    display();
}
void Mob::mvright(){
    mvwaddch(board->getWindow(), y, x, ' ');
    x += 1;
    display();
}

bool Mob::checkIfDead(){
    if (hp<=0){
        return true;
    }
    else return false;
}

void Mob::attack(Mob * p) //Attacco generico
{
    if(!checkIfDead())  //Se non è morto 
    {
        p->damageCalc(atk); //Lo attacca
    }
}


void Mob::display()
{
    if(!checkIfDead())
    {
        Entity::display();
    }
    else
    {
        mvwaddch(board->getWindow(),y,x,' ');
    }
}

//Funzioni di aggiornamento statistiche

void Mob::increaseAtk(int n){
    atk += n;
}
void Mob::damageCalc(int dmg){
    hp -= dmg;
}
void Mob::healCalc(int heal){
    if(hp+heal > max_hp) hp = max_hp;
    else hp += heal;
}
void Mob::setMaxHp(int newMaxHp){
    max_hp = newMaxHp;
    hp = max_hp;
}
void Mob::setHP(int x)
{
    hp=x;
}
void Mob::setExp(int x){
    exp=x;
}

//funzioni informative
int Mob::getMaxHp(){
    return max_hp;
}
int Mob::getHp(){
    return hp;
}
int Mob::getExp(){
    return exp;
}

int Mob::getAtk(){
    return atk;
}