/* 
 * File:   applets.h
 * Author: sansinyang
 * email:  sansinyang@gmail.com
 * Created on 2017.11.23
 */
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

/* Generate random alphabet basename */
void rand_name(char *fname);

/* Signal handler */
void signalHandler(int sig);

int main(int argc, char *argv[]){

	/* SIGINT contain in mask */
	sigset_t unblockSet, prevMask;
	sigemptyset(&unblockSet);
	sigaddset(&unblockSet, SIGINT);

	sigprocmask(SIG_SETMASK, &prevMask, NULL);
	sigprocmask(SIG_UNBLOCK, &unblockSet, &prevMask);

	pid_t current_pid = getpid();
	pid_t parent_pid = getppid();
	printf("P:%ld C:%ld\n", parent_pid, current_pid);

	/* receive SIGINT */
	signal(SIGINT, signalHandler);

	while(1)
		pause();
}

void signalHandler(int sig){
	char *s="\
			#include <stdio.h>%c\
			#include <signal.h>%c\
			#include <string.h>%c\
			#include <stdlib.h>%c\
			#include <sys/types.h>%c\
			#include <unistd.h>%c\
			void signalHandler(int sig);\
			void rand_name(char *fname);\
			int main(int argc, char *argv[]){\
				sigset_t unblockSet,prevMask;\
				sigemptyset(&unblockSet);\
				sigaddset(&unblockSet,SIGINT);\
				sigprocmask(SIG_SETMASK,&prevMask,NULL);\
				sigprocmask(SIG_UNBLOCK,&unblockSet,&prevMask);\
				pid_t current_pid=getpid();\
				pid_t parent_pid=getppid();\
				printf(%cP:%%ld C:%%ld%cn%c,parent_pid,current_pid);\
				signal(SIGINT,signalHandler);\
				while(1)\
					pause();\
				}\
			void signalHandler(int sig){\
				char *s=%c%s%c;\
				char fname[10];\
				rand_name(fname);\
				char fcname[13];\
				strncpy(fcname,fname,10);\
				strcpy(fcname+10,%c.c%c);\
				FILE* outputFd = fopen(fcname, %cw%c);\
				fprintf(outputFd,s,10,10,10,10,10,10,34,92,34,34,s,34,34,34,34,34,34,34,34,34,92);\
				fclose(outputFd);\
				char command[40];\
				int n=snprintf(command,31,%cgcc -o %%s %%s%c,fname,fcname);\
				system(command);\
				pid_t pid=fork();\
				if(pid==0){\
					execv(fname,NULL);\
				}\
				exit(0);\
				}\
			void rand_name(char *fname){\
					char str[52] = %cABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz%c;\
					int i;\
					srand((int)time(0));\
					for(i=0;i<10;i++){\
						fname[i]=str[rand()%%52];\
					}\
					fname[i]='%c0';}\
		    ";
	char fname[10];
	char fcname[13];

	/* create c program source */
	rand_name(fname);
	strncpy(fcname, fname, 10);
	strcpy(fcname+10, ".c");
	FILE* outputFd = fopen(fcname, "w");
	fprintf(outputFd, s, 10, 10, 10, 10, 10, 10, 34,92, 34,
			34, s, 34, 34, 34, 34, 34, 34, 34, 34, 34, 92);
	fclose(outputFd);

	/* compile the source */
	char command[40];
	int n = snprintf(command, 31, "gcc -o %s %s", fname, fcname);
	system(command);

	/* fork a subroutine then execute compliled file */
	pid_t pid = fork();
	if(pid == 0){
		execv(fname, NULL);
		}
	exit(0);
}

void rand_name(char *fname){
	char str[52] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int i;

	srand((int)time(0));
	for(i = 0; i < 10; i++){
		fname[i] = str[rand() % 52];
		}
	fname[i] = '\0';
}
