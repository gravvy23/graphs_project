#include "graph.h"
#include "kruskal.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <vector>
#include <iomanip>
#include <cmath>
#include <assert.h>
/////////////////////////////////////////////////////////////////
Graph::Graph(std::string name, int size){
    std::ifstream my_stream(name.c_str());
    if (!my_stream){
        std::cout<<"blad otwarcia pliku"<<std::endl;
        exit(EXIT_FAILURE);
    }
    m_size = size;
    //tab initialization
    m_neighTab = new int*[m_size];
    for (int i=0;i<m_size;i++){
        m_neighTab[i] = new int[m_size];
    }
    std::string tmp;
    int i = 0;
    //extracting data from file to neighTab
    while(!my_stream.eof()){
        std::getline(my_stream,tmp);
        std::istringstream chest(tmp);
        for (int j=0;j<m_size;j++)
            chest >> m_neighTab[i][j];
        i++;
    }
    my_stream.close();

    positionTab = new float*[m_size];
    for (int i=0; i<m_size; i++){
        positionTab[i] = new float[2];
        float partition = 2*M_PI/m_size;
        float t=(i+1)*partition;
        float r=2*m_size/M_PI;
        positionTab[i][0]=r*sin(t);
        positionTab[i][1]=r*cos(t);
    }
}
/////////////////////////////////////////////////////////////////
void Graph::show_tab() const{
    srand(3242);

    for (int i=0;i<m_size;i++){
        for (int j=0;j<m_size;j++)
            std::cout<<m_neighTab[i][j]<<" ";
        std::cout<<std::endl;
    }

    //////rysowanie z zestawu 2///////
    std::ofstream my_stream("data.dat");
    if (!my_stream){
        std::cout<<"blad otwarcia pliku data.dat"<<std::endl;
        exit(EXIT_FAILURE);
    }
    for(int i=0; i < m_size; ++i)
        my_stream<<positionTab[i][0]<<" "<<positionTab[i][1]<<" "<<(char)(i+'1')<<std::endl<<std::endl;
    for(int i=0;i < m_size; ++i)
            for(int j=0;j< m_size ; ++j){
                if(m_neighTab[i][j]){
                    my_stream<<positionTab[i][0]<<" "<<positionTab[i][1]<<" "<<(char)(i+'1')<<std::endl;
                    my_stream<<positionTab[j][0]<<" "<<positionTab[j][1]<<" "<<(char)(j+'1')<<std::endl<<std::endl;
                }
            }
    my_stream.close();

    std::ofstream my_stream2("edges.dat");
    if (!my_stream2){
        std::cout<<"blad otwarcia pliku edges.dat"<<std::endl;
        exit(EXIT_FAILURE);
    }
    for(int i=0;i < m_size; ++i)
            for(int j=i;j< m_size ; ++j){
                if(m_neighTab[i][j]){
                    float x_position = (positionTab[i][0] + positionTab[j][0])/2 + ((std::rand()%10-5)/100)*fabs(positionTab[i][0] - positionTab[j][0]);
                    float a = (positionTab[i][1] - positionTab[j][1])/(positionTab[i][0] - positionTab[j][0]);
                    float b = positionTab[i][1] - a*positionTab[i][0];
                    float y_position = a*(x_position)+b;
                    my_stream2<<x_position<<" "<<y_position<<" "<<(char)(m_neighTab[i][j]+'0')<<std::endl<<std::endl;
                }
            }
    my_stream2.close();

    system("gnuplot \"./skrypt.gplt\"");
}

