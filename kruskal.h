#pragma once

#include <iostream>
#include <vector>

//class representing graph edge in the kruskal algorithm
class KEdge{
public:
    KEdge(int node1 = 0, int node2 = 0, int tree_no = 0, int weight = 0): m_node1(node1), m_node2(node2), m_tree_no(tree_no), m_weight(weight) {}
    int m_node1;
    int m_node2;
    int m_tree_no;
    int m_weight;
};

//class representing queue in the kruskal algorithm
class KQueue{
public:
    //queue initialization
    KQueue(int** neighTab, int size);

    //pop method
    KEdge pop(void);

    //returning true if the queue is emty 
    bool empty(void){
        return queue.empty();
    }
private:
    std::vector<KEdge> queue;
};

//checking if the edge is part of existing tree 
//0 if the edge is destroying tree structure
//-1 if the edge is brand new tree
//other if the edge is a part of existing tree
int find_set(KEdge edge, const std::vector<KEdge> &v);

//union of the edge and spinning tree
void unite(KEdge &edge, std::vector<KEdge> &v);