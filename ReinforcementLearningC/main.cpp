#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib>
#include <math.h>
#include <tgmath.h>
#include <bits/stdc++.h>
using namespace std;
int rewards[4][12];
typedef pair<int,int> pozitie;
typedef pair<int,int> tranzitie;
typedef map<pair<pozitie,tranzitie>,double> Qtable;
typedef std::map<pair<pozitie,tranzitie>,double>::iterator qterator;
Qtable table;
const double alfa=0.1;
const double gamma=1;
//it = myNetwork.map.find(make_pair(pozitie,tranzitie));
tranzitie moveUp=make_pair(-1,0);
tranzitie moveRight=make_pair(0,1);
tranzitie moveDown=make_pair(1,0);
tranzitie moveLeft=make_pair(0,-1);
pozitie startingPoint=make_pair(3,0);
void print_matrix(int matrix[4][12])
{
    for (int i=0;i<4;i++)
       {for (int j=0;j<12;j++)
    {
        cout<<matrix[i][j]<<" ";
    }
    cout<<endl;
       }
}
tranzitie tryAll(int index){
if (index==0) return moveUp;
if (index==1) return moveRight;
if (index==2)return moveDown;
if (index==3) return moveLeft;
throw new exception;
}
string getMoveName(tranzitie tr){
if(tr==moveUp) return "up";
if (tr==moveRight) return "right";
if (tr==moveLeft) return "left";
if (tr==moveDown) return "down";
return nullptr;
}
bool validMove(pozitie currPos,tranzitie desiredTrans)
{
     return(currPos.first+desiredTrans.first<4 && currPos.first+desiredTrans.first>=0&&
            currPos.second+desiredTrans.second<12&&currPos.second+desiredTrans.second>=0);
}
bool checkFinal(pozitie poz)
{
    return poz.first==3&&poz.second==11;
}
void updatePosition(pozitie &currPos,tranzitie mov)
{
    if (validMove(currPos,mov)){
        currPos.first=currPos.first+mov.first;
        currPos.second=currPos.second+mov.second;
    }
}


int transitionReward(pozitie &currPos,tranzitie desiredTrans)
{

    if (currPos.first==3 && currPos.second>0 && currPos.second<11){
            int tRew= rewards[currPos.first][currPos.second];
        currPos.first=3;
        currPos.second=0;
        return tRew;
    }

    else{

    if (validMove(currPos,desiredTrans))
        {
            currPos.first+=desiredTrans.first;
            currPos.second+=desiredTrans.second;
            if (checkFinal(currPos))
            {
                currPos.first=3;
                currPos.second=0;
                return 0;
            }
            return -1;
        }
    return 0;
    }
}
void initializeTable(Qtable &table)
{
    for (int i=0;i<4;i++)
    {
        for (int j=0;j<12;j++)
        {
            for(int mov=0;mov<4;mov++)
            {
            pair<pozitie,tranzitie>pereche=make_pair(make_pair(i,j),tryAll(mov));
            if (validMove(pereche.first,pereche.second))
                table[pereche]=0;
            else
                table[pereche]=-9999;
            }
        }
    }
}
tranzitie getBestMove(Qtable table,pozitie poz)
{
tranzitie bestMov;
int bestQ=-2000;
    for(int mov=0;mov<4;mov++)
    {
    pair<pozitie,tranzitie>pereche=make_pair(poz,tryAll(mov));
    if (validMove(pereche.first,pereche.second))
       {
          qterator qt= table.find(pereche);
          if (qt->second >bestQ)
          {
              bestQ=qt->second;
              bestMov=pereche.second;
          }
       }
    }

    return bestMov;
}


tranzitie getBestMoveEpsilon(Qtable table,pozitie poz,double &eps)
{
tranzitie bestMov;
int bestQ=-2000;
double r=rand()%101/100.;
//cout<<r<<" "<<eps<<endl;
    if (r>eps)
        {for(int mov=0;mov<4;mov++)
            {
            pair<pozitie,tranzitie>pereche=make_pair(poz,tryAll(mov));
            if (validMove(pereche.first,pereche.second))
               {
                  qterator qt= table.find(pereche);
                  if (qt->second >bestQ)
                  {
                      bestQ=qt->second;
                      bestMov=pereche.second;
                  }
               }
            }
        }
    else{
        int r=rand()%4;
    while(!validMove(poz,tryAll(r)))r=rand()%4;
        bestMov=tryAll(r);
    }
    return bestMov;
}




