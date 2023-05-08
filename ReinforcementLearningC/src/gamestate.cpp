#include "gamestate.h"

gamestate::gamestate()
{
    for (int i=0;i<4;i++)
        for (int j=0;j<12;j++)
        this.rewards[i][j]=-1;
    for (int i=1;i<11;i++)
        this.rewards[i][j]=-100;
    this.rewards[3][11]=100;
    //ctor
}
gamestate::printRewards()
{
   for (int i=0;i<4;i++)
    for (int j=0;j<12;j++)
        cout<<this.rewards[i][j]=-1;
}
gamestate::~gamestate()
{
    //dtor
}
