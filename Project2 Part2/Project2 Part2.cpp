#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "SafeQueue.h"
#include <atomic>
#include <mutex>

using namespace std;

const int num_producers = 10;
const int num_consumers = 10;
const size_t queue_capacity = 10;
const int items_per_producer = 100;
const int randomInterval = 250;

SafeQueue<int> shared_queue(queue_capacity);

mutex printMutex;
vector<std::thread> producer_threads;

void PrintOutInfo() {
    lock_guard<mutex> lock(printMutex);

    system("cls");
    cout << "Queue Info Realtime:\n"
        "Producer Count: " << num_producers << 
        endl <<
        "Consumer Count: " << num_consumers << 
        endl << 
        "Random Interval for producing and consuming (ms): " << randomInterval << 
        endl;

    for (int i = 0; i < queue_capacity; i++) {
        if (i < shared_queue.size()) {
            cout << "[X] ";
        }
        else cout << "[ ] ";
    }

    cout << endl;
}

// Producer function
void Producer(int id, SafeQueue<int>& q, int num_items) {
    for (int i = 0; i < num_items; ++i) {
        int item = id * 100 + i;
        q.push(item);
        PrintOutInfo();
        std::cout << "Producer " << id << " produced: " << item << std::endl;
        int random = (int)(rand() % randomInterval);
        std::this_thread::sleep_for(std::chrono::milliseconds(random)); // Simulate work
    }
}

// Consumer function
void Consumer(int id, SafeQueue<int>& q) {
    while (true) {
        int item = q.pop();
        PrintOutInfo();
        std::cout << "Consumer " << id << " consumed: " << item << std::endl;
        int random = (int)(rand() % randomInterval);
        std::this_thread::sleep_for(std::chrono::milliseconds(random)); // Simulate work
    }
}

int main() {
    for (int i = 0; i < num_producers; ++i) {
        producer_threads.emplace_back(Producer, i + 1, std::ref(shared_queue), items_per_producer);
    }

    std::vector<std::thread> consumer_threads;
    for (int i = 0; i < num_consumers; ++i) {
        consumer_threads.emplace_back(Consumer, i + 1, std::ref(shared_queue));
    }

    for (auto& t : producer_threads) {
        t.join();
    }

    for (auto& t : consumer_threads) {
        t.detach();
    }

    std::cout << "All producers finished." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Allow some time for consumers

    return 0;
}