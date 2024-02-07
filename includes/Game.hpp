#pragma once

#include "Player.hpp"
#include "Board.hpp"
#include "Item.hpp"
#include "Enemy.hpp"
#include "Hud.hpp"
#include "Structs.hpp"


class Game
{
private:
    int levelNumber;    //Indice livello corrente
    int loopNumber = 1;
    Board board;
    Player * player;
    p_items items;
    p_enemies enemies;
    bool game_over = false;

    int timeFreeze = 0;

    chtype detail;      //Variabile per gli annunci
    int hudevent = 1, hudparam = 0;

    //flags per exp milestones
    bool flag1=1, flag2=1, flag3=1, flag4=1, flag5=1;
    

public:

    bool timeFrozen = false;
    bool timeGun = false;
    Game();
    Game(int levelNumber, Board board, Player * player, p_items items, p_enemies enemies);

    void setBoard(Board b);
    void processInput();

    void updateState();
    void defaultProcess();

    void cooldownTick();
    void timeFreezeTick();

    void gunEXPertise();

    void gunShot(int dir);
    
    bool enemySame(Enemy en,int y,int x);
    p_enemies checkEnemies(p_enemies enemies, int y, int x);
    void playerActions(int x, int y);

    void damagePlayer(int dmg);

    bool isOver();
    
    void detailGetAnnounce(chtype detail);

    void finalMessage();

    void hudRedraw();
    
    void resetHudParams();

    void updateEntities(p_enemies newEnemy, p_items newItem);

    void updateLevel(int level);

    void updateLoop(int loop);
    
};