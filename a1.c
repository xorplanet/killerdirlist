#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
struct que 
{
char *dirr;
char *name;
};
int emptyq(struct que *ques, long qcount,long resume,char **argv);

int main(int argc, char **argv) {
  //  DIR *dir;
    //struct dirent *dp;
  //  struct stat statbuf;
  //  char *dirbuf[1024];
    struct que *ques;
    ques = malloc(sizeof(struct que));
    
    long qcount=0;
    if (argc < 2) {
        fprintf(stderr, "usage: %s <pathname>\n", argv[0]);
        exit(-1);
    }

    ques[qcount].dirr = malloc((strlen(argv[1])+1)*sizeof(char));
    strcpy(ques[qcount].dirr,argv[1]);
    ques[qcount].name = malloc((strlen(argv[1])+1)*sizeof(char));
    strcpy(ques[qcount].name,argv[1]);
    qcount++;

    long resume = 0;
    emptyq(ques,qcount,resume,argv);


    return 0;
}


int emptyq(struct que *ques, long qcount,long currentq,char **argv){
    DIR *dir;
    struct dirent *dp;
    struct stat statbuf;
    //char *dirbuf[1024];
    char* wholepath;
    char cwd[PATH_MAX];

    while(currentq<qcount)
    {
        //printf("opening:%s %s\n",ques[currentq].dirr,ques[currentq].name);
        if(currentq==0)
        {
            wholepath = malloc((strlen(ques[currentq].dirr)+ 1)* sizeof(char));
            sprintf(wholepath,"%s",ques[currentq].dirr);
        }else{
            wholepath = malloc((strlen(ques[currentq].dirr)+strlen("/")+strlen(ques[currentq].name) + 1)* sizeof(char));
            sprintf(wholepath,"%s%s%s",ques[currentq].dirr,"/",ques[currentq].name);
        }
        printf("\nopening:%s\n",wholepath);
        if ((dir = opendir(wholepath)) == NULL) //deal with permission errors
        {

            perror ("Cannot open");
            if(currentq+1<qcount)
            {
                emptyq(ques,qcount,currentq+1,argv);
            }else
            {
                free(wholepath);
                closedir(dir);
                    for(int j = 0;j<qcount;j++)
                    {
                        free(ques[j].dirr);
                        free(ques[j].name);
                    }
                    free(ques);
                    printf("no more files\nEXIT\n");
                    exit(0);
            }
        }

    
        //chdir(ques[currentq].dirr);
       // chdir(ques[currentq].name);
        chdir(wholepath);
           while ((dp = readdir (dir)) != NULL) 
           {        
                if (lstat(dp->d_name, &statbuf) < 0) 
                {
                    perror(argv[0]);
                    exit(errno);
                }
                
                if (S_ISREG(statbuf.st_mode)) 
                {
                    printf("%s is a regular file.\n",dp->d_name );

                } else if (S_ISDIR(statbuf.st_mode)) 
                {

                    if((strcmp(dp->d_name,"..")!=0)&&(strcmp(dp->d_name,".")!=0))
                    {
                        printf("%s is a directory.\n", dp->d_name);
                       // printf("%ld string size\n",strlen(getcwd(*dirbuf,1024) ));

                        if(qcount!=0)
                        {
                            //que[qcount] = malloc(())
                            ques =realloc(ques, sizeof(struct que)*(qcount+1));
                        }

                        ques[qcount].dirr = malloc((strlen(getcwd(cwd,sizeof(cwd)))+1)*sizeof(char));
                        strcpy(ques[qcount].dirr,getcwd(cwd,sizeof(cwd)));

                        ques[qcount].name = malloc((strlen(dp->d_name)+1)*sizeof(char));
                        strcpy(ques[qcount].name,dp->d_name);
                        //printf("%s:%s\n",ques[qcount].dirr,ques[qcount].name );
                        qcount++;
                    }
                }else if(S_ISLNK(statbuf.st_mode))
                {
                     printf("%s is a symbolic link.\n",dp->d_name );
                }
            }//no more files in dir
            printf("cur:%ld count:%ld",currentq,qcount );
            free(wholepath);
            closedir(dir);
            currentq++;
    }
    // for(int j = 0;j<qcount;j++){
    //    printf("%s:%s\n",ques[j].dirr,ques[j].name );
    //  }

      for(long j = 0;j<qcount;j++){
         free(ques[j].dirr);
         free(ques[j].name);
     }
    free(ques);
printf("no more files\nEXIT\n");

exit(0);
}
