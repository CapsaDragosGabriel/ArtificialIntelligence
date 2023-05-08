#include <iostream>
#include <bits/stdc++.h>
#include <algorithm>
#include <vector>
#include <list>
using namespace std;
int A,B,C;


typedef pair<int,int> state;
int noSteps=1;
int currStep=0;

bool checkDone( state st){
    if (st.first==C||st.second==C) return true;
    return false;
}
bool canFillJug(state st,int name){

    if (name==0)
        {if (st.first==A) return false;}
    else if (name==1)
       {if (st.second==B)return false;}

        return true;

}

state fillJug(state st,int name){
    state new_state;
    new_state=st;
    if(canFillJug(st,name))
    {
        if (name==0)
        new_state.first=A;
        else if(name==1)
        new_state.second=B;
    }

    return new_state;
}

//primeste stateul curent(prin k)
bool canEmptyJug(state st,int name){
    if (name==0)
        {if (st.first==0) return false;}
    else if (name==1)
        {if (st.second==0) return false;}
    return true;
}

state emptyJug(state st,int name){
    state new_state=st;
    if (canEmptyJug(st,name)) {
            if (name==0)
                new_state.first=0;
            else if (name==1)
                new_state.second=0;
        }
    return new_state;
}

bool canTransfer(state st,int from){
    if (from==0)
        {if (st.first!=0&&//daca am ceva in jug
            st.second!=B)//si celalalt nu e plin
                return true;}
    else if (from==1)
        {if (st.second!=0&&//la fel ca mai sus
            st.first!=A)
            return true;}
    return false;

}

state transfer(state st,int from){
    state new_state=st;
    if (canTransfer(st,from))
        {
            if (from==0){
                int toTransfer=new_state.first;//atat pot si voi transfera
                int dif=B-new_state.second;//atat am voie maxim sa transfer
                if(dif<toTransfer) toTransfer=dif;//daca am voie sa transfer mai putin, actualizez valoarea
                new_state.second=new_state.second+toTransfer;//adaug cat am putut sa transfer
                new_state.first=new_state.first-toTransfer;//scurg din cea din care am transferat aceeasi cantitate

            }
            else {
                int toTransfer=new_state.second;//atat pot si voi transfera
                int dif=A-new_state.first;//atat am voie maxim sa transfer
                if(dif<toTransfer) toTransfer=dif;//daca am voie sa transfer mai putin, actualizez valoarea
                new_state.first=new_state.first+toTransfer;//adaug cat am putut sa transfer
                new_state.second=new_state.second-toTransfer;//scurg din cea din care am transferat aceeasi cantitate

            }

        }

        return new_state;

}

void printStates(list<state> states){
     for (auto it= states.begin();it!=states.end();it++ )
    {
       state currState=*it;
       cout<<currState.first<<" "<<currState.second<<endl;
    }
}

int n=0;

bool stopBacktrack=0;


void showList(list<state> order){
    int k=0;
list<state>::iterator it;
for(it=order.begin();it!=order.end();++it)
    {
        state st=*it;
            cout<<k<<". "<<st.first<<" "<<st.second<<"\n";
    k++;
    }
cout<<endl;
}

bool checkList(list<state> order,state toFind){
list<state>::iterator it;
for(it=order.begin();it!=order.end();++it)
    {
        state st=*it;
            if ( st.first==toFind.first&& st.second==toFind.second)
                return true;
    }
return false;
}

void waterJugBTR(list<state> order )
{
    if(!stopBacktrack)
    for (int i=1;i<=6;i++)//trec prin toate tranzitiile
    {
        state new_state;
        new_state=order.back();

        switch(i){
    case 6:{new_state=emptyJug(new_state,0);break;}
    case 5:{new_state=emptyJug(new_state,1);break;}
    case 3:{new_state=fillJug(new_state,0);break;}
    case 4:{new_state=fillJug(new_state,1);break;}
    case 1:{new_state=transfer(new_state,0);break;}
    case 2: {new_state=transfer(new_state,1);break;}
        }

        if (!checkList(order,new_state))//verific sa nu mai am state-ul asta deja in lista
            {
            order.push_back(new_state);//il adaug in lista
            if (checkDone(new_state)){// daca nu am terminat pana acum si am gasit final state-ul
                   // stopBacktrack=true; //uncomment ptr o singura solutie
                showList(order);
                break;
            }
            else {
                waterJugBTR(order);
                if(order.back()!=make_pair(0,0))
                order.pop_back();//comment ptr o singura solutie
            }
            }


    }


}
void backtracking(){

 state st;
st.first=0;
st.second=0;

list<state>order;

order.push_front(st);
waterJugBTR(order);
}
int gcd(int a,int b)
{
 int result = min(a, b); // Find Minimum of a and b
    while (result > 0) {
        if (a % result == 0 && b % result == 0) {
            break;
        }
        result--;
    }
    return result;
}
bool doMath(){
if ( C% (gcd(A,B))==0 && A+B>C )
    {cout<<"The instance is solvable!\n";
    return true;}
else cout<<"The instance cannot be solved with any of the algorithms.\n";
return false;
}

