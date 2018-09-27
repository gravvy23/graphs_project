#pragma once

class View_Graph {
public:
    //generating random consistent Graph
    View_Graph(int n, int edges);

    //dealloc
    ~View_Graph();

    //save to file
    void Save();

    //maximal consistent
    void MaxConsistent(int i);

    //checking if the graph is consistent
    bool IsConsistent(void);
private:
    int ** neighTab;
    int nodes;
    int edges;
    int colorTab[50];
};