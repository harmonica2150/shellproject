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
        noParam();
    }else if(argc==2){
        batch(argv[1]);
        //batch file input
    }else{
        fprintf(stdout,"Too many parameters.");
    }
    return 1;
}
void noParam(){
    cd(NULL);
    fprintf(stdout,">");
    while(1){
        char command[1024];
        gets(command);
        char *input = strtok(command, " ");
        run(input);
        cd(NULL);
        fprintf(stdout,">");
    }
}

void batch(char *b){
    FILE *f = fopen(b,"r");
    char *line;
    while(fscanf(f, "%s\n", line) != EOF){
        run(line);
    }
}
void run(char *line){
    if(strcmp(line,"cd")==0)
        cd(line);
    else if(strcmp(line,"dir")==0)
        dir(line);
    else if(strcmp(line,"clr")==0)
        clr();
    else if(strcmp(line,"enviro")==0)
        env();
    else if(strcmp(line,"echo")==0)
        echo(line);
    else if(strcmp(line,"help")==0)
        help();
    else if(strcmp(line,"pause")==0)
        paws();
    else if(strcmp(line,"quit")==0)
        exit(1);
    else{
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

void help() {
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

void echo(char *input) {
    
	if (input == NULL)
        fprintf(stdout, "Nothing to echo");
    else{
        input = strtok(NULL, " ");
        while(input !=NULL){
            fprintf(stdout, "%s ", input);
            input = strtok(NULL, " ");
        }
        fprintf(stdout, "\n");
    }
}

void env() {
    extern char **environ;
    int i = 0;
    while (environ[i])
        fprintf(stdout, "%s\n", environ[i++]);
}

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

void clr() {
    printf("\033[2J\033[1;1H");
}

void cd(char *input) {
   	input = strtok(NULL, " ");
    if(input == NULL) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL){
            fprintf(stdout,"%s",cwd);
        }
    }else if(input[0] == '/'){
        fprintf(stdout,"changing from root");
        chdir(input);
    }else {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL){
            char p[strlen(input)+1];
            strcat(p,input);
            strcat(cwd,p);
            chdir(input);
        }
    }
    
}
void paws(){
    fprintf(stdout, "Press 'ENTER' to continue.");
    gets(NULL);
}
