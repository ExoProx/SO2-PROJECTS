#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <random>
#include <queue>

//Initializing mutexes for FIFO Queue usage
std::mutex queue_mutex;
std::queue<int> request_queue;

//Function for simulating a philosopher
void philosopher(int id, std::vector<std::mutex>& forks) {
    int left = id;
    int right = (id + 1) % forks.size();

    while (true) {
        int time = rand() % 4000 + 1000;
        //initialising the sleeping philosopher
        std::cout << "Philosopher " << id << " is thinking.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
        
        //Locking the mutex responsible for our FIFO queue, then adding the philosopher to a waiting list.
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            request_queue.push(id);
        }

        //Chcecking, whether the philosopher is on top of the vector, protecting a critical section of accessing the vector
        while (true) {
            std::lock_guard<std::mutex> lock(queue_mutex);
            if (request_queue.front() == id) {
                request_queue.pop();
                break;
            }
            std::this_thread::yield();
        }
        //Locking forks for a philosohper, until he finishes eating
            std::lock(forks[left], forks[right]);
            std::lock_guard<std::mutex> left_fork(forks[left], std::adopt_lock);
            std::lock_guard<std::mutex> right_fork(forks[right], std::adopt_lock);
        

            std::cout << "Philosopher " << id << " is eating.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(time));
            std::cout << "Philosopher " << id << " finished eating.\n";
    }
}

int main() {
    std::vector<std::thread> philosophers;
    std::cout << "Input the number of philosophers";
    int N;
    std::cin >> N;
    std::vector<std::mutex> forks(N);

    for (int i = 0; i < N; i++) {
        //Function creating an object directly in a vector, and executing a function
        philosophers.emplace_back(philosopher, i, std::ref(forks));
    }

    for (auto& t : philosophers) {
        t.join();
    }

    return 0;
}
