#include <iostream>
#include <climits>
#include <omp.h>

#define V 6  // Number of vertices

void dijkstra(int graph[V][V], int src) {
    int dist[V];
    bool visited[V];

    // Initialize distances
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
    }
    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = -1;
        int minDist = INT_MAX;

        // Find the vertex with the minimum distance (can be parallelized with reduction)
        #pragma omp parallel for
        for (int v = 0; v < V; v++) {
            if (!visited[v]) {
                #pragma omp critical
                {
                    if (dist[v] < minDist) {
                        minDist = dist[v];
                        u = v;
                    }
                }
            }
        }

        visited[u] = true;

        // Update distances of adjacent vertices (parallelizable)
        #pragma omp parallel for
        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                #pragma omp critical
                {
                    if (dist[u] + graph[u][v] < dist[v]) {
                        dist[v] = dist[u] + graph[u][v];
                    }
                }
            }
        }
    }

    // Print result
    std::cout << "Vertex \t Distance from Source\n";
    for (int i = 0; i < V; i++)
        std::cout << i << " \t " << dist[i] << "\n";
}

int main() {
    int graph[V][V] = {
        {0, 4, 0, 0, 0, 0},
        {4, 0, 8, 0, 0, 0},
        {0, 8, 0, 7, 0, 4},
        {0, 0, 7, 0, 9, 14},
        {0, 0, 0, 9, 0, 10},
        {0, 0, 4, 14, 10, 0}
    };

    dijkstra(graph, 0);
    return 0;
}

