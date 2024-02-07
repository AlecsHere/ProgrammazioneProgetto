//#pragma once
#include "../includes/Level.hpp"
#include <time.h>
#include <stdlib.h>



Level::Level(){};
Level::Level(int levelDifficulty, int levelNumber, char c, int mhp, int exp, int atk)
{
    srand(time(NULL));
    //Inizializza le liste dei nemici ed oggetti
    oHead = new entities;
    oHead->enemy = NULL;
    oHead->item = NULL;

    this->levelDifficulty = levelDifficulty;
    this->levelNumber = levelNumber;
    this->c=c;
    this->mhp=mhp;
    this->exp=exp;
    this->atk=atk;
    this->board = Board(18, 70);

    flag = true;
    level = 1;
    loop = 1;
    levelChoiceInitializer(levelNumber);

    this->player = Player(&board, yc, xc , c, mhp, exp, atk);
    game = Game(levelDifficulty, board, &player, oHead->item, oHead->enemy);
    levelinitialize();
};

//Inizializza la board
void Level::levelinitialize()
{
    board.initialize();
}
void Level::setNextRoom(int top, int down, int left, int right){
    //sets the next rooms to warp to
    //top down left right
    nextRoom[0]=top;
    nextRoom[1]=down;
    nextRoom[2]=left;
    nextRoom[3]=right;
}
void Level::setBoxCoord(int startx,int starty, int len, int hei){
    //sets the corners of the room
    //x y len hei
    boxCoord[0]=startx;
    boxCoord[1]=starty;
    boxCoord[2]=len;
    boxCoord[3]=hei;
    board.setBox(boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
    mapInitialize(currLevel);
}
int Level::getBoxMidHeight(){
    return boxCoord[3]+((boxCoord[1]-boxCoord[3])/2);
}
int Level::getBoxMidWidth(){
    return boxCoord[0]+(boxCoord[2]-boxCoord[0])/2;
}
void Level::gameProcess()
{
    
    //check if player "entered" a wall and send it to the next room
    //bool right: is the player leaving from the right side of the screen?
    //bool bottom: is the player leaving from the bottom of the screen?
    //bool horiVert: true if horizontal movement, false if vertical
    //int nextRoom[]={0,0,0,0}; //top down left right
    //x y len hei

    int nxtlv;
    if(player.getxcoord()==boxCoord[0]){ //if player is in left
        right = false;
        horiVert = true;
        nxtlv = nextRoom[2];
        levelChoiceInitializer(nxtlv);
        drawInitialize(nxtlv);
    }
    else if(player.getxcoord()==boxCoord[2]){ //if player is in right
        right = true;
        horiVert = true;
        nxtlv = nextRoom[3];
        levelChoiceInitializer(nxtlv);
        drawInitialize(nxtlv);
        
    }
    else if(player.getycoord()==boxCoord[3]){ //if player is in top
        bottom = false;
        horiVert = false;
        nxtlv = nextRoom[0];
        levelChoiceInitializer(nxtlv);
        drawInitialize(nxtlv);
        
        
    }
    else if(player.getycoord()==boxCoord[1]){ //if player is in bottom
        bottom = true;
        horiVert = false;
        nxtlv = nextRoom[1];
        levelChoiceInitializer(nxtlv);
        drawInitialize(nxtlv);
        
    }
    else if(player.hasChangeRoom())     //Aggiornamento entità
    {
        //Pulizia oHead
        clearAll();
        oHead = new entities;
        oHead->item = NULL;
        oHead->enemy = NULL;
        oHead->next = NULL;

        int tmp=nextLevel;
        flag = true;
        
        //cambia il livello
        level++;
        //se siamo tornati all'inizio
        if(tmp==10)
        {
            //entriamo in un nuovo loop
            loop++;
            level=1;
            game.updateLoop(loop);
            levelDifficulty+=5;
        }
        //mandiamo il livello alla hud
        game.updateLevel(level);
        game.hudRedraw();
        //chiamiamo la prima stanza del prossimo livello
        levelChoiceInitializer(tmp);
        drawInitialize(tmp);
        levelDifficulty++;
    }
    game.processInput(); //Prende l'input
    game.updateState(); //Aggiorna le variabili
}

void Level::redraw()
{
    //Fa il display degli elementi
    player.display();
    enemiesDisplay();
    itemsDisplay();
    game.updateEntities(oHead->enemy, oHead->item);
    game.hudRedraw();
    if (game.timeFrozen)    //Cambia sfondo in bianco se siamo in TimeFreeze
    {
        wbkgd(board.getWindow(), COLOR_PAIR(1));
    }
    board.refresh();
    refresh();
}

void Level::setPlayerPosition(){
    //check if player "entered" a wall and send it to the next room
    //bool right: is the player leaving from the right side of the screen?
    //bool bottom: is the player leaving from the bottom of the screen?
    //bool horiVert: true if horizontal movement, false if vertical
    //boxCoord x y xmax ymin 
    if(horiVert){
        //horizontal movement
        if(right){//player is leaving from the right, spawn it to the left
            player.setX(boxCoord[0]+1);
            player.setY(getBoxMidHeight());
        }
        else{//spawn to the right
            player.setX(boxCoord[2]-1);
            player.setY(getBoxMidHeight());
        }
    }
    else{
        //vertical movement
        if(bottom){//player is leaving from the bottom of the screen, spawn it at the top
            player.setX(getBoxMidWidth());
            player.setY(boxCoord[3]+1);
        }
        else{//spawn at the bottom
            player.setX(getBoxMidWidth());
            player.setY(boxCoord[1]-1);
        }
    }

}

//spawna banks di nemici predeterminate, selezionandole a random tra quelle del livello di difficoltà specificato
void Level::spawnBank(int difficulty, int x, int y, int xmax, int ymin){
    char enemies[] = {'*','G','T','<','P'};
    //                 0   1   2   3   4  
    int index;
    switch(difficulty){
        case 1: //ragnetti e max 1 goblin
            index = rand()%2;
            switch(index){
                case 0:
                    spawnEnemy(enemies[0],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[0],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[0],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[0],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[0],x,y,xmax,ymin, Item());
                    break;
                case 1:
                    spawnEnemy(enemies[0],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[0],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[0],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    break;
                default:
                    break;
            }
            break;
        case 2: //goblin e qualche ragnetto
            index = rand()%3;
            switch(index){
                case 0:
                    spawnEnemy(enemies[0],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[0],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    break;
                case 1:
                    spawnEnemy(enemies[0],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[0],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[0],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    break;
                case 2:
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    break;
                default:
                    break;
            }
            break;
        case 3: //goblin e P istolero
            index = rand()%3;
            switch(index){
                case 0:
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    break;
                case 1:
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[4],x,y,xmax,ymin, Item());
                    break;
                case 2:
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[4],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[4],x,y,xmax,ymin, Item());
                    break;
                default:
                    break;
            }
            break;
        case 4: // misto tra T, G, <, P
            index = rand()%4;
            switch(index){
                case 0:
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[2],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[3],x,y,xmax,ymin, Item());
                    break;
                case 1:
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[3],x,y,xmax,ymin, Item());
                    break;
                case 2:
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[3],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[4],x,y,xmax,ymin, Item());
                    break;
                case 3:
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[1],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[4],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[4],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[3],x,y,xmax,ymin, Item());
                    break;
                default:
                    break;
            }
            break;
        case 5: // misto tra T, <, P
            index = rand()%4;
            switch(index){
                case 0:
                    spawnEnemy(enemies[2],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[2],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[3],x,y,xmax,ymin, Item());
                    break;
                case 1:
                    spawnEnemy(enemies[2],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[2],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[4],x,y,xmax,ymin, Item());
                    break;
                case 2:
                    spawnEnemy(enemies[2],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[2],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[3],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[4],x,y,xmax,ymin, Item());
                    break;
                case 3:
                    spawnEnemy(enemies[2],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[2],x,y,xmax,ymin, Item());
                    spawnEnemy(enemies[2],x,y,xmax,ymin, Item());
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return;
}
//spawna un singolo nemico in posizione random all'interno del range specificato
void Level::spawnEnemy(char enm, int x, int y, int xmax, int ymin,Item item){
    bool spawned = false;
    int spawnX, spawnY;
    int rangeX = xmax - x - 1, rangeY = y - ymin -1;

    //se non ha già un drop previsto, ha una piccola chance di droppare un h
    if(item.getChar()==1){
        int ranHeart = rand()%35; 
        switch(ranHeart)
        {
            case 0:
                item = initItem('h',0,0);
                break;
            default:
                break;
        }
    }
    //prova a spawnare in posizione random fino a quando non trova un tile vuoto
    chtype tmp;
    do{
        spawnX = rand()%rangeX + 1 + x;
        spawnY = rand()%rangeY + 1 + ymin;
        tmp = board.getCharAt(spawnY, spawnX);
        if(tmp==' '){
            Enemy x = initEnemy(enm, spawnY, spawnX, item);
            x.display();
            listAdderEnemies(oHead->enemy, x);
            spawned = true;
        }
    }while(!spawned);
    return;
}
//Inizializzatori Base per Entità
Enemy Level::initEnemy(chtype init, int yc, int xc, Item drop)
{
    //definizione nemici
    //hp, exp, atk, mov
    switch (init)
    {
    case '*': //ragnetto easy enemy
        return Enemy(&board, yc, xc, '*', 1, (levelDifficulty/2+0.5)+1, (levelDifficulty/4+1), (levelDifficulty/2+1), &player, drop);
    case 'G': //goblin difficoltà media
        return Enemy(&board, yc, xc, 'G', (levelDifficulty/3+0.5)+3, (levelDifficulty/2+0.5)+3, 1.5*((levelDifficulty)/3+1), (levelDifficulty+4.25)/3, &player, drop);
    case 'T': //titano difficoltà alta
        return Enemy(&board, yc, xc, 'T', (levelDifficulty/3+0.5)+5, (levelDifficulty/2+0.5)+4, 2*((1+levelDifficulty)/3), (levelDifficulty+7)/3, &player, drop);
    case '<': //pistolero, pochi colpi ma forti
        return Enemy(&board, yc, xc, '<', (levelDifficulty/3+0.5)+4, (levelDifficulty/2+0.5)+3, 2*((1+levelDifficulty)/3), (levelDifficulty+4.25)/3, &player, drop);
    case 'P': //pistolero, tanti colpi scarsi
        return Enemy(&board, yc, xc, 'P', (levelDifficulty/3+0.5)+4, (levelDifficulty/2+0.5)+3, 1.5*((1+levelDifficulty)/3), (levelDifficulty+4.25)/3, &player, drop);
    case 'N': //last boss
        return Enemy(&board, yc, xc, 'N', (1.5*levelDifficulty/2+0.5)+30, (2*levelDifficulty/2+1.5)+70, 3*((1+levelDifficulty)/3+1.75), 100, &player, drop);
    default:
        break;
    }
    return Enemy();
}

