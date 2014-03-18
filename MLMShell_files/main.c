#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

#define MAX_BUFFER 1024
#define ARGS_BUFFER 64
#define tokens " \n\t"

//void cd();
//void clr();
//void dir();
//void environ(char);
//void echo();
//void help();
//void paws(); //word pause causes an error
//void quit();


int main(int argc, char **argv)
{
    char userInputBuffer[MAX_BUFFER];	// contains user input
    char * args[ARGS_BUFFER];	// array of tokenized arguments
    char ** arg;	// allow input of args into args[]
    char path[MAX_BUFFER];	// store current path
    char shellLocation[MAX_BUFFER];	// store current location of shell
    char dirCommand[MAX_BUFFER];	// store "cd " and <directory> specified
    char helpCommand[MAX_BUFFER];
    char *errmsg;

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
    while(strcmp(args[0],"quit")) // while first arg is NOT "quit"
    {
    /* Clear the terminal. This will use the system's terminal to clear
    the terminal of all input and output that has been processed
    by any previous commands.
    */
        if (!strcmp(args[0],"clr")) // if first arg is "clear"
        {
            system("clear");	// send clear command to unix term
        }

//        /* List the directory contents. This will use the terminal's "ls" command
//        to display all the contents of the folder specified, including permissions
//        and hidden files. If no directory is specified, it will list the current
//        directory's contents.
//        */
//        if (!strcmp(args[0],"dir"))
//        {
//            dir();
//        }

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

//        /* Prints out the environment variables. It will print the location
//        the shell, including the binary name, and the current directory.
//        */
//        if (!strcmp(args[0],"environ"))
//        {
//            environ(args[0]);
//        }

//        /* Change the current directory, or list directory contents if no
//        directory is specified. It uses the POSIX function 'chdir' to
//        change directory. If
//        */

//        if (!strcmp(args[0],"cd"))
//        {
//            cd();
//        }

        /* Pause the shell. It allows the user to input any keystroke within
        the shell without manipulating the shell in anyway. Only when the
        user presses the [Enter] key will the shell return back to the
        command prompt.
        */
        if (!strcmp(args[0],"pause"))
        {
            system("read -p \"Please press 'Enter' to continue...\"");
        }

//        if (!strcmp(args[0],"help"))
//        {
//            help();
//        }

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





///* Change the current directory, or list directory contents if no
//directory is specified */
//void cd(){
//    if (args[1] == NULL)
//    {
//        system("ls -al");
//    }
//        if (chdir(args[1]) != 0)
//        {
//            perror("cd");
//        }
//}

///* Clear the terminal. This will use the system's call to clear
//the terminal of all previous commands. */
//void clr() {
//    system("clear");
//}

///* List the directory contents.  If no directory is specified, it will list the current
//directory's contents. */
//void dir(){
//    if (args[1] != NULL)	// if <directory> exists
//    {
//        sprintf(dirCommand, "ls -al %s", args[1]);
//        system(dirCommand);	// send "ls -al <directory>" to term
//    }
//    else
//    {
//        system("ls -al");	// if <directory> does not exist
//    }
//}

///* Prints out the environment variables. It prints the location
//of the shell, including the binary name, and the current directory.*/
//void environ(char arg0){
//    printf("Shell: %s",shellLocation, "%s", arg0);
//    printf("\nCurrent Directory: %s\n", path);
//}

//void echo(){
//    int i = 1;
//    while ( args[i] != NULL)
//    {
//        printf("%s ", args[i]);
//        i++;
//    }
//    printf("\n");
// }

//void paws (){ //word pause causes an error
//    system("read -p \"Please press 'Enter' to continue...\"");
//}


//void quit(){
//    printf("Exiting myShell...\n");
//    return EXIT_SUCCESS;
//}