/////////////////////////////////////////////////////////////////
void Graph::set_edge_values(){
    srand( time( NULL ) );
    for (int i=0; i<m_size;i++){
        for (int j=i;j<m_size;j++){
            if (m_neighTab[i][j]){
                m_neighTab[i][j] = std::rand() % 9 + 1;
                m_neighTab[j][i] = m_neighTab[i][j];
            }
        }
    }
}
/////////////////////////////////////////////////////////////////
int* Graph::djikstra_algorithm(int x, bool flag){
    int val = x - 1, min_index, index;
    int d[m_size];
    int p[m_size];
    //initializing arrays
    for (int i=0; i < m_size ; i++){
        d[i]=INT_MAX;
        p[i] = -1;
    }
    d[val] = 0;
    std::vector<int> queue;
    for (int i = 0;i < m_size; i++)
        queue.push_back(i);
    while (!queue.empty()){
        min_index = queue[0];
        index = 0;
        //searching for min value in the d array
        for (unsigned int i = 1; i<queue.size();i++){
            if (d[queue[i]] < d[min_index]){
                min_index = queue[i];
                index = i;
            }
        }
        //deleting min index from vector
        queue.erase(queue.begin() + index);
        //looking for neighbours
        for (unsigned int i = 0; i<queue.size();i++){
            if (m_neighTab[min_index][queue[i]]){
                if (d[queue[i]] > d[min_index] + m_neighTab[min_index][queue[i]]){
                    //d array actualization
                    d[queue[i]] = d[min_index] + m_neighTab[min_index][queue[i]];
                    //p array actualization
                    p[queue[i]] = min_index;
                }
            }
        }
    }
    ///extracting results///
    //if flag is true - version for output
    if (flag){
        std::cout<<"najkrotrze sciezki z wierzcholka: "<<x<<std::endl<<std::endl;
        for (int i = 0; i<m_size;i++){
            int length = 0;
            if (i != val){
                std::cout<<i+1;
                int prev = p[i];
                length+=m_neighTab[i][prev];
                while (prev != -1){
                    std::cout<<"<-"<<prev+1;
                    if (p[prev] != -1)
                        length+=m_neighTab[prev][p[prev]];
                    prev = p[prev];
                }
                std::cout<<std::endl<<"dlugosc sciezki: "<<length<<std::endl<<std::endl;
            }
        }
        return NULL;
    }
    //if flag is false - version for other member functions
    else {
        int* dist_tab = new int[m_size];
        for (int i = 0; i<m_size;i++){
            int length = 0;
            if (i != val){
                int prev = p[i];
                length+=m_neighTab[i][prev];
                while (prev != -1){
                    if (p[prev] != -1)
                        length+=m_neighTab[prev][p[prev]];
                    prev = p[prev];
                }
                dist_tab[i] = length;
            }
            else dist_tab[i] = 0;
        }
        return dist_tab;
    }
}

/////////////////////////////////////////////////////////////////
int** Graph::distance_matrix(void){
    int** dist_matrix = new int*[m_size];
    for (int i=0;i<m_size;i++){
        dist_matrix[i] = djikstra_algorithm(i+1,0);
    }
    return dist_matrix;
}

/////////////////////////////////////////////////////////////////
void Graph::run_distance_matrix(void){
    int** dist_matrix = distance_matrix();
    for (int i=0;i<m_size;i++){
        for (int j=0;j<m_size;j++)
            std::cout<<std::setw(3)<<std::right<<dist_matrix[i][j];
        std::cout<<std::endl;
    }

    //freeing allocated memory
    for (int i=0;i<m_size;i++)
        delete[] dist_matrix[i];
    delete[] dist_matrix;
}

/////////////////////////////////////////////////////////////////
int Graph::find_center(void){
    int** dist_matrix = distance_matrix();
    int sum = INT_MAX;
    int index;
    //finding minimal sum per node in distance matrix
    for (int i=0;i<m_size;i++){
        int tmp = 0;
        for (int j=0;j<m_size;j++)
            tmp+=dist_matrix[i][j];
        if (tmp < sum){
            sum = tmp;
            index = i+1;
        }
    }

    //freeing allocated memory
    for (int i=0;i<m_size;i++)
        delete[] dist_matrix[i];
    delete[] dist_matrix;

    return index;
}

/////////////////////////////////////////////////////////////////
int Graph::find_minmax_center(void){
    int max_distance_matrix[m_size];
    int** dist_matrix = distance_matrix();
    int max_dist, min, index;

    //finding max distances per node
    for (int i=0;i<m_size;i++){
        max_dist = 0;
        for (int j=0;j<m_size;j++){
            if (dist_matrix[i][j] > max_dist)
                max_dist = dist_matrix[i][j];
        }
        max_distance_matrix[i] = max_dist;
    }

    //finding minimum of max distances
    min = max_distance_matrix[0];
    index = 1;
    for (int i=1;i<m_size;i++){
        if (max_distance_matrix[i] < min){
            min = max_distance_matrix[i];
            index = i+1;
        }
    }

    //freeing allocated memory
    for (int i=0;i<m_size;i++)
        delete[] dist_matrix[i];
    delete[] dist_matrix;

    return index;
}

