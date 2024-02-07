#include "../includes/Board.hpp"

void Board::construct(int height, int width){
    getmaxyx(stdscr, yMax, xMax);   //massimi schermo standard
    this->height = height;
    this->width = width;
    start_row = (yMax / 2) - (height / 2);  //per creare il livello (circa) centrato
    start_col = (xMax / 2) - (width / 2);
    
    board_win = newwin(height, width, start_row, start_col);   //va dagli starts fino a height/width
    keypad(board_win, true);    //captures arrow keys
}

Board::Board(){
    construct(20, 80);   //prototipo, mai chiamato
}
Board::Board(int height, int width){
    construct(height, width);
}

void Board::addBorder(){
    box(board_win, 0, 0);
}

void Board::addBox(int xs, int ys, int len, int hei){
    //chtype colArr[] = {/*│*/4194424,/*─*/ 4194417,/*└*/ 4194413,/*┌*/ 4194412,/*┐*/ 4194411,/*┘*/ 4194410};
    //                   0            1             2             3             4             5
    //crea un rettangolo in base agli input
    len-=xs;
    hei = ys-hei;
    bool bor = true;
    printHoriLine(xs,len,bor,ys,true);
    printHoriLine(xs,len,bor,ys-hei,false);
    printVertLine(ys,hei,bor,xs,true);
    printVertLine(ys,hei,bor,xs+len,false);
}

void Board::printHoriLine(int start, int len, bool isBorder, int yLevel, bool top){
    chtype colArr[] = {/*│*/4194424,/*─*/ 4194417,/*└*/ 4194413,/*┌*/ 4194412,/*┐*/ 4194411,/*┘*/ 4194410};
    //                   0            1             2             3             4             5
    //stampa una linea orizzontale partendo da start, lunga len, se è un border mette gli angoli, ad altezza yLevel
    chtype tmp;
    top=!top;
    
    for(int i = 0; i<=len; i++){
        
        if(!isBorder)
            addAt(yLevel,start+i,colArr[1]);
        else{
            if(i==0){
                if (top){tmp=colArr[3];}
                else {tmp=colArr[2];}
                addAt(yLevel,start+i,tmp);
            }
            else if(i==len){
                if (top)tmp=colArr[4];
                else tmp=colArr[5];
                addAt(yLevel,start+i,tmp);
            }
            else addAt(yLevel,start+i,colArr[1]);
        }
    }
}

void Board::printVertLine(int start, int len, bool isBorder, int xLevel, bool top){
    chtype colArr[] = {/*│*/4194424,/*─*/ 4194417,/*└*/ 4194413,/*┌*/ 4194412,/*┐*/ 4194411,/*┘*/ 4194410};
    //                   0            1             2             3             4             5
    //identica a printHoriline ma verticale
    chtype tmp;
    //top=!top;
    //TOP =is it left side?
    for(int i = 0; i<=len; i++){
        
        if(!isBorder)
            addAt(start-i,xLevel,colArr[0]);
        else{
            if(i==0){
                if (top){tmp=colArr[2];}
                else {tmp=colArr[5];}
                addAt(start-i,xLevel,tmp);
            }
            else if(i==len){
                if (top)tmp=colArr[3];
                else tmp=colArr[4];
                addAt(start-i,xLevel,tmp);
            }
            else addAt(start-i,xLevel,colArr[0]);
        }
    }
}

//per interagire direttamente su board_win
void Board::initialize(){
    clear();
    refresh();
}
void Board::clear(){
    wclear(board_win);
}
void Board::refresh(){
    wrefresh(board_win);
}

void Board::addAt(int y, int x, chtype ch){    //scrive char at yx.
    mvwaddch(board_win, y, x, ch);
}

chtype Board::getCharAt(int y, int x){         //legge char at yx.
    return mvwinch(board_win, y, x);
}
chtype Board::getInput(){  //FUNZIONE CHE LEGGE GLI INPUT IN BOARDWIN
    return wgetch(board_win);
}

//Funzioni per l'ottenimento di attributi


//Funzioni per la board
int Board::getStartRow(){
    return start_row;
}
int Board::getStartCol(){
    return start_col;
}
int Board::getHeight(){
    return height;
}
int Board::getWidth(){
    return width;
}
int Board::getEndRow(){
    return start_row + height;
}
int Board::getEndCol(){
    return start_col + width;
}

//Funzioni per la box
int Board::boxGetStartRow(){
    return boxStartRow;
}
int Board::boxGetStartCol(){
    return boxStartCol;
}
int Board::boxGetHeight(){
    return boxHeight;
}
int Board::boxGetWidth(){
    return boxWidth;
}
int Board::boxGetEndRow(){
    return boxStartRow + boxHeight;
}
int Board::boxGetEndCol(){
    return boxStartCol + boxWidth;
}
WINDOW * Board::getWindow(){
    return board_win;
}

//Funzioni per la modifica di Window e Box
void Board::setWindow(WINDOW * window)
{
    board_win = window;
}
//Aggiorna la box che verrà cambiata in Level
void Board::setBox(int startRow, int startCol, int width, int height)
{
    this->boxStartRow=startRow;
    this->boxStartCol=startCol;
    this->boxWidth = width;
    this->boxHeight = height;
    
}

//Funzione cambio colore Board
void Board::changeColor(bool x)
{
    if(x)
    {
        attron(COLOR_PAIR(1));
    }
    else
    {
        attroff(COLOR_PAIR(1));
    }
}