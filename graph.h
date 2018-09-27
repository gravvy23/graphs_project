#pragma once
#include <iostream>

class Graph {
public:

    //initialization form generated text file 
    Graph(std::string name, int size);

    ///////////////////////////////////////////////////////
    void show_tab() const;

    //seting random values of edges from range 1-10
    void set_edge_values();

    //running Djikstra algorithm
    void run_djikstra_algorithm(int val){
        djikstra_algorithm(val);
    }

    //generating distance matrix 
    void run_distance_matrix(void);

    //finiding center of the graph
    int find_center(void);

    //finding minmax center of the graph
    int find_minmax_center(void);

    ///////////////////////////////////////////////////////
    ~Graph();

    //running kruskal algorithm
    void run_kruskal_algorithm(void);
private:
    int m_size;
    int **m_neighTab;
    float **positionTab;
    int* djikstra_algorithm(int val, bool flag = 1);
    int** distance_matrix(void);
    int** kruskal_algorithm(void);
};