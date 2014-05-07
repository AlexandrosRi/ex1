#include "t8100109-myShell.h"


static bool getargs(int *argcp, char *argv[], int max, bool *eofp){
	
	static char cmd[MAXLINE];
	char *cmdp;
	int i=0;
	
	*eofp = false;
	
	if ((getchar())== EOF){
		*eofp = true;
		return false;
	}

	fgets(cmd, sizeof(cmd), stdin);
	
	cmdp = cmd;
	
	for(i = 0; i < max; i++) {
		if ((argv[i] = strtok(cmdp, " \t\n")) == NULL)
			break;
		cmdp = NULL;
	}
	
	
	*argcp = i;
	return true;
}


static void execut1(int argc, char *argv[]){	//http://beej.us/guide/bgipc/output/print/bgipc_USLetter_2.pdf

	pid_t pid;
	
	switch(pid = fork()) {
	case -1:
		perror("fork"); /* something went wrong */
		exit(1); /* parent exits */
	case 0:
		printf("CHILD:  My PID is %d\n", getpid());
		if (argv[1] != NULL){
			printf("CHILD: Only single word commands!\n");
		}	
		else {
			execlp(argv[0], argv[0], NULL); 
			perror("Error exec!\n");
		}
		exit(1);
	default:
		printf("PARENT: My PID is %d\n", getpid());
		wait(NULL);
		}
		
	return;
}
	
int main(void){
	
	char *argv[MAXARG];
	int argc;
	bool eof;
	
	while (true) {
		printf("myshell1> ");
		if (getargs(&argc, argv, MAXARG, &eof) && argc > 0) {
			execut1(argc, argv);
		}
		if (eof){
			exit(EXIT_SUCCESS);
		}
	}
}
