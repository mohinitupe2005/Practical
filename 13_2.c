#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>

int main() {
    char sentence[200], result[200];
    int fd1, fd2;
    int chars = 0, words = 0, lines = 0;

    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);

    fd1 = open("fifo1", O_RDONLY);
    read(fd1, sentence, sizeof(sentence));
    close(fd1);

    for (int i = 0; sentence[i] != '\0'; i++) {
        if (sentence[i] != ' ' && sentence[i] != '\n')
            chars++;
        if ((sentence[i] == ' ' && sentence[i+1] != ' ' && sentence[i+1] != '\n') || sentence[i+1] == '\0')
            words++;
        if (sentence[i] == '\n')
            lines++;
    }
    if (lines == 0) lines = 1; 

    FILE *fp = fopen("output.txt", "w");
    fprintf(fp, "Characters: %d\nWords: %d\nLines: %d\n", chars, words, lines);
    fclose(fp);

    fp = fopen("output.txt", "r");
    fread(result, sizeof(char), sizeof(result), fp);
    fclose(fp);

    fd2 = open("fifo2", O_WRONLY);
    write(fd2, result, strlen(result) + 1);
    close(fd2);

    return 0;
}




gcc process1.c -o p1
gcc process2.c -o p2

t1
./p2

t2
./p1
















































#include <stdio.h>      // For I/O functions
#include <stdlib.h>     // For exit()
#include <unistd.h>     // For read(), write(), and close()
#include <fcntl.h>      // For open() and flags
#include <sys/stat.h>   // For mkfifo()

int main() {
    int fd1, fd2;             // File descriptors for FIFOs
    char sentence[200];       // To store input sentence from process1
    char result[200];         // To store final output
    FILE *fp;                 // File pointer for writing to a text file
    int characters = 0, words = 0, lines = 0; // Counters

    // Create two named pipes (same as in process1)
    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);

    // Open first FIFO in read-only mode (to receive data from process1)
    fd1 = open("fifo1", O_RDONLY);
    read(fd1, sentence, sizeof(sentence));  // Read the sentence
    close(fd1);

    // Count characters, words, and lines
    for (int i = 0; sentence[i] != '\0'; i++) {
        characters++; // Count every character

        // Count words when space or newline or null terminator is found
        if (sentence[i] == ' ' || sentence[i] == '\n' || sentence[i+1] == '\0')
            words++;

        // Count lines when newline character found
        if (sentence[i] == '\n')
            lines++;
    }

    // Write the counts into a text file
    fp = fopen("output.txt", "w");
    fprintf(fp, "Characters: %d\nWords: %d\nLines: %d\n", characters, words, lines);
    fclose(fp);

    // Reopen file to read its contents
    fp = fopen("output.txt", "r");
    fread(result, sizeof(char), sizeof(result), fp);
    fclose(fp);

    // Send the result back to process1 through second FIFO
    fd2 = open("fifo2", O_WRONLY);
    write(fd2, result, sizeof(result));
    close(fd2);

    return 0;
}


| Function                                                                           | Header File    | Description                                                                                                           |
| ---------------------------------------------------------------------------------- | -------------- | --------------------------------------------------------------------------------------------------------------------- |
| `mkfifo("fifo1", 0666);`                                                           | `<sys/stat.h>` | Creates the named pipe `fifo1` with read/write permissions. If it already exists, it just ensures it's accessible.    |
| `open("fifo1", O_RDONLY);`                                                         | `<fcntl.h>`    | Opens the FIFO `fifo1` for **reading only**, so it can receive data from `process1`.                                  |
| `read(fd1, sentence, sizeof(sentence));`                                           | `<unistd.h>`   | Reads data from `fifo1` into the array `sentence`.                                                                    |
| `close(fd1);`                                                                      | `<unistd.h>`   | Closes the read-end of the pipe to free resources.                                                                    |
| `fopen("output.txt", "w");`                                                        | `<stdio.h>`    | Opens (or creates) a file named `output.txt` in **write mode**. Used to store the results (characters, words, lines). |
| `fprintf(fp, "Characters: %d\nWords: %d\nLines: %d\n", characters, words, lines);` | `<stdio.h>`    | Writes formatted output (like printf but to a file) into `output.txt`.                                                |
| `fclose(fp);`                                                                      | `<stdio.h>`    | Closes the file after writing. Always necessary to save data.                                                         |
| `fopen("output.txt", "r");`                                                        | `<stdio.h>`    | Opens the same file again, but now in **read mode**, to read the results for sending back to process1.                |
| `fread(result, sizeof(char), sizeof(result), fp);`                                 | `<stdio.h>`    | Reads the content of `output.txt` into the buffer `result`.                                                           |
| `close(fd2);`                                                                      | `<unistd.h>`   | Closes the second FIFO after writing.                                                                                 |
| `write(fd2, result, sizeof(result));`                                              | `<unistd.h>`   | Sends the content of `result` through `fifo2` back to process1.                                                       |
| `for (int i = 0; sentence[i] != '\0'; i++)`                                        | (C Loop)       | Loops through each character in the sentence to count characters, words, and lines.                                   |


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
