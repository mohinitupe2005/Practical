#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAXSIZE 100

int main() {
    int shmid;
    key_t key = 5679;
    char *shm;

    shmid = shmget(key, MAXSIZE, 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }

    shm = (char *)shmat(shmid, NULL, 0);
    if (shm == (char *)-1) {
        perror("shmat failed");
        exit(1);
    }

    printf("Client: Reading message from shared memory:\n");
    printf("%s\n", shm);
    shmdt(shm);

    printf("Client: Client exiting.\n");
    return 0;
}



gcc server.c -o server
gcc client.c -o client

./server

./client
























































#include <stdio.h>      // Standard input/output library for printf, etc.
#include <stdlib.h>     // Standard library for exit(), perror(), etc.
#include <sys/ipc.h>    // For IPC (Inter-Process Communication) functions
#include <sys/shm.h>    // For shared memory functions: shmget, shmat, shmdt

#define MAXSIZE 100     // Define the maximum size of shared memory

int main() {
    int shmid;         // Variable to store shared memory ID
    key_t key = 5679;  // Unique key to identify the shared memory segment
    char *shm;         // Pointer to access the shared memory

    // Access the shared memory segment created by the server
    shmid = shmget(key, MAXSIZE, 0666);  
    // shmget() arguments:
    // key: unique identifier for the shared memory
    // size: MAXSIZE (number of bytes)
    // flag: 0666 → read/write permissions
    // Returns: shared memory ID (shmid), -1 on failure
    if (shmid < 0) {
        perror("shmget failed");  // Print error message if shmget fails
        exit(1);                  // Exit program with error code 1
    }

    // Attach the shared memory segment to the client's address space
    shm = (char *)shmat(shmid, NULL, 0);
    // shmat() arguments:
    // shmid: shared memory ID obtained from shmget
    // NULL: let the system choose the address to attach
    // 0: default read/write mode
    // Returns: pointer to shared memory, (char*)-1 on failure
    if (shm == (char *)-1) {
        perror("shmat failed");  // Print error message if shmat fails
        exit(1);                 // Exit program with error code 1
    }

    // Print the message read from shared memory
    printf("Client: Reading message from shared memory:\n");
    printf("%s\n", shm);         // The message stored by server is in shm

    // Detach the shared memory from the client process
    shmdt(shm);
    // shmdt() arguments:
    // shm: pointer to the shared memory to detach
    // After this, the client no longer has access to shared memory

    printf("Client: Client exiting.\n");  // Indicate that client finished
    return 0;                              // Return 0 → normal program termination
}





shmget(key, size, flags)

Creates or accesses a shared memory segment.

Returns a shared memory ID (shmid) used by other functions.

shmat(shmid, NULL, 0)

Attaches the shared memory segment identified by shmid to the process’s address space.

Returns a pointer to the shared memory.

shmdt(shm)

Detaches the shared memory segment from the process.

After detaching, the process can no longer access the memory.

perror("message")

Prints the last error encountered during system calls along with a custom message.

exit(1)

Terminates the program immediately with status code 1 (indicating failure).

IPC means exchanging data between processes.
It allows processes to talk to each other using mechanisms like pipes, message queues, shared memory, semaphores, or sockets.

| Method                  | Description                                                           | Example                            |
| ----------------------- | --------------------------------------------------------------------- | ---------------------------------- |
| **Pipes**               | Used for communication between related processes (like parent–child). | `pipe()`, `mkfifo()`               |
| **FIFOs (Named Pipes)** | Used for communication between independent processes.                 | `mkfifo("fifo1", 0666)`            |
| **Message Queues**      | Send/receive messages between processes in queue form.                | `msgget()`, `msgsnd()`, `msgrcv()` |
| **Shared Memory**       | Multiple processes access the same memory area.                       | `shmget()`, `shmat()`              |
| **Semaphores**          | Used for process synchronization (avoid data conflicts).              | `semget()`, `semop()`              |
| **Sockets**             | Communication over a network or between computers.                    | `socket()`, `send()`, `recv()`     |
