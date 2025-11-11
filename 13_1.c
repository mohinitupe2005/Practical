#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    char sentence[200], result[200];
    int fd1, fd2;

    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);

    printf("Enter a sentence: ");
    fgets(sentence, sizeof(sentence), stdin);

    fd1 = open("fifo1", O_WRONLY);
    write(fd1, sentence, strlen(sentence) + 1);
    close(fd1);

    fd2 = open("fifo2", O_RDONLY);
    read(fd2, result, sizeof(result));
    close(fd2);

    printf("\n--- Result from Process 2 ---\n");
    printf("%s\n", result);

    return 0;
}





























































#include <stdio.h>      // For input/output functions like printf, scanf, fopen
#include <stdlib.h>     // For exit() and system-related functions
#include <unistd.h>     // For read(), write(), and close()
#include <fcntl.h>      // For open() and file control options
#include <sys/stat.h>   // For mkfifo() and permission constants

int main() {
    int fd1, fd2;                  // File descriptors for two FIFOs
    char sentence[200];            // To store input sentence
    char result[200];              // To store result read from process2

    // Create two named pipes (FIFOs) for full-duplex communication
    // 0666 -> read & write permission for everyone
    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);

    // Open first FIFO in write-only mode (to send data)
    fd1 = open("fifo1", O_WRONLY);

    printf("Enter a sentence: ");
    fgets(sentence, sizeof(sentence), stdin);  // Read sentence from user input

    // Write the input sentence into fifo1 so that process2 can read it
    write(fd1, sentence, sizeof(sentence));
    close(fd1);    // Close after writing to free the pipe

    // Open second FIFO in read-only mode (to receive processed result)
    fd2 = open("fifo2", O_RDONLY);
    read(fd2, result, sizeof(result));   // Read the result from process2
    close(fd2);    // Close FIFO after reading

    // Display final output on screen
    printf("\n--- Processed Result ---\n");
    printf("%s\n", result);

    return 0;
}



| Function                                    | Header File    | Description                                                                                                                                                                               |
| ------------------------------------------- | -------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `mkfifo("fifo1", 0666);`                    | `<sys/stat.h>` | Creates a **named pipe (FIFO file)** named `fifo1` with permission `0666`. This allows both processes to communicate. <br>**0666** → read & write permission for user, group, and others. |
| `open("fifo1", O_WRONLY);`                  | `<fcntl.h>`    | Opens the FIFO file `fifo1` for **writing only**. Returns a file descriptor (integer) used to write data.                                                                                 |
| `fgets(sentence, sizeof(sentence), stdin);` | `<stdio.h>`    | Reads a full line (string) from standard input (keyboard) and stores it into `sentence`.                                                                                                  |
| `write(fd1, sentence, sizeof(sentence));`   | `<unistd.h>`   | Writes data from `sentence` into the FIFO represented by `fd1`. This sends the data to the other process.                                                                                 |
| `close(fd1);`                               | `<unistd.h>`   | Closes the FIFO file so it can be reused later and releases the file descriptor.                                                                                                          |
| `open("fifo2", O_RDONLY);`                  | `<fcntl.h>`    | Opens the second FIFO `fifo2` for **reading only**.                                                                                                                                       |
| `read(fd2, result, sizeof(result));`        | `<unistd.h>`   | Reads data (output from process2) from the FIFO `fifo2` into the `result` buffer.                                                                                                         |
| `close(fd2);`                               | `<unistd.h>`   | Closes the second FIFO.                                                                                                                                                                   |
| `printf()`                                  | `<stdio.h>`    | Used to display text or variables to the console.                                                                                                                                         |