Item Level::initItem(chtype init, int yc, int xc)
{
    //definizione Items
    switch (init)
    {
    case 'A': //atk increase
        return Item(&board, &player, yc, xc, 'A');

    case 'h': //small heal
        return Item(&board, &player, yc, xc, 'h');
    case 'H': //heart up
        return Item(&board, &player, yc, xc, 'H');

    case 'k': //key
        return Item(&board, &player, yc, xc, 'k');
    case 'X': //door
        return Item(&board, &player, yc, xc, 'X');

    case 'f': //pistol
        return Item(&board, &player, yc, xc, 'f');
    case 'l': //pistol upgrade
        return Item(&board, &player, yc, xc, 'l');

    case 'M': //medikit
        return Item(&board, &player, yc, xc, 'M');
    case 'r': //regeneration
        return Item(&board, &player, yc, xc, 'r');

    case 'b': //berserk
        return Item(&board, &player, yc, xc, 'b');
    case 'F': // timefreeze
        return Item(&board, &player, yc, xc, 'F');

    case '#': //next level
        return Item(&board, &player, yc, xc, '#');

    default:
        break;
    }
    return Item();
}

//Display entità

void Level::itemsDisplay()
{
    p_items pointer = oHead->item; //Copia la testa
    p_items p_pointer = NULL; //Genitore dellla testa
    p_items tmp;
    if(oHead->item!=NULL)   //Primo check per controllare se oHead è vuota del tutto
    {
        while(pointer!=NULL)    //Si itera per tutta la lista
        {
            //Il while controlla prima se il puntatore è nullo: se lo è esce subito, sennò controlla l'item che ci siam assicurati non sia nullo sia stato raccolto
            while(pointer!=NULL&&!pointer->item.isCollected())  
            {
                pointer->item.display();  //Lo possiamo mostrare
                p_pointer=pointer; //Il genitore diventa se stesso
                pointer = pointer->next;  //Pointer diventa suo figlio
            }
            if(pointer!=NULL)  //Controllo per non cercare funzioni su item non esistenti
            {
                if(pointer->item.isCollected())  //L'item esiste, e in linea teorica dovrebbe essere sempre raccolto
                {
                    tmp = pointer;  //Pointer che ci servirà per cancellarlo
                    pointer->item.display();  //Non lo facciamo più vedere 
                    pointer = pointer->next;  //Utilizziamo il successivo
                    if(p_pointer!= NULL)  //Se siamo nel caso dove aveva un padre (casi dopo la testa)
                    {
                        p_pointer->next=pointer;  //Aggiorniamo il figlio col nuovo
                    }
                    else  //Caso dove è la testa, e quindi non ha il padre 
                    {
                        oHead->item=pointer; //Aggiorniamo oHead 
                        p_pointer=NULL;  //Teniamo il pointer del padre nullo
                        
                    }
                    delete tmp;  //Deallochiamo tmp
                }
            }
        }
    }
}
//Analogo a itemsDisplay
void Level::enemiesDisplay()
{

    p_enemies pointer = oHead->enemy; //Copia la testa
    p_enemies p_pointer = NULL; //Genitore dellla testa
    p_enemies tmp;
    if(oHead->enemy!=NULL)
    {
        while(pointer!=NULL)
        {
            while(pointer!=NULL&&!pointer->enemy.checkIfDead())
            {
                pointer->enemy.display();
                p_pointer=pointer;
                pointer = pointer->next;
            }
            if(pointer!=NULL)
            {
                if(pointer->enemy.checkIfDead())
                {
                    
                    tmp = pointer;
                    pointer->enemy.display();
                    //Se aveva un'oggetto 
                    if(pointer->enemy.hasItem())
                    {
                        oHead->flag=true; //Gli diamo la possibilità di metterlo in lista 
                        listAdderItems(oHead->item, pointer->enemy.getItem());  //Lo aggiungiamo
                        oHead->flag=false;
                    }
                    pointer = pointer->next;
                    if(p_pointer!= NULL)
                    {
                        p_pointer->next=pointer;
                    }
                    else
                    {
                        oHead->enemy=pointer;
                        p_pointer=NULL;
                    }
                    delete tmp;
                    if (oHead->enemy==NULL) //Se abbiamo finito la lista di nemici (caso non iniziale poiché avviene dopo una rimozione,e specificatamente l'ultima)
                    {
                        flagEnemies=false;  
                        mapInitialize(currLevel);  //Stampiamo le uscite
                    }
                }
            }
        }
    }
}

