#include <iostream>
#include <chrono>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

void seq_dfs(vector<vector<int>> &adjList, vector<bool> &visited, int src) {
    // code ... 
    cout<<src<<" ";
    for(int i=0; i < adjList[src].size(); i++) 
    {
        int adj = adjList[src][i];
        if(visited[adj] == true)
            continue;
        visited[adj] = true;
        seq_dfs(adjList, visited, adj);
    }
}

void parallel_dfs(vector<vector<int>> &adjList, vector<bool> &visited, int src) {
    // code ... 
    cout<<src<<" ";
    for(int i=0; i < adjList[src].size(); i++) 
    {
        int adj = adjList[src][i];
        if(visited[adj] == true)
            continue;
        
        visited[adj] = true;
        seq_dfs(adjList, visited, adj);
        
    }

}

void seq_bfs(vector<vector<int>> adjList, vector<bool> &visited, int src) {
    // code ... 
    queue<int> que;
    
    que.push(src);
    visited[src] = true;

    while(!que.empty())
    {
        int curr = que.front();
        que.pop();
        cout<<curr<<" ";

        for(int i=0; i<adjList[curr].size(); i++) 
        {
            int adj = adjList[curr][i];
            if(visited[adj] == true)
                continue;
            que.push(adj);
            visited[adj] = true;
        }
    }

}

void parallel_bfs(vector<vector<int>> adjList, vector<bool> &visited, int src) {
    // code ... 
    queue<int> que;
    
    que.push(src);
    visited[src] = true;

    while(!que.empty())
    {
        int curr = que.front();
        #pragma omp critical
            que.pop();
        cout<<curr<<" ";

        #pragma omp parallel for
        for(int i=0; i<adjList[curr].size(); i++) 
        {
            int adj = adjList[curr][i];
            
            #pragma omp critical
            {
                if(visited[adj] == true)
                    continue;
                que.push(adj);
                visited[adj] = true;
            }
        }
    }
}

void clear(vector<bool> &visited) { 
    for(int i=0; i<visited.size(); i++)
        visited[i] = false;
}

int main(int argc, char **argv) 
{
    // Graph create 
    vector<vector<int>> adjList = {
        {2, 3, 4, 1}, 
        {0, 10, 9}, 
        {0}, 
        {0, 4, 8}, 
        {0, 3}, 
        {6}, 
        {5, 7}, 
        {6, 8}, 
        {3, 7}, 
        {1}, 
        {1}
    };

    vector<bool> visited(adjList.size(), false);
    int src = 0;


    // sequential dfs and display time  and output
    visited[src] = true;
    auto start = chrono::high_resolution_clock::now();
    seq_dfs(adjList, visited, src);
    auto stop = chrono::high_resolution_clock::now();
    cout<<"Time taken for sequential dfs = "<<chrono::duration_cast<chrono::microseconds>(stop-start).count()<<" microseconds."<<endl;
    clear(visited);

    // // parallel dfs and display time and output 
    // start = // ... ;
    // parallel_dfs(adjList);
    // stop = // ... ;
    // cout<<"Time taken for parallel dfs = "<<stop-start<<" micro seconds."<<en

    // // sequential bfs and display time  and output
    visited[src] = true;
    start = chrono::high_resolution_clock::now();
    seq_bfs(adjList, visited, src);
    stop = chrono::high_resolution_clock::now();
    cout<<"Time taken for sequential bfs = "<<chrono::duration_cast<chrono::microseconds>(stop-start).count()<<" microseconds."<<endl;
    clear(visited);

    // // parallel bfs and display time and output 
    visited[src] = true;
    start = chrono::high_resolution_clock::now();
    parallel_bfs(adjList, visited, src);
    stop = chrono::high_resolution_clock::now();
    cout<<"Time taken for parallel bfs = "<<chrono::duration_cast<chrono::microseconds>(stop-start).count()<<" microseconds."<<endl;
    clear(visited);

    return 0;
}