#include <iostream>
#include <stdlib.h>
#include <omp.h>

#define MAX 100

int adj[MAX][MAX];
int visited[MAX];
int queue[MAX], front = 0, rear = 0;

void bfs(int n, int start) {
    visited[start] = 1;
    queue[rear++] = start;

    while (front < rear) {
        int u = queue[front++];
        std::cout << u << " ";

        #pragma omp parallel for
        for (int v = 0; v < n; v++) {
            if (adj[u][v] && !visited[v]) {
                #pragma omp critical
                {
                    if (!visited[v]) {
                        visited[v] = 1;
                        queue[rear++] = v;
                    }
                }
            }
        }
    }
}

int main() {
    int n = 6;

    int temp[6][6] = {
        {0,1,1,0,0,0},
        {1,0,1,1,0,0},
        {1,1,0,0,1,0},
        {0,1,0,0,1,1},
        {0,0,1,1,0,1},
        {0,0,0,1,1,0}
    };

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            adj[i][j] = temp[i][j];

    for (int i = 0; i < n; i++)
        visited[i] = 0;

    std::cout << "BFS starting from node 0:\n";
    bfs(n, 0);
    std::cout << std::endl;

    return 0;
}
