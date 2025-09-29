#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include "thread_pool.hpp"

void create_matrix(std::vector<std::vector<int>> &matrix, int rows, int cols) {
    std::mt19937 rd(std::random_device{}());
    std::uniform_int_distribution<int> gen(0, 10);

    matrix.resize(rows, std::vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = gen(rd);
        }
    }
}

void multiply_matrices(const std::vector<std::vector<int>> &A,const std::vector<std::vector<int>> &B,std::vector<std::vector<int>> &C, int start_row,int end_row) {
    int n = A.size();
    int m = A[0].size();
    int p = B[0].size();

    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < p; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < m; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void tiled_multiply(const std::vector<std::vector<int>> &A,const std::vector<std::vector<int>> &B,std::vector<std::vector<int>> &C, int tile_size, int n, int m, int p, int start_row, int end_row) {
    for (int i = start_row; i < end_row; i += tile_size) {
        for (int j = 0; j < p; j += tile_size) {
            for (int k = 0; k < m; k += tile_size) {
                for (int ii = i; ii < std::min(i + tile_size, end_row); ++ii) {
                    for (int jj = j; jj < std::min(j + tile_size, p); ++jj) {
                        for (int kk = k; kk < std::min(k + tile_size, m); ++kk) {
                            C[ii][jj] += A[ii][kk] * B[kk][jj];
                        }
                    }
                }
            }
        }
    }
}

int main(){
    std::vector<std::vector<int>> A, B, C;
    int n = 1000, m = 1000, p = 1000;
    create_matrix(A, n, m);
    create_matrix(B, m, p);
    C.resize(n, std::vector<int>(p, 0));
    
    // Level #1
    // simple matrix multiplication with distributing works between multiple threads
    // int total_threads = 10;
    // std::vector<std::thread> threads;
    // int rows_per_thread = n / total_threads;
    // auto start_time = std::chrono::high_resolution_clock::now();
    // for (int i = 0; i < total_threads; ++i) {
    //     int start_row = i * rows_per_thread;
    //     int end_row = (i == total_threads - 1) ? n : start_row + rows_per_thread;
    //     threads.emplace_back(multiply_matrices, std::cref(A), std::cref(B), std::ref(C), start_row, end_row);
    // }

    // for (auto &t : threads) {
    //     t.join();
    // }

    // Level #2
    // using thread pool to manage threads efficiently
    // auto start_time = std::chrono::high_resolution_clock::now();
    // thread_pool pool(10);
    // int rows_per_task = n / 10;
    // for (int i = 0; i < 10; ++i) {
    //     int start_row = i * rows_per_task;
    //     int end_row = (i == 10 - 1) ? n : start_row + rows_per_task;
    //     pool.enqueue([&, start_row, end_row]() {
    //         multiply_matrices(A, B, C, start_row, end_row);
    //     });
    // }

    // Level #3
    // tiled matrix multiplication with thread pool
    auto start_time = std::chrono::high_resolution_clock::now();
    thread_pool pool(10);
    int rows_per_task = n / 10;
    int tile_size = 10;
    for(int i=0;i<10;++i){
        int start_row = i * rows_per_task;
        int end_row = (i == 10 - 1) ? n : start_row + rows_per_task;
        pool.enqueue([&, start_row, end_row]() {
            tiled_multiply(A, B, C, tile_size, n, m, p, start_row, end_row);
        });
    }
    auto stop_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    std::cout << "Matrix multiplication completed in " << duration.count()<<"μs\n";

    return 0;
}