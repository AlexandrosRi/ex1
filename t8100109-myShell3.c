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

pid_t create_process(char *part, int pipes[2][2], int procnum)
{
	
	char *argv[MAXARG];	
	pid_t pid;

	if (getargs(part, argv, MAXARG)) printf("hi\n");
		
	switch(pid = fork()) {
	case -1:
		perror("fork"); /* something went wrong */
		exit(1); /* parent exits */
	case 0:
		printf(" CHILD: My PID is %d\n", getpid());
//cp--------------------------------------------------------------------------------		
		if(procnum==1){
			printf("CHILD: procnum 1\n");
			close(pipes[0][STDIN_FILENO]);
			dup2(pipes[0][STDOUT_FILENO], STDOUT_FILENO); 
			close(pipes[0][STDOUT_FILENO]);
			close(pipes[1][STDIN_FILENO]);
			close(pipes[1][STDOUT_FILENO]);	
		}
	        	
		if(procnum==2){
			printf("CHILD: procnum 2\n");
			//close(pipes[1][STDOUT_FILENO]);
			dup2(pipes[1][STDIN_FILENO], STDIN_FILENO);//printf("CHILD: procnum 2*2\n");
			close(pipes[1][STDIN_FILENO]);//printf("CHILD: procnum 2*3\n");
			close(pipes[0][STDIN_FILENO]);//printf("CHILD: procnum 2*4\n");
			close(pipes[0][STDOUT_FILENO]); 
		}
	
       		// Close all pipes
        	//if(pipes[STDIN_FILENO] >= 0) close(pipes[STDIN_FILENO]);
        	//if(pipes[STDOUT_FILENO] >= 0) close(pipes[STDOUT_FILENO]);
		//if(pipes[1][STDIN_FILENO] >= 0) close(pipes[1][STDIN_FILENO]);
        	//if(pipes[1][STDOUT_FILENO] >= 0) close(pipes[1][STDOUT_FILENO]);
//cp--------------------------------------------------------------------------------		
		printf("CHILD: procnum 2*4 %d\n", procnum);		
		execvp(argv[0], argv); 
		printf("CHILD: procnum 2*5 %d\n", procnum);		
		perror("Error exec!\n");
		
		exit(1);
	/*default:
		printf("PARENT: My PID is %d\n", getpid());
		close(pipes[0][STDIN_FILENO]);
        	close(pipes[0][STDOUT_FILENO]);
		close(pipes[1][STDIN_FILENO]);
        	close(pipes[1][STDOUT_FILENO]);
		wait(NULL);
		printf("PARENT: Done waiting %d\n", getpid());
		}*/}
	return pid;
}
static void execut3(int argc, char *parts[MAXARG]){
	
	pid_t children[2];
	int pipes[2][2];
	int p[2];
	int stat0, stat1;
	
	pipes[0][STDIN_FILENO]=-1;
	pipes[0][STDOUT_FILENO]=-1;
	pipes[1][STDIN_FILENO]=-1;
	pipes[1][STDOUT_FILENO]=-1;
	
	pipe(p);
	
	pipes[0][STDOUT_FILENO]=p[STDOUT_FILENO]; // Process N writes to the pipe
        pipes[1][STDIN_FILENO]=p[STDIN_FILENO]; // Process N+1 reads from the pipe	
	
	children[0] = create_process(parts[0], pipes, 1); 
	children[1] = create_process(parts[1], pipes, 2);

	if(pipes[0][STDIN_FILENO] >= 0) close(pipes[0][STDIN_FILENO]);
        if(pipes[0][STDOUT_FILENO] >= 0) close(pipes[0][STDOUT_FILENO]);
	if(pipes[1][STDIN_FILENO] >= 0) close(pipes[1][STDIN_FILENO]);
        if(pipes[1][STDOUT_FILENO] >= 0) close(pipes[1][STDOUT_FILENO]);
	
	waitpid(children[0], &stat0, 0);
		printf("Done waiting\n");
                fprintf(stderr, "Child 0 returned %d\n",
                        WEXITSTATUS(stat0));
	waitpid(children[1], &stat1, 0);
		printf("Done waiting\n");
                fprintf(stderr, "Child 1 returned %d\n",
                        WEXITSTATUS(stat1));
	
	return;
}

int main(void){
	
	char *argvp[MAXARG];
	int argc;
	bool eof=true;
	
	while (true) {
		printf("myshell3> ");
		if (split_pipes(&argc, argvp, MAXARG, &eof) && argc > 0) {		
			//if (getargs(&argc, argv, MAXARG) && argc > 0) {
				printf("hi2\n");				
				execut3(argc, argvp);
			//}
		}
		if (eof){
			printf("\n");			
			exit(EXIT_SUCCESS);
		}
	}
}
