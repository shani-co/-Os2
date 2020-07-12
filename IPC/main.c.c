#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>

static int pingNum = 0;
static int storage = 0;
static int pipefd[2];
static int firstRun= 1;

void sig_handler(int signum){}

int main() {
    //int pipefd[2];
    int returnstatus;
    pid_t pid;
    int childInt;
    int parentInt;

    returnstatus = pipe(pipefd);
   
    if (returnstatus == -1) {
		printf("Unable to create pipe \n");
	 	return 1;
	}
	
	pid = fork();
   	if (pid != 0){ // Parent process
		signal(SIGUSR1,&sig_handler);
		pause();
		while(1){				
			read(pipefd[0],&storage,sizeof(storage));
			if(pingNum >= 5){
				write(pipefd[1],&pingNum,sizeof(pingNum));
				kill(pid,SIGUSR1);
				pause();
				printf("Parent is going to be terminated\n");			
				exit(0);
			}
			pingNum = storage +1;
			printf("%d\n",storage);	
			write(pipefd[1],&pingNum,sizeof(pingNum));	
			kill(pid,SIGUSR1);
			pause();
		}
	} else { //child process
		signal(SIGUSR1,&sig_handler);
		sleep(1);
		while(1){		
			if(firstRun){
				firstRun = 0;
				write(pipefd[1],&pingNum,sizeof(pingNum));
				kill(getppid(),SIGUSR1);
				pause();				
			}else{
				read(pipefd[0],&storage,sizeof(storage));
				if(pingNum > 5){
					write(pipefd[1],&pingNum,sizeof(pingNum));
					printf("Child is going to be terminated\n");
					kill(getppid(),SIGUSR1);
					exit(0);
				}
				pingNum = storage +1;
				printf("%d\n",storage);						
				write(pipefd[1],&pingNum,sizeof(pingNum));
				kill(getppid(),SIGUSR1);
				pause();
			}	
		}	
	}
	return 0;
}
