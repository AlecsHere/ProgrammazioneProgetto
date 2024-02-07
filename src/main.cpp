#include <ncurses.h>
#include <iostream>
#include "../includes/Level.hpp"


//costanti per il livello
#define STARTING_LEVEL 10
#define STARTING_DIFFICULTY 1

//costanti per il giocatore
#define PLR_CHAR '@'
#define PLR_MAXHP 20
#define PLR_EXP 0
#define PLR_ATK 1

int main(int argc, char ** argv){

    //IMPOSTAZIONI DEL TERMINALE
    initscr();
    start_color();  //Inizializza colore
    init_pair(1, COLOR_BLACK, COLOR_WHITE);  //Coppia di schermi nero e bianco
    refresh();
    noecho();   //non scrive l'input utente sul terminale
    cbreak();   //The cbreak routine disables line buffering and erase/kill character-processing, making characters typed by the user immediately available to the program
    curs_set(0);    //nasconde il cursore
    

    //CREAZIONE DEL GIOCO
    Level level(STARTING_DIFFICULTY, STARTING_LEVEL, PLR_CHAR, PLR_MAXHP, PLR_EXP, PLR_ATK);

    //INIZIALIZZAZIONE SCHERMO
    level.drawInitialize(STARTING_LEVEL);
    //LOOP PRINCIPALE   (continua finchè non c'è game_over)
    while(!level.gameIsOver()){
        level.gameProcess();
        level.redraw();
    }
    //GAME OVER
    level.finalMessage();
    getch();    //permette di leggere il messagio di game over

    //FINALIZZAZIONE SCHERMO
    endwin();
    std::cout << "\n\nGame Over! Congratulazioni per i " + std::to_string(level.getPlayer().getExp()) + " punti esperienza! \n\n\n";
    return 0;
}