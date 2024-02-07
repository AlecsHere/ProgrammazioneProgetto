//#pragma once

#include <time.h>
#include <stdlib.h>
#include "../includes/Level.hpp"
#include "../includes/Game.hpp"

Game::Game(){};
Game::Game(int levelNumber, Board board, Player *player, p_items items, p_enemies enemies)
{
    this->levelNumber = levelNumber;
    this->board = board;
    this->player = player;
    this->items = items;
    this->enemies = enemies;
};

void Game::processInput()       //Funzione per gli input del giocatore
{
    chtype input = board.getInput();
    wbkgd(board.getWindow(), COLOR_PAIR(0));    //Ripristiniamo la window modificata attraverso l'effetto di Freeze
    switch (input)
    {
    // MOVIMENTI
    case KEY_UP:
    case 'w': // UP
        if (!player->wallCollision(player->getycoord() - 1, player->getxcoord())) //Controllo collisioni
        {
            playerActions(-1, 0);
        }
        break;
    case KEY_DOWN:
    case 's': // DOWN
        if (!player->wallCollision(player->getycoord() + 1, player->getxcoord()))
        {
            playerActions(1, 0);
        }
        break;
    case KEY_LEFT:
    case 'a': // LEFT
        if (!player->wallCollision(player->getycoord(), player->getxcoord() - 1))
        {
            playerActions(0, -1);
        }
        break;
    case KEY_RIGHT:
    case 'd': // RIGHT
        if (!player->wallCollision(player->getycoord(), player->getxcoord() + 1))
        {
            playerActions(0, 1);
        }
        break;

    case 'e': // PISTOLA
        if (player->hasGun() && (player->getGunCD() == 0))
        {
            hudevent = 103;

            int affinity = player->getGunAffinity() + 2; //2 aggiunto per considerare il turno di uso e di fine
            timeFreeze++;
            timeFrozen = true;
            timeGun = true;
            player->incGunCD(affinity);
        }
        break;

    case 'f': // TIME FREEZE
        if (player->hasFreezeArtifact() && (player->getFreezeCD() == 0))
        {

            wbkgd(board.getWindow(), COLOR_PAIR(1));
            hudevent = 101;
            timeFreeze = 7 + 1; //1 aggiunto per considerare il turno di uso
            timeFrozen = true;
            player->incFreezeCD(48);
        }
        break;
    
    case 'h': // MEDKIT
        if (player->hasMedkit() && (player->getMedkitCD() == 0))
        {
            if (player->getHp() == player->getMaxHp())
            {
                hudevent = 112;
                break;
            }
            hudevent = 111;
            player->healCalc(5);
            player->incMedkitCD(76);
        }
        break;
        

    case 'b': // BERSERK
        if (player->hasBeherit() && (player->getBerserkCD() == 0))
        {
            if (player->getHp() <= 2*loopNumber)
            {
                hudevent = 602;
                break;
            }
            hudevent = 601;
            player->damageCalc(2*loopNumber);
            player->buffAtk(3*loopNumber, 11);
            player->incBerserkCD(51);
        }
        break;
    // QUIT GAME
    case 'q':
        game_over = true;
        break;
    /* DEBUGGING
    case 'l':
        player->detailCollision('#');
        break;
    case '+':
        player->healCalc(100);
        break;*/
    default:
        return;
    }

    defaultProcess();   //Funzioni dopo le mosse del player
    detailGetAnnounce(detail);  //Annuncia l'effetto appena ottenuto in gioco
    detail = 0; 
    
}

void Game::updateState()
{ // controlla se il personaggio è morto
    if (player->checkIfDead())
        game_over = true;
}

void Game::defaultProcess()
{ // processing di default chiamato sempre

    p_enemies tmp = enemies;
    if (tmp != NULL) //Se ci sono nemici
    {
        if (!timeFrozen)  //Controllo per non far muovere i nemici durante il time freeze
        {
            do
            {
                tmp->enemy.tryToFollow(); // Si muove
                tmp = tmp->next; // Nemico successivo
            } 
            while (tmp != NULL);
        }
        cooldownTick();  //Tick per i cooldown generali
        player->regenChance();   //Il player ha una chance di rigenerare un cuore
    }
    timeFreezeTick(); // ticking separato per il tempo congelato
    gunEXPertise();   //Miglioramento dell'abilità della pistola
    updateState();    //Controllo per gameover
}

void Game::cooldownTick()
{ // aggiorna i valori dei cooldowns
    player->tickFreezeCD();
    player->tickMedkitCD();
    player->tickGunCD();
    player->tickBerserkCD();
    player->tickBuffCD();
}

void Game::timeFreezeTick()
{ // scongela il tempo
    if (timeFreeze > 0)
    {
        timeFreeze--;
        return;
    }
    timeFrozen = false;
}

void Game::gunEXPertise(){ //meccanica di proficenza con la pistola: riduci il cd se si ha un tot xp
    if(player->getExp()>=25 && flag1){
        player->incGunAffinity(1);
        flag1 = false;
        hudevent=901;
    }else if(player->getExp()>=50 && flag2){
        player->incGunAffinity(1);
        flag2 = false;
        hudevent=902;
    }else if(player->getExp()>=100 && flag3){
        player->incGunAffinity(2);
        flag3 = false;
        hudevent=903;
    }else if(player->getExp()>=150 && flag4){
        player->incGunAffinity(1);
        flag4 = false;
        hudevent=904;
    }else if(player->getExp()>=250 && flag5){
        player->incGunAffinity(2);
        flag5 = false;
        hudevent=905;
    }
}

