#include "kruskal.h"
#include <climits>

/////////////////////////////////////////////////////////////////////////////
KQueue::KQueue(int** neighTab, int size){
    int no = 1;
    for (int i = 0; i<size; i++){
        for (int j=i;j<size; j++){
            if (neighTab[i][j]){
                //adding all edges from neighborhood matrix
                queue.push_back(KEdge(i,j,no,neighTab[i][j]));
                no++;
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
KEdge KQueue::pop(void){
    int min_weight = INT_MAX;
    int index;
    //looking for minimum weight edge
    for (unsigned int i=0; i < queue.size(); i++){
        if (queue[i].m_weight < min_weight){
            min_weight = queue[i].m_weight;
            index = i;
        }
    }
    //erasing and returning minimum weight edge from queue
    KEdge tmp = queue[index];
    queue.erase(queue.begin() + index);
    return tmp;
}

/////////////////////////////////////////////////////////////////////////////
int find_set(KEdge edge, const std::vector<KEdge> &v){
    int first_node_no = 0;
    int second_node_no = 0;
    for (unsigned int i=0; i< v.size();i++){
        //checking if node 1 in tree
        if (v[i].m_node1 == edge.m_node1 || v[i].m_node2 == edge.m_node1)
            first_node_no = v[i].m_tree_no;
        //checking if node 2 in tree
        if (v[i].m_node1 == edge.m_node2 || v[i].m_node2 == edge.m_node2)
            second_node_no = v[i].m_tree_no;
    }
    //if tree1 is not tree2
    if (first_node_no != second_node_no){
        if (first_node_no) return first_node_no;
        else return second_node_no;
    }
    //if edge is a brand new tree
    if ((first_node_no == second_node_no) && (first_node_no == 0))
        return -1;
    //if tree1 is same as tree2
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
void unite(KEdge &edge, std::vector<KEdge> &v){
    for (unsigned int i=0; i< v.size();i++){
        //extracting tree_no from tree connecting with node1 in the edge
        if (v[i].m_node1 == edge.m_node1 || v[i].m_node2 == edge.m_node1) {
            edge.m_tree_no = v[i].m_tree_no;
            break;
        }
    }
    for (unsigned int i=0; i< v.size();i++){
        //checking if node2 is connetced to existing tree
        if (v[i].m_node1 == edge.m_node2 || v[i].m_node2 == edge.m_node2){
            if (edge.m_tree_no != v[i].m_tree_no){
                int tree_no = v[i].m_tree_no;
                //if it is
                //changing tree_no of edges of tree connected to node2
                for (unsigned int j=0; j< v.size();j++){
                    if (v[j].m_tree_no == tree_no)
                        v[j].m_tree_no = edge.m_tree_no;
                }
            }
        }
    }
}