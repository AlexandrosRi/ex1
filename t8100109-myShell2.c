#include "8100109.h"


static bool getargs(int *argcp, char *argv[], int max/*, bool *eofp*/){
	
	static char cmd[MAXLINE];
	char *cmdp;
	int i;
	
	//*eofp = false;
	
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


static void execut1(int argc, char *argv[]){

	pid_t pid;
	
	switch(pid = fork()) {
	case -1:
		perror("fork"); /* something went wrong */
		exit(1); /* parent exits */
	case 0:
		printf(" CHILD: My PID is %d\n", getpid());
		execvp(argv[0], argv); 
		perror("Error exec!\n");
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
	//bool eof;
	
	while (true) {
		printf("myshell2 ");
		if (getargs(&argc, argv, MAXARG) && argc > 0) {
			execut1(argc, argv);
		}
	}
}