/////////////////////////////////////////////////////////////////
int** Graph::kruskal_algorithm(void){
    std::vector<KEdge> v;
    KEdge edge;
    //KRUSKAL ALGORITHM
    KQueue my_queue(m_neighTab,m_size);
    while (!my_queue.empty()){
        edge = my_queue.pop();
        //if edge is a brand new tree
        if (find_set(edge, v) == -1)
            v.push_back(edge);
        //if edge is a part of existing tree (and is not destroying the structure of exisitng tree)
        else if (find_set(edge, v)){
            unite(edge,v);
            v.push_back(edge);
        }
    }

    //creating neighbourhood matrix for minimal spanning tree
    int** tree_matrix = new int*[m_size];
    for (int i=0;i<m_size;i++){
        tree_matrix[i] = new int[m_size];
        for (int j=0;j<m_size;j++)
            tree_matrix[i][j] = 0;
    }

    //filling matrix
    for (unsigned i=0; i<v.size(); i++){
        tree_matrix[v[i].m_node1][v[i].m_node2] = v[i].m_weight;
        tree_matrix[v[i].m_node2][v[i].m_node1] = v[i].m_weight;
    }

    return tree_matrix;
}

/////////////////////////////////////////////////////////////////
void Graph::run_kruskal_algorithm(void){
    int **tree_matrix = kruskal_algorithm();
    for (int i=0; i<m_size;i++){
        for (int j=0; j<m_size; j++)
            std::cout<<std::setw(3)<<std::right<<tree_matrix[i][j];
        std::cout<<std::endl;
    }
    //drawing
    std::ofstream my_stream("sp_tree.dat");
    if (!my_stream){
        std::cout<<"blad otwarcia pliku sp_tree.dat"<<std::endl;
        exit(EXIT_FAILURE);
    }
    for(int i=0; i < m_size; ++i)
        my_stream<<positionTab[i][0]<<" "<<positionTab[i][1]<<" "<<(char)(i+'1')<<std::endl<<std::endl;
    for(int i=0;i < m_size; ++i)
            for(int j=0;j< m_size ; ++j){
                if(tree_matrix[i][j]){
                    my_stream<<positionTab[i][0]<<" "<<positionTab[i][1]<<" "<<(char)(i+'1')<<std::endl;
                    my_stream<<positionTab[j][0]<<" "<<positionTab[j][1]<<" "<<(char)(j+'1')<<std::endl<<std::endl;
                }
            }
    my_stream.close();
    std::ofstream my_stream2("edges_tree.dat");
    if (!my_stream2){
        std::cout<<"blad otwarcia pliku edges.dat"<<std::endl;
        exit(EXIT_FAILURE);
    }
    for(int i=0;i < m_size; ++i)
            for(int j=i;j< m_size ; ++j){
                if(tree_matrix[i][j]){
                    float x_position = (positionTab[i][0] + positionTab[j][0])/2 + ((std::rand()%10-5)/100)*fabs(positionTab[i][0] - positionTab[j][0]);
                    float a = (positionTab[i][1] - positionTab[j][1])/(positionTab[i][0] - positionTab[j][0]);
                    float b = positionTab[i][1] - a*positionTab[i][0];
                    float y_position = a*(x_position)+b;
                    my_stream2<<x_position<<" "<<y_position<<" "<<(char)(tree_matrix[i][j]+'0')<<std::endl<<std::endl;
                }
            }
    my_stream2.close();

    //freeing allocated memory
    for (int i=0;i<m_size;i++)
        delete[] tree_matrix[i];
    delete[] tree_matrix;

    system("gnuplot \"./skrypt_tree.gplt\"");
}


/////////////////////////////////////////////////////////////////
Graph::~Graph(){
    for (int i=0;i<m_size;i++){
        delete[] m_neighTab[i];
        delete[] positionTab[i];
    }
    delete[] m_neighTab;
    delete[] positionTab;
}
