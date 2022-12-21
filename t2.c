#include "t2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE 100
int vertix_count=0;
int edge_count=0;

vertix* root[4806];

//Function to find the vertix from their stop id
vertix* find_vertix_by_stopId(int id){
    for (int i = 0; i < 4806; i++)
    {
        if (root[i]->stopId==id)
        {
            return root[i];
        }
        
    }
    return NULL;
    
}

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

int load_edges ( char *fname ){
    FILE* f=fopen("edges.csv","r");
    char buf[MAX_STRING_SIZE];
    int flag;
    for (int i = 0; i < 3; i++)
    {
        flag=next_field(buf,f);
    }
    while (flag!=2)
    {
        flag=next_field(buf,f);
        vertix* from=find_vertix_by_stopId(atoi(buf));
        flag=next_field(buf,f);
        int to=atoi(buf);
        // vertix* to=find_vertix_by_stopId(atoi(buf));
        flag=next_field(buf,f);
        int weight=atoi(buf);

        edge* new_edge=malloc(sizeof(edge));
        new_edge->destination=to;
        new_edge->weight=weight;

        if (from->adjacent==NULL)
        {
            new_edge->next=NULL;
            from->adjacent=new_edge;
        }else{
            edge* tmp=from->adjacent;
            from->adjacent=new_edge;
            new_edge->next=tmp;
        }

       

        edge_count++;    
    }
    
    printf("Loaded %d edges\n",edge_count);

}

int load_vertices ( char *fname ){

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
        val->adjacent=NULL;
        root[vertix_count++]=val;
    }
    printf("Loaded %d vertices\n",vertix_count);
}


// //returns min_distance vertix node
// int djikstra(int currentNode,int min_distance_vertix){
//     vertix* start=find_vertix_by_stopId(currentNode);
//     vertix* min_dist_vertix;
//     int vertix_min_distance;
//     if (min_distance_vertix!=-1)
//     {
//         min_dist_vertix=find_vertix_by_stopId(min_distance_vertix);
//         vertix_min_distance=min_distance_vertix; //returning the stop id of min distance node
//     }else{
//         min_dist_vertix=NULL;
//         vertix_min_distance=-1;
//     }
    

//     edge* ptr=start->adjacent;
//     int distance_till_now=start->distance;
//     while (ptr!=NULL)
//     {
//         vertix* node=find_vertix_by_stopId(ptr->destination);
//         if (node->last==-1)
//         {
//             node->last=currentNode;
//             node->distance=distance_till_now+ptr->weight;
//         }
//         else if (node->distance>distance_till_now+start->distance)
//         {
//             node->last=currentNode;
//             node->distance=distance_till_now+ptr->weight;
//         }

//         //Code to find the minimum distance vertix
//         if (vertix_min_distance==-1)
//         {
//             vertix_min_distance=node->stopId;
//             min_dist_vertix=find_vertix_by_stopId(vertix_min_distance);
//         }
//         else if (node->distance<min_dist_vertix->distance)
//         {
//             vertix_min_distance=node->stopId;
//             min_dist_vertix=find_vertix_by_stopId(vertix_min_distance);
//         }
        
        
//         ptr=ptr->next;
        
//     }
//     return vertix_min_distance;
    
// }

// void printPath(int startNode,int endNode){
//     if (startNode==endNode)
//     {
//         printf(" %d->",endNode);
//         return;
//     }
    
//     vertix* end=find_vertix_by_stopId(endNode);
//     printf(" %d->",endNode);
//     printPath(startNode,end->last);
    
// }

void shortest_path(int startNode, int endNode){
//     vertix* start=find_vertix_by_stopId(startNode);
//     start->permanent=1;
//     vertix* end=find_vertix_by_stopId(endNode);
//     int min_vertix_stopId=-1;
//     while (end->permanent==0)
//     {
//         min_vertix_stopId=djikstra(startNode,min_vertix_stopId);
//         if (min_vertix_stopId==-1)
//         {
//             printf("\nNO PATH FOUND!");
//             return;
//         }
        
//         vertix* min_vertix=find_vertix_by_stopId(min_vertix_stopId);
//         min_vertix->permanent=1;
//     }
    
    
//     printPath(startNode,endNode);

}


void free_memory ( void ){
    for (int i = 0; i < 4806; i++)
    {

        
        edge* ptr=root[i]->adjacent;
        if (ptr==NULL)
        {
            free(ptr);
            continue;
        }
        
        edge* ptr_next=ptr->next;
        while (ptr_next!=NULL)
        {
            free(ptr);
            ptr=ptr_next;
            ptr_next=ptr_next->next;
        }
        free(ptr);
        free(root[i]);
        
    }
    
}

//TO BE DELETED
/*
void print_graph(){
    for (int i = 0; i < 10; i++)
    {
        printf("\n%d %s %s %s",root[i]->stopId,root[i]->name,root[i]->lat,root[i]->lon);
        edge* ptr=root[i]->adjacent;
        while (ptr!=NULL)
        {
            printf(" %d",ptr->destination);
            ptr=ptr->next;
        }
        
    }
    
}

int main(){
    load_vertices("vertices.csv");
    load_edges("edges.csv");
    // print();

    // get the start and end point
    printf("\nPlease enter stating bus stop >\t\t");
    int startingNode;
    scanf("%d", &startingNode);
    printf("Please enter destination bus stop >\t");
    int endingNode;
    scanf("%d", &endingNode);

	shortest_path(startingNode, endingNode);
    

    


	free_memory();
}
*/