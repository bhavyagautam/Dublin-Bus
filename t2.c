#include "t2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_VERTIX 7668
#define MAX_STRING_SIZE 100
int vertix_count=0;
int edge_count=0;

vertix* root[TOTAL_VERTIX];
int edges[TOTAL_VERTIX][TOTAL_VERTIX];

//CODE RELATED TO CSV FILES
//Provides next field of the csv file
int next_field( char *buffer, FILE *f ) {
    int cnt=0;
    int outsideQuote=1; //1 when outside of quotes, 0 when inside of quotes
    
    while (1)
    {
    if(cnt==MAX_STRING_SIZE){
        printf("Buffer size maximised! Exiting!"); 
        return -1;
    }

    char c=getc(f);

    if(c=='\"'){
    if (outsideQuote==1){outsideQuote=0;continue;}
    if(outsideQuote==0){outsideQuote=1;continue;}
    }

    if(c==',' && outsideQuote){buffer[cnt++]='\0';return 0;}
    if(c==EOF){buffer[cnt++]='\0';return 2;} //Different return value to determine if the file has ended
    if(c=='\n'){buffer[cnt++]='\0';return 1;}
    
    buffer[cnt++]=c;
    }
	
}

int load_vertices (){

    FILE* f=fopen("vertices.csv","r");
    char buf[MAX_STRING_SIZE];
    int flag;
    for (int i = 0; i < 4; i++)
    {
        flag=next_field(buf,f);
    }
    while (flag!=2)
    {   
        vertix* val=malloc(sizeof(vertix));


        val->distance=-1;
        val->last=-1;
        val->permanent=0;

        flag=next_field(buf,f);
        val->stopId=atoi(buf);
        flag=next_field(buf,f);
        // val->name=buf;
        strcpy(val->name,buf);
        flag=next_field(buf,f);
        // val->lat=buf;
        strcpy(val->lat,buf);
        flag=next_field(buf,f);
        // val->lon=buf;
        strcpy(val->lon,buf);
        root[val->stopId]=val;
        vertix_count++;

    }
    printf("Loaded %d vertices\n",vertix_count);
}


int load_edges(){
    FILE* f=fopen("edges.csv","r");
    char buf[MAX_STRING_SIZE];
    int flag;

    //Making all edges -1
    for (int i = 0; i < TOTAL_VERTIX; i++)
    {
        for (int j = 0; j < TOTAL_VERTIX; j++)
        {
            edges[i][j]=-1;
        }
        
    }
    

    //skipping titles
    for (int i = 0; i < 3; i++)
    {
        flag=next_field(buf,f);
    }
    while (flag!=2)
    {
        flag=next_field(buf,f);
        int from=atoi(buf);
        flag=next_field(buf,f);
        int to=atoi(buf);
        flag=next_field(buf,f);
        int weight=atoi(buf);
        edges[from][to]=weight;
        edges[to][from]=weight;
        edge_count++;    
    }
    
    printf("Loaded %d edges\n",edge_count);

}

//FINDS SHORTEST DISTANCE FROM NODES WHICH ARE NOT PERMANENT
int shortest_distance_vertix(){
    int ans=-1;
    for (int i = 0; i < TOTAL_VERTIX; i++)
    {   
        if (root[i]==NULL)
        {
            continue;
        }
        
        if(root[i]->permanent==0 && root[i]->distance>=0){
            ans=i;
            break;
        }
    }
    for (int i = ans+1; i < TOTAL_VERTIX; i++)
    {   
        if (root[i]==NULL)
        {
            continue;
        }
        
        if(root[i]->permanent==0 && root[i]->distance>=0){
            
            if (root[i]->distance<root[ans]->distance)
            {
                ans=i;
            }
        }
    }
    return ans;
}



//PRINT THE SHORTEST PATH
void shortest_path(int startNode, int endNode){
    if (startNode==endNode)
    {
        return;
    }
    
    int current_node=startNode;
    root[current_node]->permanent=1;

    //updating distances
    for (int i = 0; i < TOTAL_VERTIX; i++)
    {
        if (edges[current_node][i]>=0)
        {   
            //updating the distance if shorter distance found
            if (root[i]->distance > root[current_node]->distance+edges[current_node][i] || root[i]->distance==-1)
            {
                root[i]->distance=root[current_node]->distance+edges[current_node][i];
                root[i]->last=current_node;
            }
            
            
        }
        
    }

    int shortest_distance=shortest_distance_vertix();
    if(shortest_distance==-1){
        printf("SOME ERROR WITH SHORTEST DISTANCE!");
        return;
    }
    // root[shortest_distance]->last=current_node;
    shortest_path(shortest_distance,endNode);
    

}
int sol[TOTAL_VERTIX];
int cnt=0;
//PRINT PATH
void print_path(int startNode, int endNode){

    if (startNode==endNode)
    {   
        // printf(" %d->",endNode);
        sol[cnt++]=endNode;
        return;
    }

    // printf(" %d->",endNode);
    sol[cnt++]=endNode;
    print_path(startNode,root[endNode]->last);

}

void print_answer(){
    printf("StopID\t Name\n");
    for (int i = cnt-1; i >= 0; i--)
    {
        printf("%3d\t %s\n",root[sol[i]]->stopId,root[sol[i]]->name);
    }
    
}



//HELPER FUNCTIONS
int print_edges(){
    for (int i = 0; i < TOTAL_VERTIX; i++)
    {
        for(int j=0;j<TOTAL_VERTIX;j++){
            printf("%d ",edges[i][j]);
        }
        printf("\n");
    }
    
}


int main(){

    load_vertices();
    load_edges();

    // for (int i = 0; i < TOTAL_VERTIX; i++)
    // {
    //     if (root[i])
    //     printf("%d\n",root[i]->stopId);
    // }
    

    
     // get the start and end point
    printf("\nPlease enter stating bus stop >\t\t");
    int startingNode;
    scanf("%d", &startingNode);
    printf("Please enter destination bus stop >\t");
    int endingNode;
    scanf("%d", &endingNode);

	shortest_path(startingNode, endingNode);
    print_path(startingNode,endingNode);
    print_answer();

    return 1;
}