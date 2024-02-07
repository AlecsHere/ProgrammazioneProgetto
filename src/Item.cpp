//#pragma once
#include <cstring>
#include "../includes/Item.hpp"


Item::Item()
{   
};
Item::Item(Board * board, Player * player, int yc, int xc, chtype c):Entity(board,yc,xc,c)
{
    this->player = player;
};

void Item::display()
{
    
    if(player->getxcoord()==getxcoord() && player->getycoord()==getycoord() && !collected)  //Se viene raccolto
    {
        collected=true;
        player->detailCollision(getChar());  //Ottiene l'effetto
    }
    else if(!collected)
    {
        Entity::display(); //Lo mostra
    }
    
}

void Item::setPlayer(Player * p)    //Imposta il player
{
    player = p;
}

bool Item::isCollected()    //Booleano che restituisce se viene colletto
{
    if (collected)
        return true;
    return false;
}