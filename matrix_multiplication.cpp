#include <iostream>
#include <stdlib.h>
#include <omp.h>

#define N 3

#include <iostream>
#include <omp.h>

int main() {
    int A[N][N] = {{1,2,3}, {4,5,6}, {7,8,9}};
    int B[N][N] = {{9,8,7}, {6,5,4}, {3,2,1}};
    int C[N][N] = {0};

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];

    std::cout << "Resultant Matrix C:\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            std::cout << C[i][j] << " ";
        std::cout << std::endl;
    }
    return 0;
}