bool Level::gameIsOver(){
    return game.isOver();
}

void Level::finalMessage(){ //Messaggio finale
    clear();
    clearAll();
    mvprintw(0, 0, "Game Over! Congratulazioni per i %d punti esperienza!", player.getExp());
    mvprintw(1, 0, "Premere un tasto qualsiasi per tornare al terminale.");
}


void Level::setEntities(int level)
{
    p_entities add = new entities;  //Nuovo elemento
    add->id=level;  //Numero della stanza
    p_entities tmp;  //Puntatore temporaneo per la ricerca in caso fosse già stato inizializzato
    
    //Se è vuoto, lo inserisci in testa
    if(oHead->item==NULL&&oHead->enemy==NULL&&oHead->next==NULL&&oHead->id==-1)
    {
        oHead->id=level;
        oHead->next=NULL;
        oHead->flag=true;
    }
    else
    {
        p_entities p = oHead; //Puntatore che parte dalla testa 
        do  //Finche non lo percorri tutto
        {
            if(p->next!=NULL) //Se il prossimo non è nullo
            {
                if(p->next->id==level)  //Se l'hai trovato, lo metti in testa
                {
                    tmp = p->next;
                    p->next = p->next->next;
                    tmp->next = oHead;
                    oHead = tmp;
                    return;
                }
                p = p->next;    //Vai avanti
            }
            else    //Non è mai stato inizializzato (next è nullo),quindi lo inserisci in testa
            {
                add->next = oHead;
                add->id = level;
                oHead = add;
                return;
            }
        }
        while(p!=NULL);
    }
}



