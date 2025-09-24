#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class thread_pool {
  public:
    thread_pool(size_t total_threads) {
        for (size_t i = 0; i < total_threads; ++i) {
            all_threads.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(q_mutex);
                        cv_task.wait(lock, [this]() { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) {
                            return; // exit thread
                        }
                        task = std::move(tasks.front()); // extract the task
                        tasks.pop(); // remove task from the queue
                    }
                    // if i donot use scopes, then it will run task with lock which can create deadlock
                    // alternative is to unlock before running the task and lock after that but it creates overhead
                    task(); // execute the task
                }
            });
        }
    }

    template <class T> void enqueue(T &&task) {
        std::unique_lock<std::mutex> lock(q_mutex);
        tasks.emplace(std::forward<T>(task)); // forward<T> pushes value exactly as it is
        lock.unlock();
        cv_task.notify_one(); // signalling that the task is available
    }

    ~thread_pool() {
        std::unique_lock<std::mutex> lock(q_mutex);
        stop = true;
        lock.unlock();
        cv_task
            .notify_all(); // wake up all threads to finish their tasks and exit
        for (auto &thread : all_threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

  private:
    std::vector<std::thread> all_threads;
    std::queue<std::function<void()>> tasks; // can send anything inside the queue that is what function<void()> is
    std::mutex q_mutex;
    std::condition_variable cv_task;
    bool stop{false};
};