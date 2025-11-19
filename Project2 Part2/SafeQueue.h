// Thread-safe queue for producers and consumers
template <typename T>
class SafeQueue {
private:
    std::queue<T> queue;
    std::mutex queueMutex;
    std::condition_variable cv_empty; // consumers waiting for items
    std::condition_variable cv_full;  // producers waiting for space
    size_t capacity;

public:
    SafeQueue(size_t cap) : capacity(cap) {}

    void push(const T& item) {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv_full.wait(lock, [this] { return queue.size() < capacity; }); // wait if full
        queue.push(item);
        cv_empty.notify_one(); // notify a waiting consumer
    }

    T pop() {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv_empty.wait(lock, [this] { return !queue.empty(); }); // wait if empty
        T item = queue.front();
        queue.pop();
        cv_full.notify_one(); // notify a waiting producer
        return item;
    }

    size_t size() {
        return queue.size();
    }
};