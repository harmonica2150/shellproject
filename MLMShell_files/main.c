//****************************************************//
// Laura Sorenson, Malachi Mart, and Monica Pervis    //
// Project 1                                          //
// Operating Systems                                  //
// March 19, 2014                                     //
//****************************************************//


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <assert.h>

char *cdir;
void batch(char *b); //reads a batch file and excecutes the contents
void run(char *line); // calls correct function based on user input
void empties(); //called when no user input is detected
void cd(char *path); //changes the current directory 
void dir(char *input); //lists current directory contents
void clr(); //clears the screen of previous commands
void env(); //displays the environment variables
void echo(char *string); //repeats inputted content on a new line
void help(); //displays the help content
void paws(); //pauses the shell

/* Handles all calls. Specifically, it decides whether the user is using a batch file, or inputting lines manually.
*/
int main(int argc, char *argv[]){
    (getcwd(cdir, sizeof(cdir)) != NULL);
    if(argc==1){
        empties(); //There is no parameter if only one argument is present.
    }else if(argc==2){
        batch(argv[1]); //Two arguments signals reading from a batch file.
    }else{
        fprintf(stdout,"why so many parameters!?!?");
    }
    return 1;
}

/* Called when the program has no parameters passed into it.
*/
void empties(){
    cd(NULL);               //print current location
    fprintf(stdout,">");    //print >
    while(1){
        char command[1024];
        gets(command);
        char *input = strtok(command, " "); //split the input up by spaces
        run(input);                         //run the command
        cd(NULL);                           //print the next line
        fprintf(stdout,">");                //and > again
    }
}

/* Read batch file and execute the commands inside it 
*/
void batch(char *b){
    FILE *f = fopen(b,"r");                   //open the file and read from it
    char *line;                               //to hold the line
    while(fscanf(f, "%s\n", line) != EOF){    //go through each line until the end of the file
        run(line);                            //run each line
    }
}

/* Handles all inputs and calls the respective functions.
*/
void run(char *line){                         //function to run commands
    if(strcmp(line,"cd")==0)
        cd(line);                             //call if "cd" is entered, pass in the line
    else if(strcmp(line,"dir")==0)
        dir(line);                            //call dir if "dir" is entered, pass in the line
    else if(strcmp(line,"clr")==0)
        clr();                                //call crl if "clr" is entered
    else if(strcmp(line,"environ")==0)
        env();                                //call env if "environ" is entered
    else if(strcmp(line,"echo")==0)
        echo(line);                           //call echo if "echo" is entered, pass in the line
    else if(strcmp(line,"help")==0)
        help();                               //call help if "help" is entered
    else if(strcmp(line,"pause")==0)
        paws();                               //call paws if "pause" is entered
    else if(strcmp(line,"quit")==0)
        exit(1);                              //exit shell if "quit" is entered
    else{                                     //otherwise, fork!
        //lets make a new child process
        char *args[20];
        int a = 0;
        int b = 20;
        while (line != NULL) {
           if (a == b) {
                b = b * 2;
                a = b;
                *args = (char *)realloc(*args, b);
           }
           args[a] = line;
           a++;
           line = strtok(NULL, " ");
        }
        args[a] = (char *)0;
        pid_t pid = fork();
        char * myPath = malloc(snprintf(NULL, 0, "%s %s", "parent=", getenv("shell")) + 1);
        char *envp[] = { myPath, (char *) 0};
        if(pid == 0) {
            int execReturn = execve(args[0], args, envp);
            if (execReturn == -1){
                printf("execv failed.");
                exit(0);
            }
        }else if(pid < 0)
            printf("\nFork() failed.");
        else{}
    }
}

/*	Prints out the following lines to tell the user how to use myShell
*/
void help() {
    fprintf(stdout, "***** HELP ***** HELP IS HERE ***** HELP ***** HELP IS HERE *****\n");
    fprintf(stdout, "* -------------- Command Manual --------------- *\n");
    fprintf(stdout, "* @ cd - Transports the lucky bloke to a target directory *\n");
    fprintf(stdout, "* @ clr - Removes the presence of all that has transpired. *\n");
    fprintf(stdout, "* @ dir - Generously provides the files/folders of the current directory. *\n");
    fprintf(stdout, "* @ enviro - Delivers a pedantic ecological dissertation on the environmental variables. *\n");
    fprintf(stdout, "* @ echo - Mimics the command given, be careful. *\n");
    fprintf(stdout, "* @ help - Here we are my good sir. *\n");
    fprintf(stdout, "* @ pause - Wait for it... *\n");
    fprintf(stdout, "* @ quit - Delivers the final blow to this shell. Same as: exit, and kill. *\n");
    fprintf(stdout, "***** HELP ***** HELP IS HERE ***** HELP ***** HELP IS HERE *****\n");
}

/*	Prints everything after the command "echo" in a new line.
*/
void echo(char *input) {
    
	if (input == NULL)
        fprintf(stdout, "Please enter something to echo");  //If nothing is in the rest of the line, tell the user why nothing was echoed.
    else{
        input = strtok(NULL, " ");                          //break everything into tokens
        while(input !=NULL){
            fprintf(stdout, "%s ", input);                  //print out the tokens
            input = strtok(NULL, " ");  
        }
        fprintf(stdout, "\n");                              //endline
    }
}

/*	Prints out the environment variables. It will print the location
		of the shell.
*/
void env() {
    extern char **environ;
    int i = 0;
    while (environ[i])
        fprintf(stdout, "%s\n", environ[i++]); //go through and print all the environment strings
}

/*	Lists contents of a directory. If no directory is specified, it will list the current
		directory's contents.
*/
void dir(char *input) {
    char *link;
    DIR *dir;
    struct dirent *file;
    input = strtok(NULL, " ");
    if (input == NULL)
        link = ".";
    else
        link = input;
    
    if ((dir = opendir(link)) == NULL)
        perror("opendir() error");
    while((file = readdir(dir)) != NULL)
        fprintf(stdout, " %s\t", file->d_name);
    fprintf(stdout, "\n");
}

/* Clears the screen of all content.
*/
void clr() {
    printf("\033[2J\033[1;1H"); //clears the terminal
}

/*	Changes the current directory, or lists directory contents if directory is 
        unspecified.  
*/
void cd(char *input) {
   	input = strtok(NULL, " "); //tokenize the input
    if(input == NULL) {        //if nothing to change to
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL){ //just print out the same path we were already in and wait for input
            fprintf(stdout,"%s",cwd);
        }
    }else if(input[0] == '/'){      //if / is there, we'll be changing from the root
        fprintf(stdout,"changing from root");
        chdir(input);               //change the direcctory to what was entered in the input
    }else {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL){  //otherwise
            char p[strlen(input)+1];
            strcat(p,input);                    //add input to the end of p
            strcat(cwd,p);                      //add the new p to the end of cwd
            chdir(input);                       //change the directory
        }
    }
    
}

/*  Pauses the shell, which allows the user to input any keystroke within 
		the shell without manipulating the shell in anyway. Only when the
		user presses the [Enter] key will the shell return to active */
void paws(){
    fprintf(stdout, "Please press 'ENTER' to continue.");   //tell the user what's going on
    gets(NULL);                                             //pauses everything until enter is pressed
}