void Level::listAdderItems(p_items p, Item obj)
{   
    //Se non l'hai mai inizializzato
    if(oHead->flag)
    {
        p_items add = new items; //puntatore dell'oggetto da inserire
        add->item = obj;
        add->next = NULL;
        //Se è il primo item 
        if(oHead->item==NULL)
        {
            oHead->item = add;
        }
        //Per i successivi
        else
        {
            p_items pointer=oHead->item;
            while(pointer->next !=NULL)//Vai sempre avanti
            {
                pointer=pointer->next;
            }
            //All'ultimo posto, ci aggiungi l'item
            pointer->next = add;
        }
    }
    
}
//Analogo ad listAdderItems
void Level::listAdderEnemies(p_enemies p, Enemy obj)
{
    if(oHead->flag)
    {
        p_enemies pointer;
        p_enemies add = new enemies;
        add->enemy = obj;
        add->next = NULL;
        if(oHead->enemy==NULL)
        {
            oHead->enemy = add;
        }
        else
        {
            pointer=oHead->enemy;
            while(pointer->next !=NULL)
            {
                pointer=pointer->next ;
            }
            pointer->next = add;
        }
    }
}

void Level::clearAll()  //Funzione che pulisce i puntatori
{
    p_entities next;  //Puntatore del prossimo p_enemies
    while (oHead!=NULL)  //Finché non si svuota
    {
        next = oHead->next;  //Si salva il prossimo
        while(oHead->enemy!=NULL)
        {
            p_enemies nextE; //Prossimo enemy
            p_enemies tmp = oHead->enemy;  //Si prende oHead
            nextE = tmp->next;  //Si prende il prossimo
            delete tmp;  //Si cancella
            oHead->enemy = nextE;  //Si passa al prossimo
        }//Analogo agli enemy
        while(oHead->item!=NULL)
        {
            p_items nextI;
            p_items tmp = oHead->item;
            nextI = tmp->next;
            delete tmp;
            oHead->item = nextI;
        }
        delete oHead->enemy;
        delete oHead->item;
        delete oHead;
        oHead = next; //Si passa al prossimo
    }
}