void printSolutionBFS(map<state, state> mp, state u)//functie de afisare recursiva
{
if(currStep==0) noSteps=0;
    if (u.first == 0 && u.second == 0) {
        cout <<noSteps-currStep<<". "<< 0 << " " << 0 << endl;

        return;
    }
noSteps++;
currStep++;
    printSolutionBFS(mp, mp[u]);//merg in state-ul urmator
currStep--;
    cout <<noSteps-currStep<<". "<< u.first << " " << u.second << endl;
}

void BFS(){
    map<state,int> visited;
    bool solved=false;
    map<state,state> ramifications;
    queue<state> doNext;
    doNext.push(make_pair(0,0));//incep cautarea
    while(!doNext.empty())
    {
        auto u=doNext.front();
        doNext.pop();//iau primul nod si-l scot din coada

        if (visited[u]==1) continue;//daca am ajuns intr-un state parcurs deja, sar peste
         if ((u.first > A || u.second > B || u.first < 0
             || u.second < 0))
            continue;//daca am ajuns intr-un state invalid, sar peste


        visited[{u.first,u.second}]=1;

        if(checkDone(u)) //conditia de oprire
        {
            solved=true;

            printSolutionBFS(ramifications,u);

            return;
        }

        if(visited[fillJug(u,0)]!=1)//daca nu am verificat cazul in care umplu primul jug
        {
                state nextState=fillJug(u,0);

                doNext.push(nextState);//pun in coada acest caz  ca sa-l tratez
                ramifications[nextState]=u; //spun ca am ajuns in state-ul urmator din state-ul U
            }

        if(visited[fillJug(u,1)]!=1)//daca nu am verificat cazul in care umplu al doilea jug
            {
                state nextState=fillJug(u,1);

                doNext.push(nextState);//pun in coada acest caz  ca sa-l tratez
                ramifications[nextState]=u; //spun ca am ajuns in state-ul urmator din state-ul U
            }
        if (canTransfer(u,0)){
            if (visited[transfer(u,0)]!=1){//daca nu am verificat cazul in care transfer din primul
                state nextState=transfer(u,0);

                doNext.push(nextState);
                ramifications[nextState]=u;
                }
        }
        if (canTransfer(u,1)){
            if (visited[transfer(u,1)]!=1){//daca nu am verificat cazul in care transfer din al doilea jug
                state nextState=transfer(u,1);

                doNext.push(nextState);
                ramifications[nextState]=u;
            }
        }

        if (canEmptyJug(u,0)){
            if (visited[emptyJug(u,0)]!=1){//daca nu am verificat cazul in care am golit primul jug
                state nextState=emptyJug(u,0);

                doNext.push(nextState);
                ramifications[nextState]=u;
                }
        }
        if (canEmptyJug(u,1)){
            if (visited[emptyJug(u,1)]!=1){//daca nu am verificat cazul in care am golit al doilea jug
                state nextState=emptyJug(u,1);

                doNext.push(nextState);
                ramifications[nextState]=u;
                }
        }
    }
if(!solved)
    cout<<"Couldn't be solved :(.\n";
}

int heuristic(state st){
    return min(abs(st.first-C),abs(st.second-C));
}

state bestHeuristic(list<state> states){
    state bestState;
    int currHeuristic=999;
    for (auto it= states.begin();it!=states.end();++it )
    {

        if(heuristic(*it)<=currHeuristic)
        {
            state currState=*it;
            //cout<<currState.first<<" "<<currState.second<<endl;
            bestState.first=currState.first;
            bestState.second=currState.second;
            currHeuristic=heuristic(currState);

        }

    }
    return bestState;
}

