#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib>
#include <math.h>
#include <tgmath.h>
#include <bits/stdc++.h>
using namespace std;
ifstream f("iris.data");
struct flower{
    double s_len;
    double s_wid;
    double p_len;
    double p_wid;
    string name;
}instances[200],test[200];

int totalTrain=0;
int totalTest=0;
struct network{
    double firstLayer[4];
    /*double i_slen;
    double i_swid;
    double i_plen;
    double i_pwid;*/

    map<std::pair<int, int>, double> iToHiddenWeights;

    double hiddenLayer[4];
    /*double h_slen=0;
    double h_swid=0;
    double h_plen=0;
    double h_pwid=0;
*/

    map<std::pair<int, int>, double> hToToOutputWeights;
    //map<perecheOutput,double> hToOutputWeights;

    int epoci=10;//0.3
    double learningRate=0.15;
    double output[3];
    int desiredOutput[3];

    double errorO[3];
    double errorH[4];
    double biasI[4];
    double biasH[4];

    double biasGradI;
    double biasGradH;
    double gradientH[4];
    double errorI[4];
    double gradientI[4];
    double bigError=0;

}retea;
typedef std::map<std::pair<int, int>,double>::iterator mapIterator;

void printInstance(flower flo){
    cout<<flo.s_len<<" "<<flo.s_wid<<" "<<flo.p_len<<" "<<flo.p_wid<<" "<<flo.name<<endl;
}
void parseInput(){
    string currPlant;
    int k=0;
    int j=0;
    while(getline(f,currPlant))
    {
        if (k%5!=0)
        {
        instances[k-j].s_len=stof(currPlant.substr(0,3));
        instances[k-j].s_wid=stof(currPlant.substr(4,3));
        instances[k-j].p_len=stof(currPlant.substr(8,3));
        instances[k-j].p_wid=stof(currPlant.substr(12,3));
        instances[k-j].name=currPlant.substr(16);
       // printInstance(instances[k-j]);
        }
       else {
        test[j].s_len=stof(currPlant.substr(0,3));
        test[j].s_wid=stof(currPlant.substr(4,3));
        test[j].p_len=stof(currPlant.substr(8,3));
        test[j].p_wid=stof(currPlant.substr(12,3));
        test[j].name=currPlant.substr(16);
        j++;
       }
        k++;
       // cout<<"AAAA"<< stof(currPlant.substr(4,2));
       // cout<<instances[k].s_wid<<endl;*/
    }
    unsigned seed = 0;
    int lenTrain=k-j;
    totalTrain=lenTrain;
    // Shuffling our array
    shuffle(instances, instances + lenTrain,
            default_random_engine(seed));

    int lenTest=j;
    totalTest=lenTest;
    shuffle(test,test+j,default_random_engine(seed));

}


network createNetwork(flower flow){
    network reteaTemp;
    reteaTemp.firstLayer[0]=flow.p_len;
    reteaTemp.firstLayer[1]=flow.p_wid;
    reteaTemp.firstLayer[2]=flow.s_len;
    reteaTemp.firstLayer[3]=flow.s_wid;
     for (int i=0;i<4;i++)
    reteaTemp.hiddenLayer[i]=0;


    for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
        {
            reteaTemp.iToHiddenWeights.emplace(make_pair(i,j),double(rand()%100)/100);
        }

    for (int i=0;i<4;i++)
        for (int j=0;j<3;j++)
        {
            reteaTemp.hToToOutputWeights.emplace(make_pair(i,j),double(rand()%100)/100);
        }
    reteaTemp.biasI[0]=double((rand()%100)/100);
    reteaTemp.biasI[1]=double((rand()%100)/100);
    reteaTemp.biasI[2]=double((rand()%100)/100);
    reteaTemp.biasI[3]=double((rand()%100)/100);

    reteaTemp.biasH[0]=double((rand()%100)/100);
    reteaTemp.biasH[1]=double((rand()%100)/100);
    reteaTemp.biasH[2]=double((rand()%100)/100);



    for (int i=0;i<3;i++)
        reteaTemp.output[i]=0;

    if (flow.name=="Iris-setosa")
        {
        reteaTemp.desiredOutput[0]=1;
        reteaTemp.desiredOutput[1]=0;
        reteaTemp.desiredOutput[2]=0;
        }
    else if(flow.name=="Iris-versicolor"){
        reteaTemp.desiredOutput[0]=0;
        reteaTemp.desiredOutput[1]=1;
        reteaTemp.desiredOutput[2]=0;
    }
    else if (flow.name=="Iris-virginica") {
        reteaTemp.desiredOutput[0]=0;
        reteaTemp.desiredOutput[1]=0;
        reteaTemp.desiredOutput[2]=1;
    }
    else{
        reteaTemp.desiredOutput[0]=0;
        reteaTemp.desiredOutput[1]=0;
        reteaTemp.desiredOutput[2]=0;
    }
    for (int i=0;i<4;i++)
    {
        reteaTemp.errorH[i]=0;
        reteaTemp.errorI[i]=0;
    }
    for (int i=0;i<3;i++)
    {
        reteaTemp.errorO[i]=0;
    }
    return reteaTemp;

}

