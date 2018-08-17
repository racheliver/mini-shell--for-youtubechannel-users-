/**************************************************************************************************************
In this exercise, we will execute a simple C language shell under the Linux operating system,
 the shell will show PROMPT to the user, read the commands and send them to the operating system for execution.
 **************************************************************************************************************/


#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>

#define INPUT_SIZE 510 //The length of the maximum string for the user
#define CUTTING_WORD " \n"//For dividing a string into single words (using in strtok)
#define ENDING_WORD "done"//Program end word
#define RESET 0

 /*****************************Private Function declaration******************************/
char *getcwd(char *buf, size_t size);//show the path Of the current folder
void  DisplayPrompt();//Display Prompt : user@currect dir>
char** execFunction(char *input,char **argv,int *sizeOfArray,int *cmdLength);  //Preparation of a receiver input as an expense
void garbageCollector(char** argv,int size); //Memory Release

 /****************************/
static int *numOfCmd;
static int *cmdLength;
/****************************/
int main() {
    /*****************************************************************/
    numOfCmd = mmap(NULL, sizeof *numOfCmd, PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, RESET);
    cmdLength = mmap(NULL, sizeof *cmdLength, PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, RESET);
    /******************************************************************/
    (*numOfCmd)=RESET;
    (*cmdLength)=RESET;
    int sizeOfArray=RESET;

    char input[INPUT_SIZE]="";//A string array containing the input.
    DisplayPrompt();
    pid_t id; // pid_t use for process identifer
    char **argv;//A string array will containing the program name and command arguments

    while (strcmp(input,ENDING_WORD)!=RESET)
    {
        if(fgets(input,INPUT_SIZE,stdin)==RESET)
            printf(" ");
        //do nothing...countine regular

        argv=execFunction(input,argv,&sizeOfArray,cmdLength);

        if (strcmp("cd",argv[RESET])==RESET)
        {
            struct passwd *pwd;
            char* path=argv[1];

            if(path==NULL)
            {
                pwd=getpwuid(getuid());
                path=pwd->pw_dir;
            }
            if(path[0]=='/')
                (path)=++(path);
            errno=chdir(path);
            DisplayPrompt();
            if(errno!=RESET)
                printf("error changing dircatory");

        }

        else
        {
            id=fork();
            if (id<RESET)
            {
                printf("fork faild");
                exit(RESET);
            }
            else if(id==RESET) {
                (*numOfCmd)++;

                execvp(argv[RESET],argv);
                garbageCollector(argv,sizeOfArray);
                if(strcmp(input,ENDING_WORD)!=RESET)
		printf("command not found\n");
                    exit(1);
            }else {
                wait(&id);
                if (strcmp(input,ENDING_WORD) != RESET)
                {
                    DisplayPrompt();
                }                else {
                    printf("Num of cmd: %d\n", *numOfCmd);
                    printf("cmd length: %d\n", *cmdLength-4);
                    printf("Bye !\n");
                }
            }

        }
    }
    return RESET;
}
void garbageCollector(char** argv,int size)
{
    int i=RESET;
    for (i = RESET; i < size; ++i) {
        free(argv[i]);
    }
    free(argv);
    argv=NULL;
}
char** execFunction(char *input,char **argv,int *sizeOfArray,int *cmdLength)
{
    int i=RESET,counter=RESET;
    char inputCopy[INPUT_SIZE];
    strcpy(inputCopy,input);

    char* ptr= strtok(input,CUTTING_WORD);
    while(ptr!=NULL)
    {
        ptr=strtok(NULL,CUTTING_WORD);
        counter++;
    }
    argv = (char**)malloc((counter+1)*(sizeof(char*)));
    if(argv==NULL)
    {
        printf("error allocated");
        exit(RESET);
    }

    char* ptrCopy= strtok(inputCopy,CUTTING_WORD);
    while(ptrCopy!=NULL)
    {
        if (i==RESET)
            (*cmdLength)+=strlen(ptrCopy);
        argv[i]=(char*)malloc((sizeof(char)+1)*strlen(ptrCopy));
        if(argv[i]==NULL)
        {
            printf("error allocated");
            for (int j = i-1; j >-1 ; j--) {
                free(argv[j]);
            }
            free(argv);
            exit(RESET);
        }
        strcpy(argv[i],ptrCopy);
        argv[i][strlen(ptrCopy)]='\0';
        ptrCopy=strtok(NULL,CUTTING_WORD );
        i++;
    }
    argv[counter]=NULL;
    (*sizeOfArray)=counter;
    return argv;

}

void DisplayPrompt()
{

//-------------------show the path-----------------------------

    long size;
    char *buf;
    char *ptr;

    size = pathconf(".", _PC_PATH_MAX);

    if ((buf = (char *)malloc((size_t)size)) != NULL)
        ptr = getcwd(buf, (size_t)size);


    //----------show the user name root------------------------

    struct passwd *getpwuid(uid_t uid);
    struct passwd *p;
    uid_t uid=0;
    if ((p = getpwuid(uid)) == NULL)
        perror("getpwuid() error");
    else {
        printf("%s@%s>", p->pw_name, ptr);
    }
    free(buf);
}
