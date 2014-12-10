#include "game.h"
#define INF 123456789
using namespace std;

void freepairs(int*** arr, int n) {
    int** A = *arr;
    for(int i=0;i<n;i++) {
        delete [] A[i];
        A[i]=NULL;
    }
    delete [] A;
    *arr=NULL;
}
//printing for debugging function
void printpairs(int** arr, int n) {
    for(int i=0;i<n;i++)
        cout<<arr[i][0]<<","<<arr[i][1]<<" ";
    cout<<"\n";
}
//reads n ordered pairs in nx2 array
int** readpairs(int n) {
    int** arr= new int*[n];
    for(int i=0;i<n;i++)
         arr[i]=new int[2];
    
    char comma;
    for(int i=0;i<n;i++)
        cin>>arr[i][0]>>comma>>arr[i][1];
    return arr;
}

void relax(int u, int v, int w, int** dist_pointer, int** pred_pointer) {
    int* dist = *dist_pointer; 
    int* pred= *pred_pointer; 
    if(dist[u] + w < dist[v]) {
        dist[v] = dist[u] + w;
        pred[v] = u;
    }
}

int bellmanFord(int*** matrix_pointer, int s) {
    //init distances
    int* dist = new int[101];
    dist[s]=0;
    for(int i=2;i<101;i++)
        dist[i]=INF;
    //init predecessor
    int* pred = new int[101];

    int** M = *matrix_pointer;
    for(int it=0;it<101;it++)  //V-1 passes
        for(int i=1;i<101;i++) {
            for(int j=1;j<101;j++)
                relax(i,j,M[i][j],&dist, &pred);
        }

    //check for negative cycles
    for(int i=1;i<101;i++) {
        for(int j=1;j<101;j++)
            if(dist[i] + M[i][j] < dist[j])
                printf("negative cycle\n");
    }
    int shortestPath = dist[100];
    delete [] pred;
    delete [] dist;
    return shortestPath;
}

board::~board() {
    freepairs(&ladders, num_ladders);
    freepairs(&snakes, num_snakes);
    for(int i=1;i<101;i++)
        delete [] AM[i];
    delete [] AM;
}

board::board(int s, int l) {
    num_snakes=s;
    num_ladders=l;
    ladders= readpairs(l);
    snakes = readpairs(s);

    //allocate matrix, index at 1 to make board game sensible
    AM = new int*[101];
    for(int i=1;i<101;i++) {
        AM[i] = new int[101];
        for(int j=1;j<101;j++)
            AM[i][j] = INF; //all weights initially inf
    }

    //self loop has weight 0, aka diagonal of AM is all 0's
    for(int i=1;i<101;i++)
        AM[i][i]=0;

    //initialize matrix with dice rolls
    for(int i=1;i<101;i++)
        for(int j=1;j<7;j++) {
            if((i+j)<101)
                AM[i][i+j]=1; //all dice rolls are weight 1, count as 1 'move'
        }
    //initialize matrix with ladders 
    for(int i=0;i<l;i++) {
        int bottom = ladders[i][0];
        int top = ladders[i][1];
        for(int j=1;j<101;j++)
            if(bottom!=j) //self loop remains 0
                AM[bottom][j]=INF; //ladder is forced, you have no choice but to take it if you land on it
        AM[bottom][top]=0; //ladder moves are weight 0, discounted 'moves'
    }
    //initialize matrix with snakes 
    for(int i=0;i<s;i++) {
        int head = snakes[i][0];
        int tail = snakes[i][1];
        for(int j=1;j<101;j++)
            if(head!=j) //self loop remains 0
                AM[head][j]=INF; //snake is forced, you have no choice but to take it if you land on it
        AM[head][tail]=0; //snake moves are weight 0, discounted 'moves'
    }
}

//returns min moves to go from 1 to 100
int snakeLadder(board* B) {
    return bellmanFord(&(B->AM), 1);
}

int main(){

    //alloc graph

    int numtests;
    cin>>numtests;
    for(int i=0;i<numtests;i++) {
        int snakes, ladders;
        char comma;
        cin>>ladders>>comma>>snakes;
        board B(snakes,ladders);
        printf("%d\n", snakeLadder(&B));
    }
    return 0;
}
