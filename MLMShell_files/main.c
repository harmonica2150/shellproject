#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

#define MAX_BUFFER 1024
#define ARGS_BUFFER 64
#define tokens " \n\t"


int main(int argc, char **argv)
{
    char userInputBuffer[MAX_BUFFER];	// contains user input
    char * args[ARGS_BUFFER];	// array of tokenized arguments
    char ** arg;	// allow input of args into args[]
    char path[MAX_BUFFER];	// store current path
    char shellLocation[MAX_BUFFER];	// store current location of shell
    char dirCommand[MAX_BUFFER];	// store "cd " and <directory> specified
    char helpCommand[MAX_BUFFER];
    
    getcwd(shellLocation, sizeof(shellLocation));	// store location of shell for environ
    getcwd(path, sizeof(path));	// get current path, store in "path"
    
    printf("%s> ",path);	// command prompt containing the current path
    fflush(stdout);
    fgets(userInputBuffer,MAX_BUFFER,stdin); // put input into userInputBuffer
    
    /* Tokenize user input. This block of code will tokenize the user input using
    the tokens defined at the top, and store it in the args char array.
    */
    arg = args;
    *arg++ = strtok(userInputBuffer, tokens); // tokenizes first input
    while (*arg++ = strtok(NULL, tokens));	// tokenizes next input
    
    
    
    /* This is where all the commands will be defined. After taking the user input,
    this program will enter the while loop below and stay in the loop until
    the user enters "quit".
    */
    while(1)
    {
        /* Clear the terminal. This will use the system's terminal to clear
the terminal of all input and output that has been processed
by any previous commands.
*/
        if (!strcmp(args[0],"clr")) // if first arg is "clear"
        {
            system("clear");	// send clear command to unix term
        }
        
        /* List the directory contents. This will use the terminal's "ls" command
to display all the contents of the folder specified, including permissions
and hidden files. If no directory is specified, it will list the current
directory's contents.
*/
        if (!strcmp(args[0],"dir"))
        {
            if (args[1] != NULL)	// if <directory> exists
            {
                sprintf(dirCommand, "ls -al %s", args[1]);
                system(dirCommand);	// send "ls -al <directory>" to term
            }
            else
            {
                system("ls -al");	// if <directory> does not exist
            }
        }
        
        /* Print everything after the command "echo" in a new line. Will
reduce all tabs and double spaces to one whitespace.
*/
        if (!strcmp(args[0],"echo"))
        {
            int i = 1;
            while ( args[i] != NULL)
            {
                printf("%s ", args[i]);
                i++;
            }
            printf("\n");
        }
        
        /* Prints out the environment variables. It will print the location
the shell, including the binary name, and the current directory.
*/
        if (!strcmp(args[0],"environ"))
        {
            printf("shell= %s",shellLocation, "%s", argv[0]);
            printf("\nCurrent Directory: %s\n", path);
        }
        
        /* Change the current directory, or list directory contents if no
directory is specified. It uses the POSIX function 'chdir' to
change directory. If
*/
        if (!strcmp(args[0],"cd"))
        {
            if (args[1] == NULL)
            {
                system("ls -al");
            }
            if (chdir(args[1]) != 0)
            {
                perror("cd");
            }
        }
        
        /* Pause the shell. It allows the user to input any keystroke within
the shell without manipulating the shell in anyway. Only when the
user presses the [Enter] key will the shell return back to the
command prompt.
*/
        if (!strcmp(args[0],"pause"))
        {
            system("read -p \"Press [Enter] to continue...\"");
        }
        
        if (!strcmp(args[0],"help"))
        {
            sprintf(helpCommand,"more %s/readme", shellLocation);
            system(helpCommand);
        }
        
        if (!strcmp(!args[0],"quit")) // while first arg is NOT "quit"
        {
            break;
        }
        
        if(strcmp(args[0], "quit" || "help" || "pause" || "cd" || "environ" || "echo" || "dir" || "clr")){
            pid_t pid;
            pid = fork();
            if (pid == -1) {
                /* When fork() returns -1, an error occured. */
                perror("fork failed");
                exit(EXIT_FAILURE);
            }
            else if (pid == 0) {
                /* When fork() returns 0, we are in the child process. */
                printf("Child: PID of Child = %ld\n", (long) getpid());
                execvp(args[0], args); /* arg[0] has the command name. */
                exit(EXIT_SUCCESS);  /* exit() is unreliable here, so _exit must be used */
            }
            else {
                /* When fork() returns a positive number, we are in the parent process
                   and the return value is the PID of the newly created child process. */
                int status;
                (void)waitpid(pid, &status, 0);
            }
        }
        
        getcwd(path, sizeof(path));
        // give prompt, take user input
        printf("%s> ", path);
        fflush(stdout);
        fgets(userInputBuffer,MAX_BUFFER,stdin);
        
        /* Tokenize user input. This block of code will tokenize the user input using
the tokens defined at the top, and store it in the args char array.
*/
        arg = args;
        *arg++ = strtok(userInputBuffer, tokens);
        while(*arg++ = strtok(NULL, tokens));
    }
    printf("Exiting myShell...\n");
    return EXIT_SUCCESS;
}

void readBatchFile(char filename[MAX_FILENAME]) {
    FILE *fp;
    int lineNo = 1;
    
    fp = fopen(filename, "r");	// open the batch file
    if(fp == NULL)
        error("Batch file does not exist");
    
    while (fgets (lineInput, MAX_BUFFER, fp )) {	// read a line
        printf("%d. %s", lineNo++, lineInput);	// print the command number
        tokenize();	// tokenize the line
        runCommand();	// run command
        putchar('\n');
    }
    
    fclose(fp);	// close the file
    exit(0);	// exit when at the end of the file
}
