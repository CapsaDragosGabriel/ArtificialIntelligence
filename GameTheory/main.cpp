#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <list>
//def starea finala, def starea initiala, def tranzitiile
using namespace std;
ifstream f("date2.in");
void printVector(int moves[100])
{
    int i=0;
    while(moves[i])cout<<moves[i++];
}

struct Player{
    int moves[100][100];
}p1,p2;
int nr1,nr2;
typedef pair<int,int> combo;

typedef pair<int,int> punctaj;
void printList(list<combo> lc){
for (auto it=lc.begin();it!=lc.end();++it)
{
   combo c=*it;
   cout<<c.first<<" "<<c.second<<"   ";
}
}
void initialize(Player &p1,Player &p2){
    string firstLine;
       int i=0;
       int k=0;
       int l=0;
       getline(f,firstLine);
       getline(f,firstLine);

    while ( getline(f,firstLine))
    {
        i=0;
        l=0;
        while (i<firstLine.length())
        {
            int pointsA=0;
            int pointsB=0;
            int j=i;
            while(firstLine[j]!=',')
                {

                    pointsA=pointsA*10+atoi(&firstLine[j]);
                    j++;
                }

            p1.moves[k][l]=pointsA;

            j++;
            while(firstLine[j]>='0'&&firstLine[j]<='9')
            {
                pointsB=pointsB*10+atoi(&firstLine[j]);
                j++;
            }
            p2.moves[k][l]=pointsB;
          //  cout<<pointsA<<" "<<pointsB<<endl;
            l++;
           i=j+1;
        }
        if(l>nr2) nr2=l;


    k++;
            if(k>nr1) nr1=k;
    }
    for (int i=0;i<nr2;i++)
        p2.moves[k-1][i]/=10;
}
void printPunct(punctaj punct){
cout<<punct.first<<" "<<punct.second<<endl;
}
punctaj miscari(Player p1,Player p2,int move1, int move2){
    punctaj punct;
        punct=make_pair(p1.moves[move1][move2],p2.moves[move1][move2]);
    return punct;

}

list<combo> getResponseP1(){

    list<combo> lc;

    for (int j=0;j<nr2;j++){
    int maxPunctaj=0;
    combo c;
        for(int i=0;i<nr1;i++){
        if(p1.moves[i][j]>maxPunctaj)
        {
            maxPunctaj=p1.moves[i][j];
            c.first=i;
            c.second=j;
        }
        }
        for(int i=0;i<nr1;i++){
        if(p1.moves[i][j]==maxPunctaj)
        {
            combo nc=make_pair(i,j);
             lc.push_front(nc);
        }
        }
        }
       // printList(lc);
        return lc;

}


list<combo> getResponseP2(){

    list<combo> lc;

    for (int i=0;i<nr1;i++)
        {int maxPunctaj=0;
           combo c;
        for(int j=0;j<nr2;j++){
        if(p2.moves[i][j]>maxPunctaj)
        {
            maxPunctaj=p2.moves[i][j];
            c.first=i;
            c.second=j;
        }
        }
        for(int j=0;j<nr2;j++){
        if(p2.moves[i][j]==maxPunctaj)
        {
           combo nc=make_pair(i,j);
           lc.push_front(nc);
        }
        }
        }
        //        printList(lc);
return lc;
}
list<combo> getBestScoreP1(){//determin raspunsurile ptr player2 in functie de player2
    int maxPunctaj=0;
    list<combo> lc;
    combo c;
    for (int i=0;i<nr1;i++)
        for(int j=0;j<nr2;j++){
        if(p1.moves[i][j]>maxPunctaj)
        {
            maxPunctaj=p1.moves[i][j];
            c.first=i;
            c.second=j;
        }
        }
    for (int i=0;i<nr1;i++)
        for(int j=0;j<nr2;j++){
            if (miscari(p1,p2,i,j).first==miscari(p1,p2,c.first,c.second).first)
                {   combo nc=make_pair(i,j);
                    lc.push_front(nc);
                }
        }
   // cout<<"MAX PUNCTAJ:"<<maxPunctaj<<endl;
    return lc;
}
list<combo> getBestScoreP2(){//determin raspunsurile ptr player2 in functie de player1
   int maxPunctaj=0;
    list<combo> lc;
    combo c;
    for (int i=0;i<nr1;i++)
        for(int j=0;j<nr2;j++){
        if(p2.moves[i][j]>maxPunctaj)
        {
            maxPunctaj=p2.moves[i][j];
            c.first=i;
            c.second=j;
        }
        }
    for (int i=0;i<nr1;i++)
        for(int j=0;j<nr2;j++){
            if (miscari(p1,p2,i,j).second==miscari(p1,p2,c.first,c.second).second)
                {   combo nc=make_pair(i,j);
                    lc.push_front(nc);
                }
        }
    //cout<<"MAX PUNCTAJ:"<<maxPunctaj<<endl;
    return lc;
}


bool NashEQ(){
    list<combo> lc1=getResponseP1();
    list<combo> lc2=getResponseP2();
  //  printList(lc1);
    int ok=0;
  //  cout<<endl;
  //  printList(lc2);
    for (auto it:lc1)
        {
           combo c=it;
           for (auto it2:lc2)
           {
               combo c2=it2;
               if (c==c2)
               {
                   if (ok==0)cout<<"Nash equilibria are: "<<c.first<<" "<<c.second<<"    ",ok=1;
                   else cout<<c.first<<" "<<c.second<<"    ";
               }
           }
        }
    if(!ok)cout<<"There is no nash EQ.\n";
    return ok;
}



int dominantStrategy(Player &p){

     int tempScores[100];
  if (equal(begin(p1.moves),end(p1.moves),begin(p.moves)))  //daca sunt cu primul jucator
        for(int i=0;i<nr1;i++)
        {
            int ok=1;
            for (int j=0;j<nr2 and ok;j++)
                {
                   int punctajCurent=p.moves[i][j];
                for (int k=0;k<nr1 and ok;k++)
                {
                    if (p.moves[k][j]>punctajCurent)
                        ok=0;
                }
            }
            if (ok)
            {cout<<"Best set of moves for player 1 is : " <<i<<endl;
            return i;
            }
        }
    else
        for(int i=0;i<nr2;i++)
        {
            int ok=1;
            for (int j=0;j<nr1 and ok;j++)
                {
                   int punctajCurent=p.moves[j][i];
                for (int k=0;k<nr1 and ok;k++)
                {
                    if (p.moves[j][k]>punctajCurent)
                        ok=0;
                }



                }
            if (ok)
            {cout<<"Best set of moves for player 2 is : " <<i<<endl;
            return i;
            }
        }
cout<<"There s no dominant strategy for this player.\n";
        return -1;
}
int main()
{
    initialize(p1,p2);
   dominantStrategy(p1);
   dominantStrategy(p2);
NashEQ();
    return 0;
}
