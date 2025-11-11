#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define MAXSIZE 100

int main() {
    int shmid;
    key_t key = 5679; 
    char *shm;

    shmid = shmget(key, MAXSIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }

    shm = (char *)shmat(shmid, NULL, 0);
    if (shm == (char *)-1) {
        perror("shmat failed");
        exit(1);
    }

    printf("Server: Enter a message: ");
    fgets(shm, MAXSIZE, stdin);   

    printf("Server: Message written to shared memory.\n");

    sleep(5);

    shmdt(shm);

    shmctl(shmid, IPC_RMID, NULL);

    printf("Server: Shared memory removed. Server exiting.\n");
    return 0;
}





















































#include <stdio.h>      // Standard I/O library for printf(), fgets(), etc.
#include <stdlib.h>     // Standard library for exit(), perror(), etc.
#include <sys/ipc.h>    // IPC (Inter-Process Communication) library for key_t
#include <sys/shm.h>    // Shared memory functions: shmget, shmat, shmdt, shmctl
#include <string.h>     // String handling functions (optional here for fgets)
#include <unistd.h>     // For sleep() function

#define MAXSIZE 100     // Maximum size of the shared memory segment

int main() {
    int shmid;         // Variable to store shared memory ID
    key_t key = 5679;  // Unique key to identify the shared memory segment
    char *shm;         // Pointer to access the shared memory

    // Create a shared memory segment or access it if already exists
    shmid = shmget(key, MAXSIZE, IPC_CREAT | 0666);
    // Arguments:
    // key: unique identifier (must match in client)
    // size: MAXSIZE bytes
    // flags: IPC_CREAT → create if it doesn't exist
    //        0666 → read/write permission for all users
    // Returns: shared memory ID (shmid), -1 on failure
    if (shmid < 0) {
        perror("shmget failed"); // Prints the error message if shmget fails
        exit(1);                 // Exit the program with error
    }

    // Attach the shared memory segment to the server process's address space
    shm = (char *)shmat(shmid, NULL, 0);
    // Arguments:
    // shmid: shared memory ID returned by shmget
    // NULL: system chooses the attachment address
    // 0: default mode (read/write)
    // Returns: pointer to shared memory, (char *)-1 on failure
    if (shm == (char *)-1) {
        perror("shmat failed"); // Print error if attachment fails
        exit(1);                // Exit the program with error
    }

    // Prompt the user (server) to enter a message to write into shared memory
    printf("Server: Enter a message: ");
    fgets(shm, MAXSIZE, stdin);   
    // fgets() reads a line from standard input (stdin) and stores it in shm
    // MAXSIZE ensures it doesn't read more than the allocated memory

    printf("Server: Message written to shared memory.\n");

    sleep(5);  // Wait for 5 seconds to let the client read the message

    // Detach the shared memory segment from server process
    shmdt(shm);
    // Arguments:
    // shm: pointer to shared memory to detach
    // After this, the server process no longer has access to shared memory

    // Remove the shared memory segment permanently
    shmctl(shmid, IPC_RMID, NULL);
    // Arguments:
    // shmid: ID of the shared memory to remove
    // IPC_RMID: flag to mark segment for deletion
    // NULL: no additional options required
    // After this, the shared memory is destroyed once all processes detach

    printf("Server: Shared memory removed. Server exiting.\n");
    return 0;  // Normal termination of the program
}



shmget(key, size, flags)

Creates or gets a shared memory segment.

Returns a shared memory ID (shmid).

IPC_CREAT → create if it doesn’t exist.

0666 → read/write permission for all users.

shmat(shmid, NULL, 0)

Attaches the shared memory segment to the processs address space.

Returns a pointer to the shared memory, or (char *)-1 on error.

fgets(buffer, size, stdin)

Reads a line from standard input into buffer.

size prevents buffer overflow.

sleep(seconds)

Pauses execution for a given number of seconds.

Here, it allows the client time to read the shared memory.

shmdt(shm)

Detaches the shared memory from the process.

shmctl(shmid, IPC_RMID, NULL)

Marks the shared memory segment for removal.

It will be removed once all processes have detached.

perror("message")

Prints the last error encountered during system calls with a custom message.

exit(1)

Terminates the program with an error code 1.