#include "t8100109-myShell.h"

static bool split_pipes(int *argcp, char *argv[], int max, bool *eofp){
	
	static char cmd[MAXLINE];
	char *cmdp;
	int i;
	
	*eofp = false;
	
	if (fgets(cmd, sizeof(cmd), stdin) == NULL){	//null sthn arxh, stamataei
		*eofp = true;
		return false;
	}

	cmdp = cmd;
	
	for(i = 0; i < max; i++) {
		if ((argv[i] = strtok(cmdp, "|\t\n")) == NULL)
			break;
		cmdp = NULL;
	}
	
	*argcp = i;
	return true;
}

static void getargs(char *part, char *argv[], int max){
	
	char *cmdp;
	int i;
	
	cmdp = part;
	
	for(i = 0; i < max; i++) {
		
		if ((argv[i] = strtok(cmdp, " \t\n")) == NULL)
			break;
		cmdp = NULL;
	}
		
	return;
}

pid_t create_process(char *part, int pipes[2][2], int proc)
{
	
	char *argv[MAXARG];	
	pid_t pid;

	getargs(part, argv, MAXARG);
		
	pid = fork();
	
	if (pid==-1) {
		perror("fork"); /* something went wrong */
		exit(1); /* parent exits */
	}

	if (pid==0) {
		printf(" CHILD: My PID is %d\n", getpid());

		if(proc==1){
			
			close(pipes[0][STDIN_FILENO]);
			dup2(pipes[0][STDOUT_FILENO], STDOUT_FILENO); 
			close(pipes[0][STDOUT_FILENO]);
			close(pipes[1][STDIN_FILENO]);
			close(pipes[1][STDOUT_FILENO]);	
		}
	        	
		if(proc==2){
			
			close(pipes[1][STDOUT_FILENO]);
			dup2(pipes[1][STDIN_FILENO], STDIN_FILENO);
			close(pipes[1][STDIN_FILENO]);
			close(pipes[0][STDIN_FILENO]);
			close(pipes[0][STDOUT_FILENO]); 
		}
	
		execvp(argv[0], argv); 
		perror("Error exec!\n");
		
		exit(1);
	}
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
	
	pipes[0][STDOUT_FILENO]=p[STDOUT_FILENO]; // Process 1 writes to the pipe
        pipes[1][STDIN_FILENO]=p[STDIN_FILENO]; // Process 2 reads from the pipe	
	
	children[0] = create_process(parts[0], pipes, 1); 
	children[1] = create_process(parts[1], pipes, 2);

	if(pipes[0][STDIN_FILENO] >= 0) close(pipes[0][STDIN_FILENO]);
        if(pipes[0][STDOUT_FILENO] >= 0) close(pipes[0][STDOUT_FILENO]);
	if(pipes[1][STDIN_FILENO] >= 0) close(pipes[1][STDIN_FILENO]);
        if(pipes[1][STDOUT_FILENO] >= 0) close(pipes[1][STDOUT_FILENO]);
	
	printf("PARENT: My PID is %d\n", getpid());
	waitpid(children[0], &stat0, 0);		
	waitpid(children[1], &stat1, 0);
		
	return;
}

int main(void){
	
	char *argvp[MAXARG];
	int argc;
	bool eof;
	
	while (true) {
		printf("myshell3> ");
		if (split_pipes(&argc, argvp, MAXARG, &eof) && argc > 0) {		
			execut3(argc, argvp);
		}
		if (eof){
			printf("\n");			
			exit(EXIT_SUCCESS);
		}
	}
}
