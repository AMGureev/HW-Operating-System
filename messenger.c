#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>

volatile sig_atomic_t exit_flag = 0;

// Function to establish starting position and determine if the process is the writer
bool starting_position(int read_fd, int write_fd)
{
    pid_t current_pid = getpid(), other_pid;

    // Write current process ID to the other process
    size_t written = write(write_fd, &current_pid, sizeof(pid_t));

    if (written != sizeof(pid_t))
    {
        printf("Can't write PID!\n");
        exit(1);
    }

    // Read the other process ID
    size_t read_pid_size = read(read_fd, &other_pid, sizeof(pid_t));

    if (read_pid_size != sizeof(pid_t))
    {
        printf("Can't read PID!\n");
        exit(1);
    }

    printf("Current PID: %d Other PID: %d\n", current_pid, other_pid);

    return current_pid > other_pid; // We are the top => we write first
}

// Signal handler for graceful exit
void handle_exit_signal(int signum)
{
    exit_flag = 1;
}

int main(int argc, char *argv[])
{
    // Create named pipes
    mknod(argv[1], S_IFIFO | 0666, 0);
    mknod(argv[2], S_IFIFO | 0666, 0);

    srand(getpid()); // Ensure messages differ
    signal(SIGINT, handle_exit_signal);
    int read_fd, write_fd;

    // Open FIFOs for communication
    if ((read_fd = open(argv[1], O_RDONLY | O_NONBLOCK)) < 0)
    {
        printf("Can't open FIFO to read from!\n");
        exit(1);
    }

    if ((write_fd = open(argv[2], O_WRONLY)) < 0)
    {
        printf("Can't open FIFO to write to!\n");
        exit(1);
    }

    // Disable O_NONBLOCK for the read end
    fcntl(read_fd, F_SETFL, fcntl(read_fd, F_GETFL) & (~O_NONBLOCK));

    bool is_writer = starting_position(read_fd, write_fd);

    // Main communication loop
    while (!exit_flag)
    {
        if (is_writer)
        {
            // Writer process generates and sends a random message
            int message = rand();
            size_t written = write(write_fd, &message, sizeof(int));

            if (written != sizeof(int))
            {
                printf("Can't write message!\n");
                exit(1);
            }

            printf("Send message %d\n", message);
        }
        else
        {
            // Reader process receives and prints the message
            int message;
            size_t read_bytes = read(read_fd, &message, sizeof(int));

            printf("Read message: %d\n", message);
            // If we create the lucky number - exit the program
            if (message == 999999999)
            {
                printf("LUCKY NUMBER\n");
                exit(0);
            }

            if (read_bytes != sizeof(int))
            {
                printf("Can't read message!\n");
                exit(1);
            }
        }

        sleep(1); // Avoids high CPU usage
    }

    // Cleanup and exit
    close(read_fd);
    close(write_fd);
    unlink(argv[1]);
    unlink(argv[2]);

    printf("Writer exit\n");
    return 0;
}
