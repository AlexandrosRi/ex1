#include "t8100109-myShell.h"

static bool split_pipes(int *argcp, char *argv[], int max, bool *eofp){
	
	static char cmd[MAXLINE];
	char *cmdp;
	int i;
	
	//*eofp = false;
	
	//fgets(cmd, sizeof(cmd), stdin);
	
	if (fgets(cmd, sizeof(cmd), stdin) == NULL){	//null sthn arxh, stamataei
		*eofp = true;
		return false;
	}

	cmdp = cmd;
	
	for(i = 0; i < max; i++) {
		printf("%d\n", i);
		if ((argv[i] = strtok(cmdp, "|\t\n")) == NULL)
			break;
		cmdp = NULL;
	}
	
	*argcp = i;
	return true;
}

static bool getargs(char *part, char *argv[], int max){
	
	//static char cmd[MAXLINE];
	char *cmdp;
	int i;
	
	//*eofp = false;
	
	//fgets(cmd, sizeof(cmd), stdin);
	
	cmdp = part;
	
	for(i = 0; i < max; i++) {
		
		if ((argv[i] = strtok(cmdp, " \t\n")) == NULL)
			break;
		cmdp = NULL;
	}
	
	
	return true;
}

void create_process(char *part)
{
	int i;
	char *argv[MAXARG];	
	pid_t pid;

	if (getargs(part, argv, MAXARG)) printf("hi\n");
		//for (i=0; i<MAXARG; i++) printf("%s",argv[i]);
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
static void execut4(int argc, char *parts[MAXARG]){
	
	int i;
	for (i=0; i<argc; i++){
		create_process(parts[i]);
	}
	return;
}

int main(void){
	
	char *argvp[MAXARG];
	int argc;
	bool eof=true;
	
	while (true) {
		printf("myshell4> ");
		if (split_pipes(&argc, argvp, MAXARG, &eof) && argc > 0) {		
			printf("hi2\n");				
			execut4(argc, argvp);			
		}
		if (eof){
			printf("\n");			
			exit(EXIT_SUCCESS);
		}
	}
}
