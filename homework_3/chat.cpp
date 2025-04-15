#include <iostream>
#include <cstring>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>

const char* SHM_NAME = "/chat_shm";
const char* SEM_PARENT = "/sem_parent";
const char* SEM_CHILD = "/sem_child";
const int SHM_SIZE = 1024;

int main() {
    // Create shared memory
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SHM_SIZE);
    void* shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Create semaphores
    sem_t* sem_parent = sem_open(SEM_PARENT, O_CREAT, 0666, 0);
    sem_t* sem_child = sem_open(SEM_CHILD, O_CREAT, 0666, 0);

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed!\n";
        return 1;
    }

    if (pid == 0) {  
        execl("./child.out", NULL);

    } else {
        std::string message;
        while (message != "exit") {
            message.clear();
            std::cout << "Parent: ";
            std::getline(std::cin, message);
            std::strcpy(static_cast<char*>(shm_ptr), message.c_str());

            sem_post(sem_parent);
            sem_wait(sem_child);

            std::cout << "Parent received: " << static_cast<char*>(shm_ptr) << std::endl;
        }

        wait(NULL); 
        shm_unlink(SHM_NAME);
        sem_close(sem_parent);
        sem_close(sem_child);
        sem_unlink(SEM_PARENT);
        sem_unlink(SEM_CHILD);
    }
    return 0;
}

