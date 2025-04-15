#include <iostream>
#include <cstring>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

const char* SHM_NAME = "/chat_shm";
const char* SEM_PARENT = "/sem_parent";
const char* SEM_CHILD = "/sem_child";
const int SHM_SIZE = 1024;


int main(int argc, char* argv[]){
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SHM_SIZE);
    void* shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    sem_t* sem_parent = sem_open(SEM_PARENT, O_CREAT, 0666, 0);
    sem_t* sem_child = sem_open(SEM_CHILD, O_CREAT, 0666, 0);

    std::string message;

    while (message != "exit") {
        sem_wait(sem_parent);        

        message.clear();
        std::cout << "Child received: " << static_cast<char*>(shm_ptr) << std::endl;

        std::cout << "Child: ";
        std::getline(std::cin, message);
        std::strcpy(static_cast<char*>(shm_ptr), message.c_str());

        sem_post(sem_child);  
    }
}