void hillclimbing(){
    map<state,int> visited;
    bool solved=false;
    map<state,state> ramifications;
    list<state> doNext;
    list<state> parcurse;
    doNext.push_front(make_pair(0,0));//incep cautarea
    state currentState=make_pair(0,0);
    while(!doNext.empty() and heuristic(bestHeuristic(doNext))<= heuristic(currentState))
    {
        auto u=bestHeuristic(doNext);
        doNext.remove(u);
        parcurse.push_back(u);
        if (visited[u]==1) continue;//daca am ajuns intr-un state parcurs deja, sar peste
         if ((u.first > A || u.second > B || u.first < 0
             || u.second < 0))
            continue;//daca am ajuns intr-un state invalid, sar peste


        visited[{u.first,u.second}]=1;

        if(checkDone(u)) //conditia de oprire
        {
            solved=true;

            printSolutionBFS(ramifications,u);

            return;
        }

        if(visited[fillJug(u,0)]!=1)//daca nu am verificat cazul in care umplu primul jug
        {
                state nextState=fillJug(u,0);

                doNext.push_front(nextState);//pun in coada acest caz  ca sa-l tratez
                ramifications[nextState]=u; //spun ca am ajuns in state-ul urmator din state-ul U
            }

        if(visited[fillJug(u,1)]!=1)//daca nu am verificat cazul in care umplu al doilea jug
            {
                state nextState=fillJug(u,1);

                doNext.push_front(nextState);//pun in coada acest caz  ca sa-l tratez
                ramifications[nextState]=u; //spun ca am ajuns in state-ul urmator din state-ul U
            }
        if (canTransfer(u,0)){
            if (visited[transfer(u,0)]!=1){//daca nu am verificat cazul in care transfer din primul
                state nextState=transfer(u,0);

                doNext.push_front(nextState);
                ramifications[nextState]=u;
                }
        }
        if (canTransfer(u,1)){
            if (visited[transfer(u,1)]!=1){//daca nu am verificat cazul in care transfer din al doilea jug
                state nextState=transfer(u,1);

                doNext.push_front(nextState);
                ramifications[nextState]=u;
            }

        }

        if (canEmptyJug(u,0)){
            if (visited[emptyJug(u,0)]!=1){//daca nu am verificat cazul in care am golit primul jug
                state nextState=emptyJug(u,0);

                doNext.push_front(nextState);
                ramifications[nextState]=u;
                }
        }
        if (canEmptyJug(u,1)){
            if (visited[emptyJug(u,1)]!=1){//daca nu am verificat cazul in care am golit al doilea jug
                state nextState=emptyJug(u,1);

                doNext.push_front(nextState);
                ramifications[nextState]=u;
                }
        }
      currentState=u;
    }
if(!solved)
    {cout<<"s-a atins maximul local\n";
    showList(parcurse);
    }

}

int setSteps(state st,map<state,int> visited,int n)
{
    if(visited[st]<=n&& visited[st]!=0) return visited[st];
    else return n;
}
int noStepsBestAstar=0;
int currStepAstar=0;

list<state> saveSolutionGOOD(map<state, state> mp, state u)//functie de afisare recursiva
{
    state st=u;
    list<state> solution;
    while(st!=make_pair(0,0)){
        solution.push_front(st);
        st=mp[st];
    }
     solution.push_front(st);
    return solution;
}

state bestHeuristicA(list<state> states, map<state,int> visited){
    state bestState;
    int currHeuristic=999;
    for (auto it= states.begin();it!=states.end();++it )
    {
        state currState=*it;
        if(heuristic(currState)+visited[currState]<=currHeuristic)
        {

            //cout<<currState.first<<" "<<currState.second<<endl;
            bestState.first=currState.first;
            bestState.second=currState.second;
            currHeuristic=heuristic(currState)+visited[currState];

        }

    }
    return bestState;
}