void updateTable(Qtable &table,pozitie poz,tranzitie mov)
{
    pozitie futurePos=poz;
    updatePosition(futurePos,mov);
    pair<pozitie,tranzitie>pereche_curenta=make_pair(poz,mov);

    table[pereche_curenta]+=
        alfa*
        (rewards[futurePos.first][futurePos.second]+
        gamma*table[make_pair(futurePos,getBestMove(table,futurePos))]-
        table[pereche_curenta]);
}

void updateTableEpsilon(Qtable &table,pozitie poz,tranzitie mov,double &eps)
{
    pozitie futurePos=poz;
    updatePosition(futurePos,mov);
    pair<pozitie,tranzitie>pereche_curenta=make_pair(poz,mov);

    table[pereche_curenta]+=
        alfa*
        (rewards[futurePos.first][futurePos.second]+
        gamma*table[make_pair(futurePos,getBestMoveEpsilon(table,futurePos,eps))]-
        table[pereche_curenta]);
}

void printPosition(pozitie poz)
{
    cout<<poz.first<<" "<<poz.second;
}
void printTransition(tranzitie tr)
{
    cout<<tr.first<<" "<<tr.second;
}
void runEpisode(Qtable &table)
{
    pozitie currPos=startingPoint;
    while (!checkFinal(currPos))
    {
        tranzitie bestMove=getBestMove(table,currPos);
        updateTable(table,currPos,bestMove);
        updatePosition(currPos,bestMove);
        /*printPosition(currPos);
       cout<<" ";
       printTransition(bestMove);
        cout<<endl;
*/
    }

}
void runEpisodeEpsilon(Qtable &table,double &eps)
{
    pozitie currPos=startingPoint;
    while (!checkFinal(currPos))
    {
        tranzitie bestMove=getBestMoveEpsilon(table,currPos,eps);
        updateTableEpsilon(table,currPos,bestMove,eps);
        updatePosition(currPos,bestMove);
        /*printPosition(currPos);
       cout<<" ";
       printTransition(bestMove);
        cout<<endl;
*/
    }

}
void trainTable(Qtable &table)
{
    int ep=0;
    while (ep<200)
    {
        runEpisode(table);
        ep++;

    }
}
void trainTableEpsilon(Qtable &table)
{
    int ep=0;
    double epsilon=1;
    while (ep<250)
    {
        runEpisodeEpsilon(table,epsilon);
        ep++;
        if (epsilon>0.1)epsilon-=0.05;
        //cout<<epsilon<<endl;
    }
}
void printSolution(Qtable &table)
{
    pozitie currPos=startingPoint;
    while(!checkFinal(currPos)){
        tranzitie bestMove=getBestMove(table,currPos);
        printPosition(currPos);
        cout<<" "<<getMoveName(bestMove)<<endl;
        updatePosition(currPos,bestMove);


    }
}

void printPolicy(Qtable &table)
{
for (int i=0;i<4;i++)
    {
        for (int j=0;j<12;j++)
        {
            for(int mov=0;mov<4;mov++)
            {
            pair<pozitie,tranzitie>pereche=make_pair(make_pair(i,j),tryAll(mov));
            if (getBestMove(table,pereche.first)==pereche.second)
              {
                  cout<<"P:";
                  printPosition(make_pair(i,j));
                  cout<<" M:"<<getMoveName(pereche.second)<<endl;
              }
            }
        }
    }

}
void printTable(Qtable table)
{
    for (int i=0;i<4;i++)
    {
        for (int j=0;j<12;j++)
        {
            for(int mov=0;mov<4;mov++)
            {
            pair<pozitie,tranzitie>pereche=make_pair(make_pair(i,j),tryAll(mov));
            if (validMove(pereche.first,pereche.second))
            {

            qterator qt=table.find(pereche);
            cout<<"P:";
            printPosition(pereche.first);
            cout<<" T:";
            cout<<getMoveName(pereche.second);
            //printTransition(pereche.second);
            cout<<" Q:"<<qt->second<<endl;
            }
            }
        }
    }
}

int main()
{
    for (int i=0;i<4;i++)
        for (int j=0;j<12;j++)
    {
        rewards[i][j]=-1;
    }

    for (int j=1;j<11;j++)
    {
        rewards[3][j]=-100;
    }
    rewards[3][11]=100;
    print_matrix(rewards);

    pozitie poz=make_pair(3,0);
    int currReward=0;

    initializeTable(table);

    trainTableEpsilon(table);

    printSolution(table);
    cout<<endl;
    cout<<"Policy:\n";
    printPolicy(table);
    return 0;
}
