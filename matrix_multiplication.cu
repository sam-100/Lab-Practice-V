#include <iostream>
#include <cstdlib>
#include <ctime>

// CUDA Kernel to perform matrix multiplication
__global__ void matrixMultiplication(int *a, int *b, int *c, int m, int n, int p) {
    // Calculate the row and column index of the element
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < m && col < p) {
        int sum = 0;
        for (int i = 0; i < n; ++i) {
            sum += a[row * n + i] * b[i * p + col];
        }
        c[row * p + col] = sum;
    }
}

int main() {
    int m, n, p;

    // Input matrix dimensions
    std::cout << "Enter number of rows (m) for matrix A: ";
    std::cin >> m;
    std::cout << "Enter number of columns (n) for matrix A and rows for matrix B: ";
    std::cin >> n;
    std::cout << "Enter number of columns (p) for matrix B: ";
    std::cin >> p;

    // Allocate memory for matrices A, B, and C on host
    int *h_A = new int[m * n];
    int *h_B = new int[n * p];
    int *h_C = new int[m * p];

    // Initialize matrices A and B with random values
    srand(time(NULL));
    for (int i = 0; i < m * n; ++i) {
        h_A[i] = rand() % 10;  // Random numbers between 0 and 9
    }
    for (int i = 0; i < n * p; ++i) {
        h_B[i] = rand() % 10;  // Random numbers between 0 and 9
    }

    // Allocate memory for matrices A, B, and C on device
    int *d_A, *d_B, *d_C;
    cudaMalloc((void**)&d_A, m * n * sizeof(int));
    cudaMalloc((void**)&d_B, n * p * sizeof(int));
    cudaMalloc((void**)&d_C, m * p * sizeof(int));

    // Copy matrices A and B from host to device
    cudaMemcpy(d_A, h_A, m * n * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, n * p * sizeof(int), cudaMemcpyHostToDevice);

    // Define grid and block dimensions
    dim3 threadsPerBlock(16, 16);
    dim3 numBlocks((p + threadsPerBlock.x - 1) / threadsPerBlock.x, (m + threadsPerBlock.y - 1) / threadsPerBlock.y);

    // Launch kernel to perform matrix multiplication
    matrixMultiplication<<<numBlocks, threadsPerBlock>>>(d_A, d_B, d_C, m, n, p);

    // Copy matrix C from device to host
    cudaMemcpy(h_C, d_C, m * p * sizeof(int), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    // Output the resulting matrix C
    std::cout << "Resultant matrix C (" << m << "x" << p << "):" << std::endl;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            std::cout << h_C[i * p + j] << " ";
        }
        std::cout << std::endl;
    }

    // Free host memory
    delete[] h_A;
    delete[] h_B;
    delete[] h_C;

    return 0;
}
