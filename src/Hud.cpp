//#pragma once
#include "../includes/Hud.hpp"
#include <stdlib.h>

void stampa(const char * s){
    move(1, 0);     //sposta il cursore a inizio riga
    clrtoeol();    //clear to end of line
    mvprintw(1, 0, s);
    move(2, 0);
    clrtoeol();    //clear to end of line
}


void printGameHud(Player player, int evento, int p, int level, int loop){
    printStats(player, level, loop);
    printLog(evento, p, loop);
}

void printStats(Player p, int level, int loop){
    move(0, 0);     //sposta il cursore a inizio riga
    clrtoeol();     //clear to end of line

    mvprintw(0, 0, "Level: %d-%d", loop, level);
    printw("  Hp: %d/%d", p.getHp(), p.getMaxHp());
    printw("  Atk:%d", p.getAtk() + p.getBuffedAtk());  //si mostra anche il buff
    printw("  Exp:%d", p.getExp());
    printw("  Keys:%d", p.getKeyCount());

    if(!(p.hasGun()))           printw("  ");
    else{
        if (p.getGunCD() != 0)  printw("  CD Gun:%d ", p.getGunCD());
        else                    printw("  CD Gun:!");
    }

    if(!(p.hasBeherit()))           printw("  ");
    else{
        if (p.getBerserkCD() != 0)  printw("  CD Berserk:%d ", p.getBerserkCD());
        else                        printw("  CD Berserk:!");
    }

    if(!(p.hasMedkit()))           printw("  ");
    else{
        if (p.getMedkitCD() != 0)  printw("  CD Medikit:%d ", p.getMedkitCD());
        else                        printw("  CD Medikit:!");
    }

    if(!(p.hasFreezeArtifact()))           printw("  ");
    else{
        if (p.getFreezeCD() != 0)  printw("  CD Freeze:%d ", p.getFreezeCD());
        else                        printw("  CD Freeze:!");
    }

}

void printLog(int evento, int param, int loop){
    std::string mes;

    switch(evento){
        case 0:
        default:
            mes = "";
            break;
        case 1:
            mes = "Benvenuto! Usa WASD o le frecce direzionali per muoverti.";
            break;
        case 2:
            mes  = "Hai perso ";          //formare il messaggio con param
            mes += std::to_string(param);
            mes += " hp!";      
            break;

        //scale
        case 55:
            mes = "Un gradino cede e precipiti verso il basso...";
            break;
        
        //affila-arma
        case 70:
            mes = flavorTextA();
            break;
        //cuori
        case 80:
            mes = flavorTextB();
            break;
        case 81:
            mes = "Un raro cuore vacuo! Vita massima permanentemente aumentata di 5.";
            break;

        //chiave e porta
        case 90:
            mes = "Hai raccolto una chiave.";
            break;
        case 91:
            mes = "Hai usato una chiave per aprire una porta.";
            break;
        

        //time freeze
        case 100:
            mes = "Hai trovato un orologio rotto! Premi F per congelare il tempo.";
            break;
        case 101:
            mes = "ZA WARUDO! Il tempo è congelato per 7 turni!";
            break;

        //pistola
        case 102:
            mes = "Raccogli dal suolo la tua pistola magica. Usa la E per sparare.";
            break;
        case 103:
            mes = "Scegli una direzione in cui sparare (la distanza influirà sul danno)";
            break;

        //pistola buff
        case 105:
            mes = "Con questo upgrade, gli attachi melee riducono il CD della pistola!";
            break;

        //medkit
        case 110:
            mes = "Hai trovato un medkit! Premi H per recuperare 5 hp.";
            break;
        case 111:
            mes = "Hai recuperato 5 hp.";
            break;
        case 112:
            mes = "Vita già al massimo!";
            break;

        //regen buff
        case 120:
            mes = "Con questo upgrade, hai una chance di curarti ogni turno!";
            break;

        //beherit
        case 600:
            mes = "Hai raccolto il beherit! Aumenta il tuo ATK con B... ad un prezzo!";
            break;
        case 601:
            mes = "Senti la rabbia! Vita ridotta di " + std::to_string(2*loop) + ", ATK aumentato di " + std::to_string(3*loop) + " per 10 turni.";
            break;
        case 602:
            mes = "Non essere folle! Vita troppo bassa.";
            break;

        //exp milestones
        case 901:
            mes = "Hai 25 exp! Inizi ad avere confidenza armeggiando la pistola.";
            break;
        case 902:
            mes = "Hai 50 exp! Ti sei abituato al rinculo di ogni sparo.";
            break;
        case 903:
            mes = "Hai 100 exp! Ormai sei un pistolero provetto!";
            break;
        case 904:
            mes = "Hai 150 exp! Spareresti i nemici ad occhi chiusi. Forse.";
            break;
        case 905:
            mes = "Hai 250 exp! Guai a chi osa intralciarti!";
            break;

    }
    
    const char * mess = mes.c_str();    //trasforma una stringa in un const char *
    stampa(mess);

}

std::string flavorTextA(){
    std::string mes;
    switch(rand()%8){
        case 0:
            mes = "Ti senti più forte! +1 Atk";
            break;
        case 1:
            mes = "Fai 3 set da 20 flessioni. +1 Atk";
            break;
        case 2:
            mes = "Bevi una bibita tonificante. +1 Atk";
            break;
        case 3:
            mes = "Pensi a qualcosa che ti fa bollire il sangue! +1 Atk";
            break;
        case 4:
            mes = "Affili la tua lama. +1 Atk";
            break;
        case 5:
            mes = "Il potere dell'amicizia ti rende più forte! +1 Atk";
            break;
        case 6:
            mes = "La spesa intelligente ti fortifica! +1 Atk";
            break;
        case 7:
            mes = "Il pensiero del boss finale ti riempie di determinazione! +1 Atk";
            break;
        default:
            break;
    }
    return mes;
}
std::string flavorTextB(){
    std::string mes;
    switch(rand()%8){
        case 0:
            mes = "Ti senti subito meglio! Vita recuperata.";
            break;
        case 1:
            mes = "Le tue ferite spariscono! Vita recuperata.";
            break;
        case 2:
            mes = "Bevi una tazza di latte caldo. Vita recuperata.";
            break;
        case 3:
            mes = "Pensi a qualcosa che rilassa gli spiriti. Vita recuperata.";
            break;
        case 4:
            mes = "Senti la fatica svanire dal corpo. Vita recuperata.";
            break;
        case 5:
            mes = "Ogni tuo dolore sparisce! Vita recuperata.";
            break;
        case 6:
            mes = "Hai appena accarezzato un gattino! Vita recuperata.";
            break;
        case 7:
            mes = "Gli dei ti benedicono con un kebab! Vita recuperata.";
            break;
        default:
            break;
    }
    return mes;
}
