#pragma once
#include <ncurses.h>
#include "Entity.hpp"
#include "Player.hpp"


class Item : public Entity
{
protected:
    bool collected = false;
    Player * player;

public:

    Item();
    Item(Board * board, Player * player, int yc, int xc, chtype c);

    void display();
    void setPlayer(Player * p);
    bool isCollected();
};