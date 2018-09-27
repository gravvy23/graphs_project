#include "zestaw2.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

/////////////////////////////////////////////////////////////////
View_Graph::View_Graph(int n, int edges){
    nodes = n;
    if (edges < nodes -1 || edges > (nodes*(nodes-1)/2)){
        std::cout << "dla podanej liczby krawedzi nie mozna wygenerowac grafu spojnego"<<std::endl;
        exit(EXIT_FAILURE);
    }
    neighTab = new int*[nodes];
    for (int i=0;i<nodes;i++){
        neighTab[i] = new int[nodes];
    }

    srand(time(NULL));
    int i,j,x;
    do {
        for (int i=0;i < nodes; ++i){
            for (int j=0; j < nodes; ++j){
                neighTab[i][j] = 0;
            }
        }
        x = edges;
        while (x > 0){
            i = std::rand()%nodes;
            j = std::rand()%nodes;
            if (!neighTab[i][j] && i!=j) {
                neighTab[i][j] = 1;
                neighTab[j][i] = 1;
                --x;
            }
        }
    }   while (!IsConsistent());

    Save();
}

/////////////////////////////////////////////////////////////////
View_Graph::~View_Graph(){
    for (int i=0;i<nodes;i++)
        delete[] neighTab[i];
    delete[] neighTab;
}

/////////////////////////////////////////////////////////////////
void View_Graph::Save(){
    std::ofstream my_stream("tab.tmp");
    if (!my_stream){
        std::cout<<"blad otwarcia pliku"<<std::endl;
        exit(EXIT_FAILURE);
    }
    for (int i=0; i< nodes; i++){
        for (int j=0; j< nodes; j++){
            my_stream << neighTab[i][j] <<" ";
        }
        my_stream<<"\n";
    }
    my_stream.close();
}

/////////////////////////////////////////////////////////////////
void View_Graph::MaxConsistent(int i){
    static int k;   // counting numers of nodes in graph consistent
    if(colorTab[0]==0)   // if i start searchning
    {
        for(i=0;i<nodes;++i)  // loop throuh records in neighTab[] invoke max_consistent for each record
        {
            if(colorTab[i]==0)
            {
                colorTab[i]=1;
                k=1;
                for(int j=0 ; j < nodes ; ++j)     // loop through columns in each record 
                {
                    if(neighTab[i][j]) MaxConsistent(j);
                }
            }
        }
    }

    else if(colorTab[i]==0)  // when i know which record (node) i am currently
    {   
        colorTab[i]=++k;     // incrementing amount of consistent nodes
        for(int j=0; j < nodes; ++j)
        {
            if(neighTab[i][j]) MaxConsistent(j);      
        }
    }
}

/////////////////////////////////////////////////////////////////
bool View_Graph::IsConsistent(void){
    for(int i=0;i < nodes; ++i) colorTab[i]=0;
    MaxConsistent(0);
    int max = colorTab[0];
    for(int i=0; i < nodes ; ++i) {
        if(max < colorTab[i]) max = colorTab[i];
    }
    if (max == nodes) return true;
    else return false;
}
