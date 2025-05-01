#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>

#define MAX_NODES 100
#define INF 1000000

int graph[MAX_NODES][MAX_NODES];
int dist[MAX_NODES];
int visited[MAX_NODES];

void dijkstra_parallel(int num_nodes, int source) {
    for (int i = 0; i < num_nodes; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[source] = 0;

    for (int count = 0; count < num_nodes - 1; count++) {
        int min_dist = INF, u = -1;

        #pragma omp parallel
        {
            int local_min = INF, local_u = -1;
            #pragma omp for nowait
            for (int v = 0; v < num_nodes; v++) {
                if (!visited[v] && dist[v] < local_min) {
                    local_min = dist[v];
                    local_u = v;
                }
            }
            #pragma omp critical
            {
                if (local_min < min_dist) {
                    min_dist = local_min;
                    u = local_u;
                }
            }
        }

        if (u == -1) break;

        visited[u] = 1;

        #pragma omp parallel for
        for (int v = 0; v < num_nodes; v++) {
            if (!visited[v] && graph[u][v] && dist[u] + graph[u][v] < dist[v]) {
                #pragma omp critical
                {
                    if (dist[u] + graph[u][v] < dist[v]) {
                        dist[v] = dist[u] + graph[u][v];
                    }
                }
            }
        }
    }
}

#include <iostream>
#include <omp.h>

int main() {
    int num_nodes = 6;

    for (int i = 0; i < num_nodes; i++)
        for (int j = 0; j < num_nodes; j++)
            graph[i][j] = 0;

    graph[0][1] = 2; graph[0][2] = 4;
    graph[1][2] = 1; graph[1][3] = 7;
    graph[2][4] = 3;
    graph[4][3] = 2; graph[4][5] = 5;
    graph[3][5] = 1;

    double start = omp_get_wtime();
    dijkstra_parallel(num_nodes, 0);
    double end = omp_get_wtime();

    std::cout << "Shortest distances from source node 0:\n";
    for (int i = 0; i < num_nodes; i++) {
        if (dist[i] == INF)
            std::cout << "Node " << i << " is unreachable" << std::endl;
        else
            std::cout << "Node " << i << " : " << dist[i] << std::endl;
    }

    std::cout << "Parallel Dijkstra completed in " << (end - start) << " seconds." << std::endl;
    return 0;
}