void Game::gunShot(int dir) // sparo proiettile
{
    if(enemies!=NULL)
    {
        p_enemies same = NULL; //Puntatore di controllo nemici
        int i;
        switch (dir)
        {
        case -10:
            i = player->getycoord()-1;
            while(mvwinch(board.getWindow(), i, player->getxcoord())==' ')  //Finché non trova un elemento nella traiettoria
            {
                i--;
            }
            same = checkEnemies(enemies, i, player->getxcoord()); // Variabile interazione nemico: se c'è un nemico, lo prende
            i=player->getycoord()-i;
            break;
        case 10:
            i = player->getycoord()+1;
            while(mvwinch(board.getWindow(), i, player->getxcoord())==' ')
            {
                i++;
            } 
            same = checkEnemies(enemies, i, player->getxcoord()); // Variabile interazione nemico: se c'è un nemico, lo prende
            i=i-player->getycoord();
            break;
        case 0-1:
            i = player->getxcoord()-1;
            while(mvwinch(board.getWindow(), player->getycoord(),i)==' ')
            {
                i--;
            }
            same = checkEnemies(enemies, player->getycoord(), i); // Variabile interazione nemico: se c'è un nemico, lo prende
            i=player->getxcoord()-i;
            break;
        case 01:
            i = player->getxcoord()+1;
            while(mvwinch(board.getWindow(), player->getycoord(),i)==' ')
            {
                i++;
            }
            same = checkEnemies(enemies, player->getycoord(), i); // Variabile interazione nemico: se c'è un nemico, lo prende
            i=i-player->getxcoord();
            break;
        default:
            break;
        }
        if (same != NULL) // Se il player sta nella traiettoria e ha colpito un nemico
        {
            same->enemy.damageCalc(std::max(1,((player->getAtk()+player->getBuffedAtk()) - (int)(std::max(0,(i-3))*0.5))));  //Gli facciamo danno: Con distanza fino a 3, danno completo, da 4 in poi per ogni 2 tiles diminuisce di 1. Attacco minimo 1
            same->enemy.display();
        }
    }
    timeGun = false;
}

p_enemies Game::checkEnemies(p_enemies p, int y, int x)  //Funzione che calcola se tale nemico sta nella posizione y,x
{
    p_enemies pointer = p; //Puntatore nemici
    if (pointer!=NULL)  //Se ci sono nemici
    {
        do
        {
            if (pointer->enemy.getycoord() == y && pointer->enemy.getxcoord() == x)
            {
                return pointer;
            }
            pointer = pointer->next;
        } while (pointer != NULL);
    }
    return NULL;
}

void Game::playerActions(int x, int y)
{
    int coord = (x * 10) + y;
    if (timeGun) // Se sta sparando
    {
        gunShot(coord);
    }
    else
    {
        detail = board.getCharAt(player->getycoord() + x, player->getxcoord() + y);    // Variabile annunci
        p_enemies same = checkEnemies(enemies, player->getycoord() + x, player->getxcoord() + y); // Variabile interazione nemico
        if (same != NULL) //Se ha trovato un nemico nella ricerca
        {
            same->enemy.damageCalc(player->getAtk() + player->getBuffedAtk()); // Attacca il nemico
            if (player->hasGunAbsorbPower() && player->getGunCD() > 0)
                player->tickGunCD();                              // riduci il CD della pistola
        }
        else
        {
            switch (coord)
            {
            case -10:
                player->mvup();
                break;
            case 10:
                player->mvdown();
                break;
            case 0 - 1:
                player->mvleft();
                break;
            case 01:
                player->mvright();
                break;
            }
        }
    }
}

bool Game::isOver()
{
    return game_over;
}

void Game::detailGetAnnounce(chtype detail)
{
    switch (detail)
    {
    case '#':
        hudevent = 55;
        break;
    case 'A':
        hudevent = 70;
        break;
    case 'h':
        hudevent = 80;
        break;
    case 'H':
        hudevent = 81;
        break;
    case 'k':
        hudevent = 90;
        break;
    case 'X':
        hudevent = 91;
        break;
    case 'f':
        hudevent = 102;
        break;
    case 'l':
        hudevent = 105;
        break;
    case 'F':
        hudevent = 100;
        break;
    case 'M':
        hudevent = 110;
        break;
    case 'r':
        hudevent = 120;
        break;
    case 'b':
        hudevent = 600;
        break;
    default:
        break;
    }
}
//Disenga l'HUD
void Game::hudRedraw()
{
    printGameHud(*player, hudevent, hudparam, levelNumber, loopNumber);
    resetHudParams();
}

void Game::resetHudParams()
{ // reimposta gli input della hud
    hudevent = 0;
    hudparam = 0;
}


//Aggiornamenti variabili per Level

//Aggiorna la Board
void Game::setBoard(Board b)
{
    this->board = b;
}
//Aggiorna le entità
void Game::updateEntities(p_enemies newEnemy, p_items newItem)
{
    enemies = newEnemy;
    items = newItem;
}
//Aggiorna il valore del livello e del loop
void Game::updateLevel(int level)
{
    levelNumber = level;
}
void Game::updateLoop(int loop)
{
    loopNumber = loop;
}