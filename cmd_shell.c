#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    int pipe1[2];   // Used for parent <-> child communication
    enum {IN, OUT}; // Make naming of pipe "ends" more natural
    pid_t child;    // Current pid of any fork
    
    pipe(pipe1);    // Establish our pipe

    char input[80]; // Create our input buffer

    while (1) {     // Infinite loop
        printf("cmd> ");        // Prompt the user
        gets(input);       // Take their input
        printf("You said: ");   // Confirm the input
        puts(input);            // ..

        child = fork();     // Create a forked process to run the command
        if (child < 0) {    // Check for a fork error and panic nicely
            perror("ERROR: failed to fork child process");
        } 
        else if (child == 0)    // child (the forked PID) is equal to 0?
        {
            // this is the child process
            printf("I am the child process\n");
            dup2(pipe1[OUT], OUT); // Duplicate the pipe, make the child's OUT point to the parent's OUT
            close(pipe1[IN]);      // Not needed anymore
            close(pipe1[OUT]);     // Not needed anymore
            execlp(input, input, NULL, NULL);   // Execute the command entered
        }
        else
        {
            // We are in the parent again
            printf("I am the parent process\n");
            close(pipe1[IN]);   // Not needed anymore
            close(pipe1[OUT]);  // Not needed anymore
            wait(NULL);         // equivalent to waitpid(child, NULL, 0);
            printf("I waited for the child to finish\n");
            // the parent process is done
        }
    }
}