#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int count(char* buffer)
{
    int count=0;
    //do some stuff here, come on this should be easy guys!
    return count;
}

int main(int argc, char **argv)
{
        //buffer is to hold the commands that the user will type in
        char buffer[512];
        // /bin/program_name is the arguments to pass to execv
        //if we want to run ls, "/bin/ls" is required to be passed to execv()
        char* path = "/bin/";

        while(1)
        {
                //print the prompt
                printf("myShell>");
                //get input
                fgets(buffer, 512, stdin);
                //fork!
                int pid = fork();
                //Error checking to see if fork works
                //If pid !=0 then it's the parent
                if(pid!=0)
                {
                        wait(NULL);
                }
                else
                {
                        int no_of_args = count(buffer);
            //we plus one so that we can make it NULl
            char** array_of_strings = malloc((sizeof(char*)*(no_of_args+1)));

            //here we break the string up and create an array of pointers that point to each of the arguments.
            int count=0;
            char* pch2;
            pch2 = strtok (buffer," ");
            while (pch2 != NULL)
            {
                array_of_strings[count]=(char*)malloc((sizeof(char)*strlen(pch2)));
                strcpy(array_of_strings[count], pch2);
                //printf("[%d]:%sn", count, array_of_strings[count]);
                pch2 = strtok (NULL, " ");
                count++;
            }

            //format for command is eg. ls -a -l
            //therefore the first element in the array will be the program name
            //add the path so it'll be /bin/command eg. /bin/ls
            char* prog = malloc(sizeof(char)*(strlen(array_of_strings[0]+strlen(path))));
            prog = strcat(strcpy(prog, path),array_of_strings[0]);


            /*
            int k=0;
            for(k=0; k<(no_of_args); k++)
                printf(">>>>%sn", array_of_strings[k]);
            */

                        //pass the prepared arguments to execv and we're done!
                        int rv = execv(prog, array_of_strings);
                }
        }
        return 0;
}
