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
void batch(char *b);
void run(char *line);
void noParam();
void cd(char *path);
void dir(char *input);
void clr();
void env();
void echo(char *string);
void help();
void paws();


int main(int argc, char *argv[]){
    (getcwd(cdir, sizeof(cdir)) != NULL);
    if(argc==1){
        noParam();       //There is no parameter if only one argument is present.
    }else if(argc==2){
        batch(argv[1]);  //Two arguments signals reading from a batch file.
        //batch file input
    }else{
        fprintf(stdout,"Too many parameters.");  //We don't want more than two.
    }
    return 1;
}
void noParam(){
    cd(NULL);                  //print current location
    fprintf(stdout,">");       //print >
    while(1){
        char command[1024];
        gets(command);
        char *input = strtok(command, " ");   //split the input up by spaces
        run(input);                           //run the command
        cd(NULL);                             //print the next line
        fprintf(stdout,">");                  //and > again
    }
}

void batch(char *b){                          //function to read commands from a batch file
    FILE *f = fopen(b,"r");                   //open the file and read from it
    char *line;                               //to hold the line
    while(fscanf(f, "%s\n", line) != EOF){    //go through each line until the end of the file
        run(line);                            //run each line
    }
}
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
        char *args[20];
        int x = 0;
        int y = 20;
        while (line != NULL) {
           if (x == y) {
                y = y * 2;
                x=y;
                *args = (char *)realloc(*args, y);
           }
           args[x] = line;
           x++;
           line = strtok(NULL, " ");
        }
        args[x] = (char *)0;
        pid_t pID = fork();
        char * myPath = malloc(snprintf(NULL, 0, "%s %s", "parent=", getenv("shell")) + 1);
        char *envp[] = { myPath, (char *) 0};
        if(pID == 0) {
            //execv(envargs[0], envargs);
            int execReturn = execve(args[0], args, envp);
            if (execReturn == -1){
                printf("execv failed.");
                exit(0);
            }
        }else if(pID < 0)
            printf("\nFailed to fork.");
        else{}
    }
}

void help() {     //prints out the help menu
    fprintf(stdout, "***** HELP ***** HELP IS HERE ***** HELP ***** HELP IS HERE *****\n");
    fprintf(stdout, "* -------------- Command Manual --------------- *\n");
    fprintf(stdout, "* @ cd - Transports the lucky bloke to a target directory *\n");
    fprintf(stdout, "* @ clr - Removes the presence of all that has transpired. *\n");
    fprintf(stdout, "* @ dir - Generously provides the files/folders of the current directory. *\n");
    fprintf(stdout, "* @ environ - Delivers a pedantic ecological dissertation on the environmental variables. *\n");
    fprintf(stdout, "* @ echo - Mimics the command given, be careful. *\n");
    fprintf(stdout, "* @ help - Here we are my good sir. *\n");
    fprintf(stdout, "* @ pause - Wait for it... *\n");
    fprintf(stdout, "* @ quit - Delivers the final blow to this shell. Same as: exit, and kill. *\n");
    fprintf(stdout, "***** HELP ***** HELP IS HERE ***** HELP ***** HELP IS HERE *****\n");
}

void echo(char *input) {  //function to echo the rest of the input in the line

    if (input == NULL)
        fprintf(stdout, "Nothing to echo");    //If nothing is in the rest of the line, tell the user why nothing was echoed.
    else{
        input = strtok(NULL, " ");             //break everything into tokens
        while(input !=NULL){
            fprintf(stdout, "%s ", input);     //print out the tokens
            input = strtok(NULL, " ");
        }
        fprintf(stdout, "\n");                 //endline
    }
}

void env() {                   //function to list the environment strings
    extern char **environ;
    int i = 0;
    while (environ[i])
        fprintf(stdout, "%s\n", environ[i++]);  //go through and print all the environment strings
}

void dir(char *input) {       //function to list the contents of the current directory
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

void clr() {  //function to clear the terminal
    printf("\033[2J\033[1;1H");  //clears the terminal
}

void cd(char *input) {  //function to change the directory we're looking at
    input = strtok(NULL, " ");   //tokenize the input
    if(input == NULL) {           //if nothing to change to
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL){   //just print out the same path we were already in and wait for input
            fprintf(stdout,"%s",cwd);
        }
    }else if(input[0] == '/'){                 //if / is there, we'll be changing from the root
        fprintf(stdout,"changing from root");
        chdir(input);                          //change the direcctory to what was entered in the input
    }else {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL){ //otherwise,
            char p[strlen(input)+1];
            strcat(p,input);                   //add input to the end of p
            strcat(cwd,p);                     //add the new p to the end of cwd
            chdir(input);                      //change the directory
        }
    }

}
void paws(){  //function to pause shell operation
    fprintf(stdout, "Press 'ENTER' to continue.");  //tell the user what's going on
    gets(NULL);  //pauses everything until enter is pressed
}
