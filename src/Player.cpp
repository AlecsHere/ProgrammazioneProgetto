//#pragma once
#include <stdlib.h>
#include "../includes/Player.hpp"

static chtype colArr[] = {/*│*/4194424,/*─*/ 4194417,/*└*/ 4194413,/*┌*/ 4194412,/*┐*/ 4194411,/*┘*/ 4194410};
Player::Player()
{   
};
Player::Player(Board * board, int yc, int xc, chtype c, int max_hp, int exp, int atk):Mob(board,yc,xc,c,max_hp,exp,atk)
{
};

bool Player::wallCollision(int cy,int cx){  //collisioni vietate con oggetti come muri e porte
    for(long unsigned int i=0; i<6; i++){
        if(colArr[i]==mvwinch(board->getWindow(),cy,cx) || ('X'==mvwinch(board->getWindow(),cy,cx) && keyCount==0))
        {
            return true;
        }
            
    }
    return false;
}

void Player::detailCollision(chtype ch){    //collisioni consentite con porte, chiavi e artefatti
    
    switch (ch)
    {
    case 'A':   //attack up
        atk += 1;
        break;
    case 'h':   //cuore
        if(hp+7>max_hp)hp=max_hp;
        else hp+=7;
        break;
    case 'H':   //cuore vacuo
        max_hp+=5;
        hp = max_hp;
        break;
    case 'X':   //porta
        keyCount--;
        break;
    case 'k':   //chiave
        keyCount++;
        break;
    case 'f':   //artefatto pistola
        gunPower = true;
        break;
    case 'l':   //artefatto upgrade pistola
        gunAbsorbPower = true;
        break;
    case 'F':   //artefatto congelamento del tempo
        freezePower = true;
        break;
    case 'M':   //artefatto medkit
        medkitPower = true;
        break;
    case 'r':   //artefatto upgrade regen
        regenPower = true;
        break;
    case 'b':   //artefatto berserk
        berserkPower = true;
        break;
    case '#':   //Cambiamento stanza
        changeRoom = true;
        break;
    default:
        break;
    }
}

void Player::regenChance(){ //potere rigenerativo, 1/45 di recuperare hp ogni turno.
    if (regenPower){
        switch(rand()%45)
        {
            case 0:
            if(hp<max_hp){
                hp++;
            }
            break;
            default:
            break;
        }
    }
}

int Player::getKeyCount(){  //funzione che ritorna le chiavi in possesso
    return keyCount;
}

void Player::givexp(int a){ //funzione che dà esperienza
    setExp(getExp()+a);
}

int Player::getGunAffinity(){   //funzione che ritorna l'affinità con la propria arma
    return gunAffinity;
}
void Player::incGunAffinity(int a){   //funzione che aumenta l'affinità con la propria arma (...riducendo la variabile gunAffinity)
    gunAffinity -= a;
}

void Player::buffAtk(int a, int t){   //funzione che incrementa il danno di 'a' punti per 't' turni
    buffedAtk = a;
    buffCD = t;
}
void Player::tickBuffCD(){    //funzione che riduce il buffCD ogni turno e resetta il buff
    if(buffCD > 0)   buffCD--;
    else buffedAtk = 0;
}
int Player::getBuffedAtk(){
    return buffedAtk;
}

//funzioni di verifica possesso artefatti
bool Player::hasGun(){
    return gunPower;
}
bool Player::hasGunAbsorbPower(){
    return gunAbsorbPower;
}
bool Player::hasFreezeArtifact(){
    return freezePower;
}
bool Player::hasMedkit(){
    return medkitPower;
}
bool Player::hasBeherit(){ 
    return berserkPower;
}
//Funzione che verifica il cambiamento di una stanza. Appena avviene, viene subito settato a falso se vero
bool Player::hasChangeRoom(){
    if(changeRoom)
    {
        changeRoom = false;
        return true;
    }
    return false;
}
//funzioni che ritornano il cooldown degli artefatti
int Player::getFreezeCD(){
    return freezeCD;
}
int Player::getMedkitCD(){
    return medkitCD;
}
int Player::getGunCD(){
    return gunCD;
}
int Player::getBerserkCD(){
    return berserkCD;
}
//funzioni che attivano i cooldowns degli artefatti
void Player::incFreezeCD(int n){
    freezeCD += n;
}
void Player::incMedkitCD(int n){
    medkitCD += n;
}
void Player::incGunCD(int n){
    gunCD += n;
}
void Player::incBerserkCD(int n){
    berserkCD += n;
}
//funzioni che riducono ogni turno i cooldowns
void Player::tickFreezeCD(){
    if(freezeCD > 0)    freezeCD--;
}
void Player::tickMedkitCD(){
    if(medkitCD > 0)    medkitCD--;
}
void Player::tickGunCD(){
    if(gunCD > 0)    gunCD--;
}
void Player::tickBerserkCD(){
    if(berserkCD > 0)    berserkCD--;
}