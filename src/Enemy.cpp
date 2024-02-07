#include <stdlib.h>
#include "../includes/Enemy.hpp"


Enemy::Enemy()
{   
};
Enemy::Enemy(Board * board, int yc, int xc, chtype c, int max_hp, int exp, int atk, int movement, Player * player, Item drop): Mob(board,yc,xc,c,max_hp,exp,atk)
{
    this->player = player;
    this->movement = movement;
    this->drop = drop;
    switch(c)
    {
        case '<':
            this->CDValue=8; //Cooldown pistolero forte
            break;
        case 'P':
            this->CDValue=5; //Cooldown pistolero
            break;
        default:
            this->CDValue=0;    //Default mai presentato
            break;
    }
};

bool Enemy::wallCollision(int cy,int cx){  //Condizione che controlla la collisione di un nemico
    if(' '==mvwinch(board->getWindow(),cy,cx)||player->getChar()==mvwinch(board->getWindow(),cy,cx)){ //Se è spazio vuoto oppure è un'attacco al player
        if(cy>board->boxGetHeight()&&cy<board->boxGetStartCol()&&cx>board->boxGetStartRow()&&cx<board->boxGetWidth()){ //Se è al confine del box all'interno della board
            return false;   //Abbiamo avuto collisione
        }
    }
    return true;
}

void Enemy::randomMovement()  //Scelta di un movimento casuale tra le 4 assi, con una possibilità di 1/3 di rimanere fermi
{
    int i = rand()%6;
    switch(i)
    {
        case 0:
            cases(-1,0); //UP
            break;
        case 1:
            cases(1,0); //DOWN
            break;
        case 2:
            cases(0,-1); //LEFT
            break;
        case 3:
            cases(0,1);  //RIGHT
            break;
        default:    //Rimane fermo
            break;
    }
}

void Enemy::tryToFollow()   //Funzione di routine del nemico
{
    if(!checkIfDead())      //Controllo nel caso non si è ancora arrivati a togliere il nemico dalla lista
    {
        int x = getxcoord()-player->getxcoord();
        int y = getycoord()-player->getycoord();
        if(abs(x)+abs(y)<=1&&(abs(x)<2&&abs(y)<2))   //Se è vicino al player
        {
            attack(player);     //Lo attacca
        }
        else
        {
            switch(rand()%movement){  //Realizza un movimento in base alla sua natura.
            case 0:
                randomMovement();
                break;
            default:
                switch(c){
                    case '<': case 'P':
                        gunShot(y,x);  //Condizione pistolero
                        break;
                    default:
                        followUp(y,x);  //Condizione generica
                    break;
                }
            break; 
            }
        }
    }
}

void Enemy::followUp(int y, int x)  //Tentativo di allinearci all'asse del player
{
    switch(rand()%2)  //Scelta casuale da quale mossa iniziare a muoverci
    {
        case 0:  //Scelta orizontale
            if(x==0)
            {
                cases(-(y/abs(y)),0);  //Siamo nell'asse x del player, quindi avviciniamoci sul asse y
            }
            else
            {
                cases(0,-(x/abs(x)));
            }
        break;
        default:
            if(y==0)
            {
                cases(0,-(x/abs(x)));     //Siamo nell'asse y del player, quindi avviciniamoci sul asse x
            }
            else
            {   
                cases(-(y/abs(y)),0);
            }
        break;
    }
}
bool Enemy::checkIfDead()
{   if (hp<=0){
        //Settiamo le coordinate del drop nel punto di morte
        drop.setY(getycoord()); 
        drop.setX(getxcoord());
        if(exp>0)  //Diamo l'exp
        {
            player->givexp(exp);
            exp=0;
        }
        return true;
    }
    else return false;
}
void Enemy::cases(int y, int x)
{
    if(!wallCollision(getycoord()+y, getxcoord()+x))
    {
            switch(y)
            {
                case -1:
                    mvup();
                    break;
                case 1:
                    mvdown();
                    break;
                default:
                    switch(x)
                    {
                        case -1:
                            mvleft();
                            break;
                        case 1:
                            mvright();
                            break;
                        default:
                        break;
                    }
                break;
            }
            //Dopo esserci mosso, se il nemico può attaccarci ci attacca
            int x = getxcoord()-player->getxcoord();
            int y = getycoord()-player->getycoord();
            if(abs(x)+abs(y)<=1&&(abs(x)<2&&abs(y)<2))   //Se è vicino al player
            {
                attack(player);     //Lo attacca
            }
        //}
    }
}

void Enemy::display()
{
    if(!checkIfDead())
    {
        Entity::display();  //Si mostra
    }
    else
    {
        mvwaddch(board->getWindow(),y,x,' ');      //Lo cancelliamo
    }
}


void Enemy::setPlayer(Player * p)
{
    player = p;
}

void Enemy::gunShot(int y,int x)
{
    int i = 0;
    //Se non ha cooldown
    if(x==0 &&CD==0)    //Spara orizontalmente se è nello stesso asse x
    {
        if(y>0)
        {
            int i = getycoord()-1;
            while(mvwinch(board->getWindow(), i, getxcoord())==' ')
            {
                i--;
            }
        }
        else if (y<0)
        {
            i = getycoord()+1;
            while(mvwinch(board->getWindow(), i, getxcoord())==' ')
            {
                i++;
            }  
        }
        if(i==player->getycoord()){  //Se il proiettile ha raggiunto il player
            wbkgd(board->getWindow(), COLOR_PAIR(1));
            player->damageCalc(atk);
        }
        CD=5;
    }
    else if (y==0 && CD==0)  //Spara verticalmente se è nello stesso asse y
    {
        if(x>0)
        {
            i = getxcoord()-1;
            while(mvwinch(board->getWindow(), getycoord(),i)==' ')
            {
                i--;
            }
        }
        else if (x<0)
        {
            i = getxcoord()+1;
            while(mvwinch(board->getWindow(), getycoord(),i)==' ')
            {
                i++;
            }
        }
        if(i==player->getxcoord()){
            wbkgd(board->getWindow(), COLOR_PAIR(1));
            player->damageCalc(atk);
        }
        CD=5;
    }
    else
    {
        if(CD>0)
        {
            CD--; //Riduce il cooldown
        }
        followUp(y,x);  //Oppure va a mani nude
    }
 }   
bool Enemy::hasItem()  //Controlo Drop
{
    if(drop.getChar()==1)
    {
        return false;
    }
    return true;
}

Item Enemy::getItem()
{
    return drop;
}
    
