#include <cstddef>
#include <ctime>
#include <iostream>
#include <queue>

struct Process {
    int id;
    int burstTime;
};


class RoundRobin{
public:
    RoundRobin(size_t time = 3):_time(time) {}

    void add_process(const Process& proc){
        _processes.push(proc);
    }

    void set_time(size_t time){
        _time = time;
    }

    void run(){
        while (!_processes.empty()) {
            Process current = _processes.front();
            _processes.pop();

            if (current.burstTime > _time) {
                std::cout << "Process " << current.id << " executed for " << _time << " units\n";
                current.burstTime -= _time;
                _processes.push(current); 

            } else {
                std::cout << "Process " << current.id << " finished execution\n";
            }
        }
    }


private:
    std::queue<Process> _processes;
    size_t _time;

};



int main() {
    RoundRobin sch(3);

    sch.add_process({0, 8});
    sch.add_process({1, 18});
    sch.add_process({2, 9});

    sch.run();
}