//Sistema dimensioni e entità
void Level::levelChoiceInitializer(int n)
{
    flagEnemies = false;  //Se non è richiesto, non chiudere le stanze 
    currLevel = n;  //Livello attualeroom
    board.initialize();

    //Porta in testa la stanza con l'ID n in oHead
    setEntities(n);

    //set all nextrooms to the lock room
    for(int i = 0; i<4; i++){
        nextRoom[i]=5;
    }
    if(oHead->flag){ //Se non è mai stato inzializzato, parti con gli elementi NULL
        oHead->enemy=NULL;
        oHead->item=NULL;
    }
    switch (n)
    {
    case 5:
        //gbj - Stanza per eventuali errori di compenetrazione non previsti
        setBoxCoord(0,3,2,1);
        setNextRoom(5,5,5,5);
        setPlayerPosition();
        break;
    //LV 1 -------------------------------------------------------
    case 10:
        nextLevel=30;
        //set room coords
        setBoxCoord(25,17,45,7);
        setNextRoom(11,5,5,5);
        this->yc = getBoxMidHeight(); this->xc = getBoxMidWidth();
        //if it's the spawn: put the player in the middle
        if(flag){
            flag = false;
            player.setX(getBoxMidWidth());
            player.setY(getBoxMidHeight());
        }
        else setPlayerPosition();
        break;
        
    case 11:
        setBoxCoord(20,17,50,7);
        setNextRoom(12,10,17,5);
        setPlayerPosition();
        //se è la prima volta nella stanza, spawna items e nemici
        if(oHead->flag){
            listAdderItems(oHead->item, initItem('X', getBoxMidHeight(), boxCoord[0]));
            spawnBank(1,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 12:
        setBoxCoord(32,17,38,5);
        setNextRoom(13,11,5,5);
        setPlayerPosition();
        break;
    case 13:
        setBoxCoord(25,17,45,7);
        setNextRoom(14,12,5,15);
        setPlayerPosition();
        if(oHead->flag){
            spawnBank(1,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 14:
        setBoxCoord(30,15,40,10);
        setNextRoom(5,13,5,5);
        setPlayerPosition();
        if(oHead->flag){
            listAdderItems(oHead->item, initItem('#', getBoxMidHeight(), getBoxMidWidth()));
            listAdderItems(oHead->item, initItem('h', getBoxMidHeight()+1, getBoxMidWidth()));
            oHead->flag=false;
        }

        break;
    case 15:
        setBoxCoord(25,17,45,7);
        setNextRoom(5,16,13,5);
        setPlayerPosition();
        if(oHead->flag){
            spawnBank(1,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 16:
        setBoxCoord(30,15,40,10);
        setNextRoom(15,5,5,5);
        setPlayerPosition();
        if(oHead->flag){
            listAdderItems(oHead->item, initItem('k', getBoxMidHeight(), getBoxMidWidth()));
            oHead->flag=false;
        }
        break;
    case 17:
        setBoxCoord(25,17,45,7);
        setNextRoom(18,5,5,11);
        setPlayerPosition();
        
        if(oHead->flag){
            flagEnemies = true;
            spawnBank(1,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 18:
        setBoxCoord(30,15,40,10);
        setNextRoom(5,17,5,5);
        setPlayerPosition();
        if(oHead->flag){
            if(loop==1)
            listAdderItems(oHead->item, initItem('f', getBoxMidHeight(), getBoxMidWidth()));
            else listAdderItems(oHead->item, initItem('H', getBoxMidHeight(), getBoxMidWidth()));
            oHead->flag=false;
        }
        break;
    //LV 2 -------------------------------------------------------
    case 30:
        nextLevel=50;
        setBoxCoord(25,17,45,7);
        setNextRoom(31,5,5,34);
        if(flag){
            flag = false;
            player.setX(getBoxMidWidth());
            player.setY(getBoxMidHeight());
        }
        else setPlayerPosition();
        break;
    case 31:
        setBoxCoord(25,17,45,7);
        setNextRoom(32,30,38,35);
        setPlayerPosition();
        break;
    case 32:
        setBoxCoord(25,17,45,7);
        setNextRoom(33,31,37,36);
        setPlayerPosition();
        if(oHead->flag){
            listAdderItems(oHead->item, initItem('X', getBoxMidHeight(), boxCoord[0]));
            spawnBank(1,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 33:
        setBoxCoord(30,15,40,10);
        setNextRoom(5,32,5,5);
        setPlayerPosition();
        if(oHead->flag){
            listAdderItems(oHead->item, initItem('#', getBoxMidHeight(), getBoxMidWidth()));
            listAdderItems(oHead->item, initItem('h', getBoxMidHeight()+1, getBoxMidWidth()));
            oHead->flag=false;
        }
        break;
    case 34:
        setBoxCoord(25,17,45,7);
        setNextRoom(35,5,30,5);
        setPlayerPosition();
        if(oHead->flag){
            spawnBank(2,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 35:
        setBoxCoord(25,17,45,7);
        setNextRoom(5,34,31,5);
        setPlayerPosition();
        if(oHead->flag){
            if(loop==1)
            listAdderItems(oHead->item, initItem('r', getBoxMidHeight(), getBoxMidWidth()));
            else listAdderItems(oHead->item, initItem('H', getBoxMidHeight(), getBoxMidWidth()));
            oHead->flag=false;
        }
        break;
    case 36:
        setBoxCoord(25,17,45,7);
        setNextRoom(5,5,32,5);
        setPlayerPosition();
        if(oHead->flag){
            spawnEnemy('*', boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3], initItem('k', 0, 0));
            spawnBank(2,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 37:
        setBoxCoord(25,17,45,7);
        setNextRoom(5,38,5,32);
        setPlayerPosition();
        if(oHead->flag){
            flagEnemies = true;
            spawnBank(2,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 38:
        setBoxCoord(25,17,45,7);
        setNextRoom(37,5,5,31);
        setPlayerPosition();
        if(oHead->flag){
            if(loop==1){
                listAdderItems(oHead->item, initItem('A', getBoxMidHeight(), getBoxMidWidth()-1));
                listAdderItems(oHead->item, initItem('l', getBoxMidHeight(), getBoxMidWidth()+1));
            }
            else listAdderItems(oHead->item, initItem('A', getBoxMidHeight(), getBoxMidWidth()));
            oHead->flag=false;
        }
        break;
    //LV 3 -------------------------------------------------------
    case 50:
        nextLevel=70;
        setBoxCoord(25,17,45,7);
        setNextRoom(56,5,55,51);
        if(flag){
            flag = false;
            player.setX(getBoxMidWidth());
            player.setY(getBoxMidHeight());
        }
        else setPlayerPosition();
        if(oHead->flag){
            //listAdderItems(oHead->item, initItem('X', getBoxMidHeight(), boxCoord[0]));
            listAdderItems(oHead->item, initItem('X', boxCoord[3], getBoxMidWidth()));
            oHead->flag=false;
        }
        break;
    case 51:
        setBoxCoord(25,17,45,7);
        setNextRoom(5,52,50,5);
        setPlayerPosition();
        if(oHead->flag){
            spawnBank(1,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 52:
        setBoxCoord(25,17,45,7);
        setNextRoom(51,5,53,5);
        setPlayerPosition();
        if(oHead->flag){
            spawnBank(2,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 53:
        setBoxCoord(25,17,45,7);
        setNextRoom(5,5,54,52);
        setPlayerPosition();
        if(oHead->flag){
            listAdderItems(oHead->item, initItem('h', getBoxMidHeight(), getBoxMidWidth()));
            spawnBank(3,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 54:
        setBoxCoord(25,17,45,7);
        setNextRoom(55,5,5,53);
        setPlayerPosition();
        break;
    case 55:
        setBoxCoord(25,17,45,7);
        setNextRoom(57,54,5,50);
        setPlayerPosition();
        if(oHead->flag){
            listAdderItems(oHead->item, initItem('k', getBoxMidHeight(), getBoxMidWidth()+1));
            listAdderItems(oHead->item, initItem('k', getBoxMidHeight(), getBoxMidWidth()-1));
            listAdderItems(oHead->item, initItem('X', getBoxMidHeight(), boxCoord[2]-3));
            oHead->flag=false;
        }
        break;
    case 56:
        setBoxCoord(30,15,40,10);
        setNextRoom(5,50,5,5);
        setPlayerPosition();
        if(oHead->flag){
            listAdderItems(oHead->item, initItem('#', getBoxMidHeight(), getBoxMidWidth()));
            listAdderItems(oHead->item, initItem('h', getBoxMidHeight()+1, getBoxMidWidth()));
            oHead->flag=false;
        }
        break;
    case 57:
        setBoxCoord(25,17,45,3);
        setNextRoom(58,55,5,5);
        setPlayerPosition();
        if(oHead->flag){
            flagEnemies = true;
            spawnBank(3,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 58:
        setBoxCoord(30,15,40,10);
        setNextRoom(5,57,5,5);
        setPlayerPosition();
        if(oHead->flag){
            if(loop==1)
                listAdderItems(oHead->item, initItem('b', getBoxMidHeight(), getBoxMidWidth()));
            else
                listAdderItems(oHead->item, initItem('H', getBoxMidHeight(), getBoxMidWidth()));
            oHead->flag=false;
        }
        break;
    //LV 4 -------------------------------------------------------
    case 70:
        nextLevel=90;
        setBoxCoord(25,17,45,7);
        setNextRoom(5,5,5,71);
        if(flag){
            flag = false;
            player.setX(getBoxMidWidth());
            player.setY(getBoxMidHeight());
        }
        else setPlayerPosition();
        break;
    case 71:
        setBoxCoord(25,17,45,7);
        setNextRoom(5,72,70,77);
        setPlayerPosition();
        if(oHead->flag){
            spawnBank(1,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 72:
        setBoxCoord(25,17,45,7);
        setNextRoom(71,5,5,73);
        setPlayerPosition();
        if(oHead->flag){
            spawnBank(2,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 73:
        setBoxCoord(20,17,50,7);
        setNextRoom(77,5,72,74);
        setPlayerPosition();
        if(oHead->flag){
            flagEnemies = true;
            spawnBank(2,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 74:
        setBoxCoord(25,17,45,7);
        setNextRoom(75,5,73,5);
        setPlayerPosition();
        if(oHead->flag){
            spawnBank(3,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 75:
        setBoxCoord(25,17,45,7);
        setNextRoom(5,74,77,76);
        setPlayerPosition();
        if(oHead->flag){
            spawnBank(2,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 76:
        setBoxCoord(30,15,40,10);
        setNextRoom(5,5,75,5);
        setPlayerPosition();
        if(oHead->flag){
            listAdderItems(oHead->item, initItem('#', getBoxMidHeight(), getBoxMidWidth()));
            listAdderItems(oHead->item, initItem('h', getBoxMidHeight()+1, getBoxMidWidth()));
            oHead->flag=false;
        }
        break;
    case 77:
        setBoxCoord(25,17,45,4);
        setNextRoom(78,73,71,75);
        setPlayerPosition();
        if(oHead->flag){
            spawnBank(4,boxCoord[0]+3,boxCoord[1],boxCoord[2]-3,boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 78:
        setBoxCoord(30,15,40,10);
        setNextRoom(5,77,5,5);
        setPlayerPosition();
        if(oHead->flag){
            if (loop==1)
            listAdderItems(oHead->item, initItem('M', getBoxMidHeight(), getBoxMidWidth()));
            else listAdderItems(oHead->item, initItem('H', getBoxMidHeight(), getBoxMidWidth()));
            oHead->flag=false;
        }
        break;
    //LV 5 -------------------------------------------------------
    case 90:
        nextLevel = 10;
        setBoxCoord(25,17,45,7);
        setNextRoom(92,5,5,91);
        if(flag){
            flag = false;
            player.setX(getBoxMidWidth());
            player.setY(getBoxMidHeight());
        }
        else setPlayerPosition();
        break;
    case 91:
        setBoxCoord(25,17,45,5);
        setNextRoom(5,5,90,5);
        setPlayerPosition();
        if(oHead->flag){
            listAdderItems(oHead->item, initItem('k', getBoxMidHeight(), getBoxMidWidth()));
            flagEnemies = true;
            spawnBank(2,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 92:
        setBoxCoord(25,17,45,7);
        setNextRoom(94,90,93,5);
        setPlayerPosition();
        if(oHead->flag){
            spawnBank(1,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 93:
        setBoxCoord(25,17,45,3);
        setNextRoom(5,5,5,92);
        setPlayerPosition();
        if(oHead->flag){
            listAdderItems(oHead->item, initItem('k', getBoxMidHeight(), getBoxMidWidth()));
            flagEnemies = true;
            spawnBank(2,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 94:
        setBoxCoord(25,17,45,7);
        setNextRoom(102,92,5,95);
        setPlayerPosition();
        if(oHead->flag){
            flagEnemies = true;
            listAdderItems(oHead->item, initItem('X', boxCoord[3]+3, getBoxMidWidth()));
            spawnBank(3,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]+3);
            oHead->flag=false;
        }
        break;
    case 95:
        setBoxCoord(25,17,45,7);
        setNextRoom(101,5,94,96);
        setPlayerPosition();
        if(oHead->flag){
            spawnBank(4,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 96:
        setBoxCoord(25,17,45,7);
        setNextRoom(100,97,95,98);
        setPlayerPosition();
        if(oHead->flag){
            spawnBank(4,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 97:
        setBoxCoord(25,17,45,7);
        setNextRoom(96,5,5,5);
        setPlayerPosition();
        if(oHead->flag){
            flagEnemies = true;
            listAdderItems(oHead->item, initItem('k', getBoxMidHeight(), getBoxMidWidth()));
            spawnBank(5,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 98:
        setBoxCoord(20,17,50,7);
        setNextRoom(5,99,96,5);
        setPlayerPosition();
        if(oHead->flag){
            spawnBank(5,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 99:
        setBoxCoord(30,15,40,10);
        setNextRoom(98,5,5,5);
        setPlayerPosition();
        if(oHead->flag){
            listAdderItems(oHead->item, initItem('k', getBoxMidHeight(), getBoxMidWidth()));
            oHead->flag=false;
        }
        break;
    case 100:
        setBoxCoord(25,17,45,7);
        setNextRoom(5,96,101,5);
        setPlayerPosition();
        if(oHead->flag){
            flagEnemies = true;
            listAdderItems(oHead->item, initItem('X', getBoxMidHeight(), boxCoord[0]+3));
            spawnBank(5,boxCoord[0]+3,boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 101:
        setBoxCoord(25,17,45,7);
        setNextRoom(5,95,5,100);
        setPlayerPosition();
        if(oHead->flag){
            if(loop==1){
                listAdderItems(oHead->item, initItem('F', getBoxMidHeight(), getBoxMidWidth()+1));
                listAdderItems(oHead->item, initItem('H', getBoxMidHeight(), getBoxMidWidth()-1));
            }
            else{
                listAdderItems(oHead->item, initItem('A', getBoxMidHeight(), getBoxMidWidth()+1));
                listAdderItems(oHead->item, initItem('H', getBoxMidHeight(), getBoxMidWidth()-1));
            }
            oHead->flag=false;
        }
        break;
    case 102:
        setBoxCoord(25,17,45,7);
        setNextRoom(103,94,5,5);
        setPlayerPosition();
        if(oHead->flag){
            listAdderItems(oHead->item, initItem('X', boxCoord[3]+3, getBoxMidWidth()));
            spawnBank(5,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]+3);
            oHead->flag=false;
        }
        break;
    case 103:
        setBoxCoord(25,17,45,7);
        setNextRoom(5,102,5,104);
        setPlayerPosition();
        if(oHead->flag){
            listAdderItems(oHead->item, initItem('X', getBoxMidHeight(), boxCoord[2]-3));
            spawnBank(5,boxCoord[0],boxCoord[1],boxCoord[2]-3,boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 104://last corridor
        setBoxCoord(15,14,55,10);
        setNextRoom(5,5,103,105);
        setPlayerPosition();
        if(oHead->flag){
            listAdderItems(oHead->item, initItem('A', getBoxMidHeight(), getBoxMidWidth()));
            listAdderItems(oHead->item, initItem('h', getBoxMidHeight(), getBoxMidWidth()-3));
            listAdderItems(oHead->item, initItem('h', getBoxMidHeight(), getBoxMidWidth()+3));
            oHead->flag=false;
        }
        break;
    case 105://last fight
        setBoxCoord(15,17,55,1);
        setNextRoom(106,5,104,5);
        setPlayerPosition();
        if(oHead->flag){
            flagEnemies = true;
            listAdderEnemies(oHead->enemy, initEnemy('N', getBoxMidHeight(), getBoxMidWidth(), initItem('A',0,0)));
            listAdderItems(oHead->item, initItem('h', boxCoord[3]+1, boxCoord[0]+1));
            listAdderItems(oHead->item, initItem('h', boxCoord[1]-1, boxCoord[2]-1));
            listAdderItems(oHead->item, initItem('h', boxCoord[1]-1, boxCoord[0]+1));
            listAdderItems(oHead->item, initItem('h', boxCoord[3]+1, boxCoord[2]-1));
            spawnBank(4,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            spawnBank(4,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            spawnBank(1,boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
            oHead->flag=false;
        }
        break;
    case 106:
        setBoxCoord(25,17,45,7);
        setNextRoom(5,105,5,5);
        setPlayerPosition();
        if(oHead->flag){
            listAdderItems(oHead->item, initItem('#', getBoxMidHeight(), getBoxMidWidth()));
            listAdderItems(oHead->item, initItem('H', getBoxMidHeight()+2, getBoxMidWidth()));
            oHead->flag=false;
        }
        break;
    default:
        break;
    }
    //Aggiorna le entità in game
    game.updateEntities(oHead->enemy, oHead->item);
}

//Sistema il layout grafico
void Level::drawInitialize(int level)
{
    mapInitialize(level);
    board.refresh();
    redraw();
}
void Level::mapInitialize(int level)
{   //disegna la stanza attuale
    board.addBox(boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
    //in base al livello, aggiunge eventuali porte / muri / strutture
    switch (level)
    {
    case 5:
        board.addAt(0,0,'G');
        board.addAt(0,1,'B');
        board.addAt(0,2,'J');
        break;
    //LV 1 -------------------------------------------------------
    case 10:
        board.addAt(boxCoord[3],getBoxMidWidth(),' ');
        break;
    case 11:
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); // SHOULD BE LOCKED DOOR LEFT
        board.addAt(boxCoord[3],getBoxMidWidth(),' ');//TOP
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.printVertLine(boxCoord[1]-3, boxCoord[1]-boxCoord[3]-6, false, boxCoord[2]-8, true);
        board.printVertLine(boxCoord[1]-3, boxCoord[1]-boxCoord[3]-6, false, boxCoord[0]+8, true);
        break;
    case 12:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        break;
    case 13:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        break;
    case 14:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        break;
    case 15:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        break;
    case 16:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        break;
    case 17:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        break;
    case 18:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        break;
    //LV 2 -------------------------------------------------------
    case 30:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        break;
    case 31:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        board.addBox(getBoxMidWidth()-2,getBoxMidHeight()+1,getBoxMidWidth()+2,getBoxMidHeight()-1);
        break;
    case 32:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        
        break;
    case 33:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        break;
    case 34:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        break;
    case 35:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        board.printVertLine(boxCoord[1]-1, boxCoord[1]-boxCoord[3]-2, false, boxCoord[0]+3, true);
        
        break;
    case 36:
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        
        break;
    case 37:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        break;
    case 38:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        board.printVertLine(boxCoord[1]-1, boxCoord[1]-boxCoord[3]-2, false, boxCoord[2]-3, true);
        
        break;
    //LV 3 -------------------------------------------------------
    case 50:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        break;
    case 51:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        break;
    case 52:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        break;
    case 53:
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        break;
    case 54:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        break;
    case 55:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        board.printVertLine(boxCoord[1]-1, boxCoord[1]-boxCoord[3]-2, false, boxCoord[2]-3, true);
        board.addAt(getBoxMidHeight(),boxCoord[2]-3,' ');
        break;
    case 56:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        break;
    case 57:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.printHoriLine(getBoxMidWidth()-5, 10, false, getBoxMidHeight()+4, false);
        board.printHoriLine(getBoxMidWidth()-5, 10, false, getBoxMidHeight()-4, false);
        break;
    case 58:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        
        break;
    //LV 4 -------------------------------------------------------
    case 70:
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        break;
    case 71:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        break;
    case 72:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        break;
    case 73:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        board.printVertLine(boxCoord[3]+6, 5, false, boxCoord[2]-8, true);
        board.printVertLine(boxCoord[1]-1, 5, false, boxCoord[0]+8, true);
        break;
    case 74:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        break;
    case 75:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        break;
    case 76:
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        break;
    case 77:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        board.printVertLine(boxCoord[1]-1, boxCoord[1]-boxCoord[3]-2, false, boxCoord[0]+3, true);
        board.printVertLine(boxCoord[1]-1, boxCoord[1]-boxCoord[3]-2, false, boxCoord[2]-3, true);
        break;
    case 78:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        break;
    //LV 5 -------------------------------------------------------
    case 90:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        break;
    case 91:
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        board.printVertLine(boxCoord[1]-3, boxCoord[1]-boxCoord[3]-6, false, boxCoord[2]-8, true);
        board.printVertLine(boxCoord[1]-3, boxCoord[1]-boxCoord[3]-6, false, boxCoord[0]+8, true);
        break;
    case 92:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        break;
    case 93:
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        board.printVertLine(boxCoord[1]-3, boxCoord[1]-boxCoord[3]-6, false, boxCoord[2]-8, true);
        board.printVertLine(boxCoord[1]-3, boxCoord[1]-boxCoord[3]-6, false, boxCoord[0]+8, true);
        break;
    case 94:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        board.printHoriLine(boxCoord[0]+1, boxCoord[2]-boxCoord[0]-2, false, boxCoord[3]+3, false);
        board.addAt(boxCoord[3]+3,getBoxMidWidth(),' ');
        break;
    case 95:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        break;
    case 96:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        break;
    case 97:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        break;
    case 98:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        break;
    case 99:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        break;
    case 100:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        board.printVertLine(boxCoord[1]-1, boxCoord[1]-boxCoord[3]-2, false, boxCoord[0]+3, true);
        board.addAt(getBoxMidHeight(),boxCoord[0]+3,' ');
        break;
    case 101:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        board.printHoriLine(boxCoord[0]+1, boxCoord[2]-boxCoord[0]-2, false, boxCoord[1]-3, false);
        break;
    case 102:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.printHoriLine(boxCoord[0]+1, boxCoord[2]-boxCoord[0]-2, false, boxCoord[3]+3, false);
        board.addAt(boxCoord[3]+3,getBoxMidWidth(),' ');
        break;
    case 103:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        board.printVertLine(boxCoord[1]-1, boxCoord[1]-boxCoord[3]-2, false, boxCoord[2]-3, true);
        board.addAt(getBoxMidHeight(),boxCoord[2]-3,' ');
        break;
    case 104:
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        board.addAt(getBoxMidHeight(),boxCoord[2],' '); //RIGHT
        break;
    case 105:
        board.addAt(boxCoord[3],getBoxMidWidth(),' '); //TOP
        board.addAt(getBoxMidHeight(),boxCoord[0],' '); //LEFT
        break;
    case 106:
        board.addAt(boxCoord[1],getBoxMidWidth(),' '); //DOWN
        break;
    default:
        break;
    }

    //Se si dovrebbe bloccare, blocca le porte
    if(flagEnemies)
    {
        board.addBox(boxCoord[0],boxCoord[1],boxCoord[2],boxCoord[3]);
    }
}

// funzioni informative
int Level::getLevelNumber()
{
    return levelNumber;
}
Game Level::getGame()
{
    return game;
}
Board Level::getBoard()
{
    return board;
}
Player Level::getPlayer()
{
    return player;
}
