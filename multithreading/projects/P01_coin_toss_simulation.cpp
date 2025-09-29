#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <syncstream>
#include <thread>
#include <vector>

// for level #1 and #2
// std::mutex m;
// long long heads = 0;
// long long tails = 0;
// long long total_time = 0;
// std::atomic<long long> flips = 0;

// Level #1
// using simple lock mechanism
// void simulate_toss() {
//     auto start_time = std::chrono::high_resolution_clock::now();
//     for (long long i = 1; i <= 100000; ++i) {
//         flips++;
//         std::osyncstream out (std::cout); // this is a thread safe output stream
//         out<<"\r"<<"flip "<<flips<<std::flush;
//         m.lock();
//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::bernoulli_distribution toss (0.5);
//         if (toss(gen)) {
//             heads++;
//         } else {
//             tails++;
//         }
//         m.unlock();
//     }
//     std::cout<<"\n";
//     auto stop_time = std::chrono::high_resolution_clock::now();
//     auto duration =
//     std::chrono::duration_cast<std::chrono::microseconds>(stop_time -start_time); 
//     total_time +=  duration.count();
// }

// Level #2
// using local variables and scoped_lock to decrease overhead of locking
// void simulate_toss() {
//     auto start_time = std::chrono::high_resolution_clock::now();
//     long long heads_local = 0, tails_local = 0;
//     std::mt19937 gen(std::random_device{}());
//     std::bernoulli_distribution toss(0.5);

//     for (long long i = 0; i < 100000; ++i) {
//         flips.fetch_add(1, std::memory_order_relaxed);
//         std::osyncstream out (std::cout);
//         out<<"\r"<<"flip "<<flips<<std::flush;
//         if (toss(gen)) {
//             ++heads_local;
//         } else {
//             ++tails_local;
//         }
//     }

//     std::scoped_lock lock(m);
//     heads += heads_local;
//     tails += tails_local;
//     auto stop_time = std::chrono::high_resolution_clock::now();
//     auto duration =
//     std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time); 
//     total_time += duration.count();
// }

// Level #3
// using atomic variables to avoid locking altogether
std::atomic<long long> heads{0};
std::atomic<long long> tails{0};
std::atomic<long long> flips{0};
std::atomic<long long> total_time{0};

void simulate_toss() {
    auto start_time = std::chrono::high_resolution_clock::now();
    long long heads_local = 0, tails_local = 0;
    std::mt19937 gen(std::random_device{}());
    std::bernoulli_distribution toss(0.5);

    for (long long i = 0; i < 100000; ++i) {
        flips.fetch_add(1, std::memory_order_relaxed);
        std::osyncstream out(std::cout);
        out << "\r" << "flip " << flips << std::flush;
        if (toss(gen)) {
            ++heads_local;
        } else {
            ++tails_local;
        }
    }

    heads.fetch_add(heads_local, std::memory_order_relaxed);
    tails.fetch_add(tails_local, std::memory_order_relaxed);
    auto stop_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    total_time += duration.count();
}

int main() {
    std::vector<std::thread> all;
    all.reserve(10);
    for (long long i = 1; i <= 3; ++i) {
        all.emplace_back(simulate_toss);
    }
    for (auto &t : all) {
        t.join();
    }
    std::cout <<"\nTotal heads = " << heads << "\nTotal tails = " << tails
              << "\nTotal time = " << total_time << "μs\n";

    return 0;
}