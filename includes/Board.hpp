#pragma once    //In the C and C++ programming languages, pragma once is a non-standard but widely supported preprocessor directive designed to cause the current source file to be included only once in a single compilation.
#include <ncurses.h>

class Board
{

private:
    WINDOW * board_win;  //finestra
    int height, width, start_row, start_col;    //Parametri board
    int boxStartRow, boxStartCol, boxWidth, boxHeight;  //Parametri Box
    void construct(int height, int width);  //Funzione costruttore
    int xMax, yMax;     //Confini massimi Board

public:

    Board();
    Board(int height, int width);

    void addBorder();
    //per interagire direttamente su board_win
    void initialize();
    void clear();
    void refresh();

    void addAt(int y, int x, chtype ch);
    chtype getCharAt(int y, int x);
    chtype getInput();
    
    //funzioni informative
    int getStartRow();
    int getStartCol();
    int getHeight();
    int getWidth();
    int getEndRow();
    int getEndCol();
    int boxGetStartRow();
    int boxGetStartCol();
    int boxGetHeight();
    int boxGetWidth();
    int boxGetEndRow();
    int boxGetEndCol();
    WINDOW * getWindow();

    void changeColor(bool x);
    void addBox(int xs, int ys, int xl, int yl);
    void printVertLine(int start, int len, bool isBorder, int xLevel, bool top);
    void printHoriLine(int start, int lenght, bool isBorder, int yLevel, bool top);
    void setWindow(WINDOW * window);
    void setBox(int startRow, int startCol, int width, int height);
};