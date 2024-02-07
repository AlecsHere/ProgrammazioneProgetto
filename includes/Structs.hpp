#pragma once    //In the C and C++ programming languages, pragma once is a non-standard but widely supported preprocessor directive designed to cause the current source file to be included only once in a single compilation.
#include <ncurses.h>
#include "Item.hpp"
#include "Enemy.hpp"

struct items    //Strutture per gli items
{
    Item item;
    items *next;
};
typedef items *p_items;  //Puntatori items

struct enemies  //Strutture per gli enemies
{
    Enemy enemy;
    enemies *next;
};
typedef enemies *p_enemies;  //Puntatori enemies

struct entities //Strutture che andranno a rappresentare le stanze
{
    p_enemies enemy;
    p_items item;
    entities *next;
    int id = -1;     //Id stanza
    bool flag = true;   //Se è la prima visita o meno
};
typedef entities *p_entities;  //Puntatori entità