void updateNetwork(network &reteaTemp,flower flow)//pastrez tot inafara de input si desired output
{
    reteaTemp.firstLayer[0]=flow.p_len;
    reteaTemp.firstLayer[1]=flow.p_wid;
    reteaTemp.firstLayer[2]=flow.s_len;
    reteaTemp.firstLayer[3]=flow.s_wid;

    if (flow.name=="Iris-setosa")
        {
        reteaTemp.desiredOutput[0]=1;
        reteaTemp.desiredOutput[1]=0;
        reteaTemp.desiredOutput[2]=0;
        }
    else if(flow.name=="Iris-versicolor"){
        reteaTemp.desiredOutput[0]=0;
        reteaTemp.desiredOutput[1]=1;
        reteaTemp.desiredOutput[2]=0;
    }
    else if (flow.name=="Iris-virginica") {
        reteaTemp.desiredOutput[0]=0;
        reteaTemp.desiredOutput[1]=0;
        reteaTemp.desiredOutput[2]=1;
    }
    else{
        reteaTemp.desiredOutput[0]=0;
        reteaTemp.desiredOutput[1]=0;
        reteaTemp.desiredOutput[2]=0;
    }


}
double derivata(double value)
{
    return value*(1.0-value);
}
double sigmoidFunction(double value1)
{
    double value2;
   value2=1/(1+exp(-value1));
  // value2=derivata(value2);
//   value2*=weight;

   return value2;
}

void updateError(network &myNetwork)
{
double bigError=0;

    for(int i=0;i<3;i++)
        myNetwork.errorO[i]=-(myNetwork.desiredOutput[i]-myNetwork.output[i])  ;

    for(int i=0;i<3;i++){
        double value=0.5*pow(myNetwork.errorO[i],2);
        bigError+=value;

    }
    myNetwork.bigError+=bigError;

}
void forwardPropagation(network &myNetwork)
{
  for (int i=0;i<4;i++)
  {

        for (int j=0;j<4;j++)
        {
            std::map<std::pair<int, int>,double>::iterator it = myNetwork.iToHiddenWeights.find(make_pair(i,j));

            //cout<<"ar trebui sa adun"<<sigmoidFunction(myNetwork.firstLayer[i],it->second)<<endl;
            //adun la fiecare nod din stratul ascuns produsul dintre nodul curent din stratul de intrare si ponderea pe care o are
            //cu nodul corespunzator din stratul ascuns
            myNetwork.hiddenLayer[i]+=myNetwork.firstLayer[i]*it->second;

        }
        myNetwork.hiddenLayer[i]+=myNetwork.biasI[i];


  }

for (int i=0;i<4;i++)
{
    myNetwork.hiddenLayer[i]=sigmoidFunction(myNetwork.hiddenLayer[i]);
}

    for (int i=0;i<3;i++)
        {
            for (int j=0;j<4;j++)
        {

            std::map<std::pair<int, int>,double>::iterator it = myNetwork.hToToOutputWeights.find(make_pair(i,j));

            //adun la fiecare nod din stratul ascuns produsul dintre nodul curent din stratul ascuns si ponderea pe care o are
            //cu nodul corespunzator din stratul de iesire

            myNetwork.output[i]+=myNetwork.hiddenLayer[i]*it->second;
        }
        myNetwork.output[i]+=myNetwork.biasH[i];

    }
for (int i=0;i<3;i++)
{
    myNetwork.output[i]=sigmoidFunction(myNetwork.output[i]);
}
   updateError(myNetwork);

}

void printNetwork(network myNetwork)
{
    for (int i=0;i<4;i++)
    {
        cout<<myNetwork.firstLayer[i]<<" er:"<<myNetwork.errorI[i]<<"   ";
        cout<<"iToH:";
        for(int j=0;j<4;j++)
        {
            std::map<std::pair<int, int>,double>::iterator it = myNetwork.iToHiddenWeights.find(make_pair(i,j));
            cout<<it->second<<",";
        }
        cout<<"    "<<myNetwork.hiddenLayer[i]<<"  er:"<<myNetwork.errorH[i]<<"   ";
        cout<<"hToO:";
        for(int j=0;j<3;j++)
        {
            std::map<std::pair<int, int>,double>::iterator it = myNetwork.hToToOutputWeights.find(make_pair(i,j));
            cout<<it->second<<",";
        }
        cout<<endl;

    }
    for(int i=0;i<3;i++)
        cout<<"Output layer"<<i<<":"<<myNetwork.output[i]<<"   er:"<<myNetwork.errorO[i]<<"      "<<"desired:"<<myNetwork.desiredOutput[i]<<endl;

    cout<<"Eroarea totala la output:"<<myNetwork.bigError<<endl;
}

