#include <iostream>
#include <fstream>
#include <list>
#include <map>
using namespace std;
ifstream f("date.in");
int n,blocks[30][30];
typedef pair<int,int> locatie;
typedef struct state
 {int queens[30][30];};
state init;
/*
void processFile(){
fstream my_file;
fstream processed_data;
my_file.open("transform.param",ios::in);
processed_data.open("date.in",ios::out);

	if (!my_file) {
		cout << "No such file";
	}
	else {
		char ch;

		while (1) {
			ch=my_file.get();
			if (my_file.eof())
            {cout<<ch;
				break;}
        //if(ch>='0'&&ch<='9'||ch==' ')
          /*  for (int j=0;*ch[j];j++)
            {
                if (ch[j]==']');
            }
			cout << ch;
		}

	}
	my_file.close();
processed_data.close();
}*/

void initialize()//citesc din fisier marimea tablei si pozitionarea blocurilor
{
f>>n;
int a,b;



while(f>>a)
{
    if (a==99999) break;
    f>>b;
   // cout<<a<<" "<<b<<endl;
    blocks[a][b]=-1;
    init.queens[a][b]=-1;
}

while(f>>a)
{
    if (a==99999) break;
    f>>b;
    init.queens[a][b]=3;
}

}


locatie getQueenPos(state st,int whichQueen)//verific daca o regina a fost plasata si ii iau pozitia
{
    locatie loc;
    loc.first=0;
    loc.second=0;
    for(int i=1;i<=n;i++)
        if (st.queens[whichQueen][i]!=0 && st.queens[whichQueen][i]!=-1)
            {loc.second=i;
            loc.first=whichQueen;
            return loc;
            }
    return loc;
}

list<locatie> possibleLocations(state st,int whichQueen)//fac o lista cu miscarile pe care o regina le are la dispozitie la un moment dat
{
    list<locatie> locatii;
    locatie loc;
    loc=getQueenPos(st,whichQueen);
    if (loc.first!=0 && loc.second!=0)
        {locatii.push_back(loc);
    return locatii;}
    for (int i=1;i<=n;i++)
        if (st.queens[whichQueen][i]==0) {locatii.push_back(make_pair(whichQueen,i));
        }

  return locatii;
}

void printDomains(state st){
for (int i=1;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
            cout<<st.queens[i][j]<<" ";
    cout<<endl;
    }
cout<<endl;
cout<<"Sau:\n";
for (int i=1;i<=n;i++)
    cout<<i<<": "<<getQueenPos(st,i).second<<endl;
}


state refreshDomains(state st)//actualizez domeniile dupa ce pun o regina
{/**blocks*/
    for(int i=1;i<=n;i++)
    {
        locatie loc=getQueenPos(st,i);

        for (int j=1;j<=n;j++){
                for (int k=1;k<=n;k++)
            {
                if (loc.first!=0 and loc.second!=0)
                {
                if(j==loc.first&&k!=loc.second)st.queens[j][k]=-1;
                if(k==loc.second&&j!=i)st.queens[j][k]=-1;
                if(abs(j-loc.first)==abs(k-loc.second) && st.queens[j][k]!=3) st.queens[j][k]=-1;

                }
            }
        }

    }

    return st;
}

bool checkDone(state st){
for (int i=1;i<=n;i++)
    if(getQueenPos(st,i).second==0) return false;//daca nu am plasat una dintre regine atunci n-am terminat
return true;
}
int stopFC=0;

void forwardChecking(state st,int queen,list<locatie> locatii)
{

int ok=1;
    for (auto it = locatii.begin(); it != locatii.end(); ++it){
        locatie loc=*it;
        state copystate=st;
        copystate.queens[loc.first][loc.second]=3;
        copystate=refreshDomains(copystate);
        if (checkDone(copystate))
            {printDomains(copystate);
            stopFC=1;}
        else if (stopFC==0){
        for (int i=1;i<=n&&ok==1;i++)
            if (possibleLocations(copystate,i).empty())
                ok=0;

        if (ok)
            forwardChecking(copystate,queen+1,possibleLocations(copystate,queen+1));
    }
    }
}
void printList(list<locatie> locatii)//folosita pentru afisare
{
    for (auto it = locatii.begin(); it != locatii.end(); ++it){
        locatie l=*it;
        cout<<l.first<< ","<<l.second<< " ";
}

}
void runFC(state st)
{
    list<locatie> locatii=possibleLocations(st,1);
   // printList(locatii);
    forwardChecking(st,0,locatii);
    if (!stopFC) cout<<"Couldn t find a solution.";
}

int getNextQueen(state st, map <int,list<locatie>> toSort)//imi da prima regina cu cele mai putine miscari disponibile
{
    int nextQueen=0;
    int minMoves=9999;
for (int i=1;i<=n;i++)
{
    locatie currLocation=getQueenPos(st,i);

    if (toSort[i].size()<minMoves && currLocation.second==0)
    {minMoves=toSort[i].size();
    nextQueen=i;
    }
}

 return nextQueen;
}
void printLocations(map <int,list<locatie>> allLocations)//debugging purposes
{
    for (int i=1;i<=n;i++)
     {cout<<i<<": ";
         for (auto it = allLocations[i].begin(); it != allLocations[i].end(); ++it){
        {locatie loc=*it;
    cout<<loc.second;
    }
     }
    cout<<endl;
    }

}

void setLocations(state st,map <int,list<locatie>> &allLocations)//functie de actualizare a pozitiilor disponibile
{
      for (int i=1;i<=n;i++)
        allLocations[i]=possibleLocations(st,i);
}
void forwardCheckingMRV(state st,int queen,map<int,list<locatie>>allLocations)
{

int ok=1;
    for (auto it = allLocations[queen].begin(); it != allLocations[queen].end(); ++it){
        locatie loc=*it;
        state copystate=st;
        copystate.queens[loc.first][loc.second]=3;
        copystate=refreshDomains(copystate);

        if (checkDone(copystate))
            {
                printDomains(copystate);
            stopFC=1;}
        else if (stopFC==0){

        for (int i=1;i<=n&&ok==1;i++)
            if (allLocations[i].empty())
                ok=0;

        if (ok)
        {map<int,list<locatie>>tempLocations;//creez un obiect temporar ca sa nu modific lista pe care se lucreaza deja(eroare la memorie)
            setLocations(copystate,tempLocations);//actualizez pozitiile disponibile
            forwardCheckingMRV(copystate,getNextQueen(copystate,tempLocations),tempLocations);//noua iteratie pe prima regina pe care trebuie sa o mut
            }
    }
    }
}

void runMRV(state st)//setup MRV+FC
{

map <int,list<locatie>> allLocations;
  setLocations(st,allLocations);
stopFC=0;

    forwardCheckingMRV(st,getNextQueen(st,allLocations),allLocations);
}

int main()
{
    initialize();
    printDomains(init);
    runMRV(init);
    cout<<endl;
    stopFC=0;
    runFC(init);

    return 0;
}
