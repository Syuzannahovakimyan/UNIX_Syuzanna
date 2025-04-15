#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

struct Process {
    int id;
    int burstTime;
};


class RoundRobin {
public:
    RoundRobin(size_t timeQuantum) : _time(timeQuantum), _stop(false) {}

    void add_process(Process proc) {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _processQueue.push(proc);
        }
        _cv.notify_one(); 
    }

    void run() {
        std::unique_lock<std::mutex> lock(_mutex);
        while (!_stop) {
            _cv.wait(lock, [this] { return !_processQueue.empty() || _stop; });

            while (!_processQueue.empty()) {
                Process current = _processQueue.front();
                _processQueue.pop();
                lock.unlock();

                std::cout << "Process " << current.id << " is running...\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(_time * 100));

                lock.lock();
                if (current.burstTime > _time) {
                    std::cout << "Process " << current.id << " executed for " 
                              << _time << " units, remaining: " 
                              << current.burstTime - _time << " units\n";
                    current.burstTime -= _time;
                    _processQueue.push(current);
                } else {
                    std::cout << "Process " << current.id << " finished execution\n";
                }
            }
        }
    }

    void start() {
        _schedulerThread = std::thread(&RoundRobin::run, this);
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _stop = true;
        }
        _cv.notify_all();
        _schedulerThread.join();
    }

private:
    std::queue<Process>     _processQueue;
    size_t                  _time;
    std::mutex              _mutex;
    std::condition_variable _cv;
    std::thread             _schedulerThread;
    bool                    _stop;
};

int main() {
    RoundRobin scheduler(3); 
    scheduler.start(); 

    scheduler.add_process({1, 5});
    scheduler.add_process({2, 8});
    scheduler.add_process({3, 2});

    std::this_thread::sleep_for(std::chrono::seconds(2));
    scheduler.add_process({4, 5}); 

    std::this_thread::sleep_for(std::chrono::seconds(5));
    scheduler.stop(); 

    return 0;
}
