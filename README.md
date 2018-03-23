# autosynthesis

Linux light program can not be "killed"

## My little production 

When we send a signal (SIG_XX),
process create subroutine and kill himself rather than be aborted by signal. 

## Unix environment advanced programming  

* sighandler_t signal(int signum, sighandler_t handler); 
* pid_t fork(void);

With the help of my teacher Mr.Jiang,I make good use of the two funtions. 

## You can append extra features:

* int remove(const char *pathname); 
* int socket(int domain, int type, int protocol);
* etc. Make it functional and random duplication ...