void aStar(){
    map<state,int> visited;
    map<state,int> checked;
    bool solved=false;
    map<state,state> ramifications;
    list<state> doNext;
    list<state> solution;
    list<state>bestSolution;
    int step=0;
    doNext.push_front(make_pair(0,0));//incep cautarea
    visited[make_pair(0,0)]=0;
    checked[make_pair(0,0)]=0;
    while(!doNext.empty()&&
  (!solved||(solved&&heuristic(bestHeuristicA(doNext,visited))+visited[bestHeuristicA(doNext,visited)]<bestSolution.size())))
    {/**
    daca am rezolvat-o si pot aproxima ca lungimea unei noi solutii este mai mica decat
        lungimea celei mai bune solutii
      utilizand euristica si distanta parcursa pana atunci
     */



//iau nodul cu potentialul cel mai bun
       auto u=bestHeuristicA(doNext,visited);
        doNext.remove(u);
        step=visited[u];
        if(checked[u]!=0) continue;
         if ((u.first > A || u.second > B || u.first < 0
             || u.second < 0))
            continue;//daca am ajuns intr-un state invalid, sar peste

        checked[u]=1;
       // cout<<u.first<<" "<<u.second<<" steps:"<<visited[u]<<" heuristic:"<<heuristic(u)<<"\n";
        if(checkDone(u)) //conditia de oprire
        {
            solved=true;
            if (bestSolution.empty())//daca e prima solutie pe care am gasit-o
               {bestSolution= saveSolutionGOOD(ramifications,u);

               }
            else {
                solution=saveSolutionGOOD(ramifications,u);

                if (bestSolution.size()>solution.size())//verific sa fie o solutie mai buna
                    bestSolution=solution;
            }
        }

        if(visited[fillJug(u,0)]==0||visited[fillJug(u,0)]>step+1)//daca nu am verificat cazul in care umplu primul jug
        {
                state nextState=fillJug(u,0);
                doNext.push_front(nextState);//pun in coada acest caz  ca sa-l tratez

                if (visited[nextState]==0|| visited[nextState]>step+1)
                {visited[nextState]=setSteps(nextState,visited,step+1);
                ramifications[nextState]=u;} //spun ca am ajuns in state-ul urmator din state-ul U
        }

        if(visited[fillJug(u,1)]==0||visited[fillJug(u,1)]>step+1)//daca nu am verificat cazul in care umplu al doilea jug
            {
                state nextState=fillJug(u,1);
                 doNext.push_front(nextState);//pun in coada acest caz  ca sa-l tratez
                //      cout<<"state urmator: "<<nextState.first<<" "<<nextState.second<<" SE INCEARCA UMPLERE2\n";
                if (visited[nextState]==0|| visited[nextState]>step+1)
                {visited[nextState]=setSteps(nextState,visited,step+1);
                ramifications[nextState]=u;} //spun ca am ajuns in state-ul urmator din state-ul U
            }
        if (canTransfer(u,0)){
            if (visited[transfer(u,0)]==0||visited[transfer(u,0)]>step+1){//daca nu am verificat cazul in care transfer din primul
                state nextState=transfer(u,0);
                doNext.push_front(nextState);

                if (visited[nextState]==0|| visited[nextState]>step+1)
                {visited[nextState]=setSteps(nextState,visited,step+1);
                ramifications[nextState]=u;} //spun ca am ajuns in state-ul urmator din state-ul U
                }
        }
        if (canTransfer(u,1)){
            if (visited[transfer(u,1)]==0||visited[transfer(u,1)]>step+1){//daca nu am verificat cazul in care transfer din al doilea jug
                state nextState=transfer(u,1);
                doNext.push_front(nextState);
                 if (visited[nextState]==0|| visited[nextState]>step+1)
                {visited[nextState]=setSteps(nextState,visited,step+1);
                ramifications[nextState]=u;} //spun ca am ajuns in state-ul urmator din state-ul U

            }
        }

        if (canEmptyJug(u,0)){
            if (visited[emptyJug(u,0)]==0||visited[emptyJug(u,0)]>step+1){//daca nu am verificat cazul in care am golit primul jug
                state nextState=emptyJug(u,0);
                doNext.push_front(nextState);
              //  cout<<"SE INCEARCA golire\n";

         if (visited[nextState]==0|| visited[nextState]>step+1)
                {visited[nextState]=setSteps(nextState,visited,step+1);
                ramifications[nextState]=u;} //spun ca am ajuns in state-ul urmator din state-ul U
                }
        }
        if (canEmptyJug(u,1)){
            if (visited[emptyJug(u,1)]==0||visited[emptyJug(u,1)]>step+1){//daca nu am verificat cazul in care am golit al doilea jug
                state nextState=emptyJug(u,1);
                doNext.push_front(nextState);
                 if (visited[nextState]==0|| visited[nextState]>step+1)
                {visited[nextState]=setSteps(nextState,visited,step+1);
                ramifications[nextState]=u;} //spun ca am ajuns in state-ul urmator din state-ul U

                }
        }

}
if(!solved)
    cout<<"POC\n";
else
    showList(bestSolution);
}
void initialize(){
     cin>>A>>B>>C;
    cout<<"One jug can hold up to: "<<A<<endl;
    cout<<"The other jug can hold up to: "<<B<<endl;
    cout<<"You need to get: "<<C<<endl;

     cout<<"Initial state: "<<0<<" "<<0<<endl;
}
void menu(){
    int method=0;
cout<<"\nYou can choose what method to use in order to solve the problem:"<<endl;
cout<<"0.Check if solvable\n";
cout<<"1.Backtracking"<<endl;
cout<<"2.BFS\n";
cout<<"3.Hillclimbing\n";
cout<<"4.A*\n";
cout<<"9.Exit program\n";
cin>>method;
switch(method){
case 0:
    doMath();
    break;
case 1:
    backtracking();
    break;
case 2:
    BFS();
    break;
case 3:
    hillclimbing();
    break;
case 4:
    aStar();
    break;
case 9:
    return;
default:
    cout<<"That is not a valid method, please, try again.\n";
}
menu();
}
int main()
{


initialize();
/*
list<state> states;
state st0=pair<int,int>(0,0);
state st1=pair<int,int>(1,1);
states.push_front(state(1,1));
state st2=pair<int,int>(2,3);
state st3=pair<int,int>(1,0);
states.push_front(st0);
states.push_front(st1);
states.push_front(st2);
states.push_front(st3);
state bestState=bestHeuristic(states);
printStates(states);

cout<<endl;
printStates(states);
//printStates(states);
*/
menu();

     return 0;
}