void backpropagation(network &myNetwork)
{
 //   updateError(myNetwork);
// cout<<"inainte\n";
 // printNetwork(myNetwork);
    for (int i=0;i<4;i++)
    {
        double temp=0;
        myNetwork.errorH[i]=derivata(myNetwork.hiddenLayer[i]);
        for (int j=0;j<3;j++)
            {
                std::map<std::pair<int, int>,double>::iterator it = myNetwork.hToToOutputWeights.find(make_pair(i,j));
                temp=temp+myNetwork.errorO[j]*it->second;
            }
        myNetwork.errorH[i]*=temp;
    }
    for (int i=0;i<4;i++)
    {
        double temp=0;
        for (int j=0;j<3;j++)
            temp+=myNetwork.errorO[j]*myNetwork.hiddenLayer[i];

        myNetwork.gradientH[i]=temp;
    }

    myNetwork.biasGradH=0;
    for (int i=0;i<3;i++)
        myNetwork.biasGradH+=myNetwork.errorO[i];


    for (int i=0;i<4;i++)
    {

        for( int j=0;j<3;j++)
        {
        mapIterator it=myNetwork.hToToOutputWeights.find(make_pair(i,j));
        it->second-=myNetwork.learningRate*myNetwork.gradientH[i];
        }
        //myNetwork.biasH[i]-=myNetwork.learningRate*myNetwork.biasGradH;
    }

    for (int j=0;j<3;j++)
    {
    myNetwork.biasH[j]-=myNetwork.learningRate*myNetwork.biasGradH;

    }



    for (int i=0;i<4;i++)
    {
        double temp=0;
        for (int j=0;j<4;j++)
            temp+=myNetwork.errorH[j]*myNetwork.firstLayer[i];

        myNetwork.gradientI[i]=temp;
    }
    myNetwork.biasGradI=0;
    for (int i=0;i<4;i++)
        myNetwork.biasGradI+=myNetwork.errorH[i];

    for (int i=0;i<4;i++)
    {

        for( int j=0;j<4;j++)
        {
        mapIterator it=myNetwork.iToHiddenWeights.find(make_pair(i,j));

        it->second-=myNetwork.learningRate*myNetwork.gradientI[i];
        }
        myNetwork.biasI[i]-=myNetwork.learningRate*myNetwork.biasGradI;

    }
  /*  cout<<"\ndupa\n";
    printNetwork(myNetwork);
cout<<endl;
cout<<endl;
cout<<endl;
cout<<endl;*/
}
int maxi(double vect[4])
{
    double temp=0;
    int index=-1;
    for (int i=0;i<4;i++)
    {
        if (vect[i]>temp)
        {
            temp=vect[i];
            index=i;
        }
    }
    return index;
}
pair<int,int> prediction(network myNetwork){
    int corecte=0;
    for (int i=0; i<totalTest/*test[i].p_len*/;i++)
    {
        updateNetwork(myNetwork,test[i]);
        forwardPropagation(myNetwork);
//printNetwork(myNetwork);
        int indexMax= maxi(myNetwork.output);
      /**
        if (flow.name=="Iris-setosa")
        {
        reteaTemp.desiredOutput[0]=1;
        reteaTemp.desiredOutput[1]=0;
        reteaTemp.desiredOutput[2]=0;
        }
        */


        if (indexMax==0 && test[i].name=="Iris-setosa") corecte++;
        else if (indexMax==1 && test[i].name=="Iris-versicolor") corecte++;
            else if (indexMax==2 && test[i].name=="Iris-virginica") corecte++;
    }
    return make_pair(corecte,totalTest-corecte);
}
void runNetwork(network &myNetwork){
   // for (int n=0;n<myNetwork.epoci;n++)
   for (int epoca=0;epoca<myNetwork.epoci;epoca++)
    {
        myNetwork.bigError=0;
        for (int i=1; instances[i].p_len/*test[i].p_len*/;i++)
    {
        updateNetwork(myNetwork,instances[i]);
        forwardPropagation(myNetwork);
        backpropagation(myNetwork);
    }
    printNetwork(myNetwork);
   pair<int,int> tempPair=prediction(myNetwork);
   cout<<"Epoca["<<epoca<<"]:"<<tempPair.first<<" corecte, "<<tempPair.second<<" gresite.\n";
    }

}


int main()
{
    parseInput();
    cout<<"DATE DE TESTARE\n";

    network myNetwork;
    myNetwork=createNetwork(instances[0]);
    cout<<endl<<test[0].name<<" "<<test[0].p_len<<" "<<test[0].p_wid<<" "<<test[0].s_len<<" "<<test[0].s_wid<<endl;
    printNetwork(myNetwork);
    forwardPropagation(myNetwork);

    runNetwork(myNetwork);

    return 0;
}
