
#ifndef T2_H_
#define T2_H_
typedef struct edge edge;
typedef struct vertix vertix;

struct vertix
{
    int stopId;
    char name[100];
    char lat[15];
    char lon[15];
    edge* adjacent;

    int last; //where we came from to that node
    int distance; //lowest distance to that node
    int permanent; //is it the permanent node

};

struct edge
{
    int destination;
    int weight;
    edge* next;
};



int load_edges ( char *fname ); //loads the edges from the CSV file of name fname
int load_vertices ( char *fname );  //loads the vertices from the CSV file of name fname
void shortest_path(int startNode, int endNode); // prints the shortest path between startNode and endNode, if there is any
void free_memory ( void ) ; // frees any memory that was used

#endif