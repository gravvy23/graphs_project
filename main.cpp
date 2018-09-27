#include <iostream>
#include "graph.h"
#include "zestaw2.h"
int main(){
///////////part 1 ///////////
    //losowanie grafu spojnego i zapis do pliku w formie macierzy sasiedztwa
    //odczyt z pliku
    int node_number, edge_number;
    std::cout<<"Podaj liczbe wierzcholkow"<<std::endl;
    do {
        std::cin>>node_number;
    }   while (node_number < 0 || node_number >= 50);
    std::cout<<"Podaj liczbe krawedzi"<<std::endl;
    std::cin>>edge_number;
    View_Graph graph = View_Graph(node_number,edge_number);
    Graph my_graph("tab.tmp",node_number);
    my_graph.show_tab();
    //losowanie krawedzi
    my_graph.set_edge_values();
    std::cout<<"Macierz wag po wyolosowaniu wag krawedzi"<<std::endl;
    my_graph.show_tab();

///////////part 2 /////////// algorytm Djikstry
    int x;
    std::cout<<"*****Algotytm dijkstry******"<<std::endl;
    std::cout<<"Wybierz wierzcholek startowy: ";
    std::cin>>x;
    my_graph.run_djikstra_algorithm(x);

///////////part 3 /////////// macierz odleglosci
    std::cout<<"*****Wygenerowana macierz odleglosci*****"<<std::endl;
    my_graph.run_distance_matrix();

///////////part 4 /////////// cetrum i centrum minmax
    std::cout<<std::endl<<"centrum grafu: "<<my_graph.find_center()<<std::endl;
    std::cout<<"centrum minmax grafu: "<<my_graph.find_minmax_center()<<std::endl;

///////////part 5 /////////// minimalne drzewo rozpinajace
    std::cout<<"*****Minimalne drzewo rozpinajace*****"<<std::endl;
    my_graph.run_kruskal_algorithm();
    return 0;
}