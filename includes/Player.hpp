#pragma once
#include <ncurses.h>
#include "Mob.hpp"


class Player : public Mob
{
private:

    int keyCount = 0;   //Chiavi in possesso

    //Vari poteri
    bool gunPower = false;
    bool gunAbsorbPower = false;
    bool freezePower = false;
    bool medkitPower = false;
    bool regenPower = false;
    bool berserkPower = false;
    bool changeRoom = false;

    //Countdown 
    int gunCD = 0;
    int freezeCD = 0;
    int medkitCD = 0;
    int berserkCD = 0;
    int buffedAtk = 0;
    int buffCD = 0;

    //Gun Affinity
    int gunAffinity = 10;


public:

    Player();
    Player(Board * board, int yc, int xc, chtype c,int max_hp, int exp, int atk);

    bool wallCollision(int cy,int cx);
    void detailCollision(chtype ch);

    void attack(Mob * p);

    void regenChance();

    int getKeyCount();

    void givexp(int a);

    int getGunAffinity();
    void incGunAffinity(int a);

    void buffAtk(int a, int t);
    void tickBuffCD();
    int getBuffedAtk();

    bool hasGun();
    bool hasGunAbsorbPower();
    bool hasFreezeArtifact();
    bool hasMedkit();
    bool hasBeherit();
    bool hasChangeRoom();
    void LevelUp();


    int getFreezeCD();
    int getMedkitCD();
    int getGunCD();
    int getBerserkCD();

    void incFreezeCD(int n);
    void incMedkitCD(int n);
    void incGunCD(int n);
    void incBerserkCD(int n);

    void tickFreezeCD();
    void tickMedkitCD();
    void tickGunCD();
    void tickBerserkCD();

};