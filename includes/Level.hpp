#pragma once
#include <ncurses.h>
#include "Board.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include "Item.hpp"
#include "Enemy.hpp"
#include "Structs.hpp"



class Level
{
protected:
    Board board;
    Player player;
    Game game;
    p_entities oHead;
    int levelNumber;
    int levelDifficulty;
    int height, width;
    int yc, xc;
    int hudevent, hudparam;
    bool right;
    bool bottom;
    bool horiVert;
    char c;
    int mhp;
    int exp;
    int atk;
    bool flagEnemies = false;   //Variabile per il meccanismo di stanza bloccata fino a quando rimangono nemici
    int currLevel;  //Stanza corrente
    bool flag = true; //spawn del player
    int boxCoord[4]={0,0,0,0};
    int nextRoom[4]={0,0,0,0}; //top down left right
    int nextLevel;
    int loop;
    int level;

public:
    Level();
    Level(int levelDifficulty, int levelNumber, char c, int mhp, int exp, int atk);

    void levelinitialize();

    void levelChoiceInitializer(int n);

    Enemy initEnemy(chtype init, int yc, int xc, Item drop);
    Item initItem(chtype init, int yc, int xc);

    void redraw();
    void itemsDisplay();
    void enemiesDisplay();
    void gameProcess();
    bool gameIsOver();
    void finalMessage();

    void drawInitialize(int level);
    void mapInitialize(int level);
    void setEntities(int level);
    void listAdderItems(p_items p, Item obj);
    void listAdderEnemies(p_enemies p, Enemy obj);
    void clearAll();

    // funzioni informative
    int getLevelNumber();
    Game getGame();
    Board getBoard();
    Player getPlayer();

    void setBoxCoord(int startx,int starty, int len, int hei);
    void setNextRoom(int top, int down, int left, int right);
    int getBoxMidHeight();
    int getBoxMidWidth();
    void setPlayerPosition();
    void spawnBank(int difficulty, int x, int y, int xmax, int ymin);
    void spawnEnemy(char enm, int x, int y, int xmax, int ymin, Item item);
};