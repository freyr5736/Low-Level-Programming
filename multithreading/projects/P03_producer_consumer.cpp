#include "thread_pool.hpp"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <random>
#include <syncstream>
#include <thread>

std::deque<int> task_queue;
std::atomic<bool> done{false};
std::mutex m;
std::condition_variable cv;

void producer(int task) {
    while (task) {
        std::unique_lock<std::mutex> lock(m);
        task_queue.push_back(task);
        // std::osyncstream out(std::cout);
        // out << "\r" << task << " in queue"<<std::flush;
        task--;
        lock.unlock();
        cv.notify_all();
    }
    done = true;
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, []() { return !task_queue.empty(); });
        std::osyncstream out(std::cout);
        out << "\r" << task_queue.front() << " completed" << std::flush;
        task_queue.pop_front();
        lock.unlock();
        cv.notify_all();
        if (done) {
            break;
        }
    }
}

int main() {
    std::vector<std::thread> all;
    all.reserve(std::thread::hardware_concurrency());
    // Level #1 Basic Call
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 1; i <= 4; ++i) {
        if (i % 2 == 0) {
            all.emplace_back(producer, 10000000);
        } else {
            all.emplace_back(consumer);
        }
    }

    for (auto &t : all) {
        t.join();
    }
    auto stop_time = std::chrono::high_resolution_clock::now();
    auto duration =
    std::chrono::duration_cast<std::chrono::microseconds>(stop_time -start_time); 
    std::cout<< "\nTime Elapsed "<<duration.count()<<" μs\n";

    // Level #2 Thread pool
    // auto start_time = std::chrono::high_resolution_clock::now();
    // thread_pool producer_pool(2);
    // auto a = producer_pool.enqueue([&]() { producer(10000000); });
    // thread_pool consumer_pool(2);
    // auto b = consumer_pool.enqueue([&]() { consumer(); });

    // a.get();
    // b.get();

    // auto stop_time = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    // std::cout << "\n Time Elapsed " << duration.count() << " μs\n";
    return 0;
}