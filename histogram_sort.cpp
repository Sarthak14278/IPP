#include <iostream>
#include <stdlib.h>
#include <omp.h>

#define N 8
#define MAX_VAL 10

#include <iostream>
#include <omp.h>

int main() {
    int input[N] = {3, 6, 2, 7, 3, 8, 2, 4};
    int histogram[MAX_VAL + 1] = {0};
    int output[N];

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        #pragma omp atomic
        histogram[input[i]]++;
    }

    int index = 0;
    for (int i = 0; i <= MAX_VAL; i++) {
        for (int j = 0; j < histogram[i]; j++) {
            output[index++] = i;
        }
    }

    std::cout << "Sorted Output:\n";
    for (int i = 0; i < N; i++)
        std::cout << output[i] << " ";
    std::cout << std::endl;
    return 0;
}