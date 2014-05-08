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
		printf("%d\n", i);	//checking
		if ((argv[i] = strtok(cmdp, "|\t\n")) == NULL)
			break;
		cmdp = NULL;
	}
	
	*argcp = i;
	return true;
}

static bool getargs(char *part, char *argv[], int max){
	
	char *cmdp;
	int i;
	
	cmdp = part;
	
	for(i = 0; i < max; i++) {
		
		if ((argv[i] = strtok(cmdp, " \t\n")) == NULL)
			break;
		cmdp = NULL;
	}
	
	
	return true;
}

pid_t create_process(char *part, int pipes[][2], int proc, int max) {
	int i;
	char *argv[MAXARG];	
	pid_t pid;

	if (getargs(part, argv, MAXARG)) printf("hi\n");
		
	pid = fork();
	if (pid==-1) {
		perror("fork"); /* something went wrong */
		exit(1); /* parent exits */
	}
	if (pid==0) {
		printf(" CHILD: My PID is %d\n", getpid());
		
		if(pipes[proc][STDIN_FILENO] >= 0) dup2(pipes[proc][STDIN_FILENO], STDIN_FILENO); // FD 0
        	if(pipes[proc][STDOUT_FILENO] >= 0) dup2(pipes[proc][STDOUT_FILENO], STDOUT_FILENO); // FD 1

        	// Close all pipes
        	for(i=0; i<max; i++) {
            		if(pipes[i][STDIN_FILENO] >= 0) close(pipes[i][STDIN_FILENO]);
           		if(pipes[i][STDOUT_FILENO] >= 0) close(pipes[i][STDOUT_FILENO]);
		}



		execvp(argv[0], argv); 
		perror("Error exec!\n");

		exit(1);
	}
	return;
}
static void execut4(int argc, char *parts[MAXARG]){
	
	pid_t children[argc];
	int pipes[argc][2];
		
	int i;

	for(i=0; i<argc; i++)
        {
	        pipes[i][STDIN_FILENO]=-1; 
		pipes[i][STDOUT_FILENO]=-1;
        }	

	for(i=0; i<(argc-1); i++)
        {
            int p[2];
		
            pipe(p);

            pipes[i][STDOUT_FILENO]=p[STDOUT_FILENO]; // Process N writes to the pipe
            pipes[i+1][STDIN_FILENO]=p[STDIN_FILENO]; // Process N+1 reads from the pipe
        }

	for (i=0; i<argc; i++){
		children[i]=create_process(parts[i], pipes, i, argc); 
	}

	for(i=0; i<argc; i++)
        {
            if(pipes[i][STDIN_FILENO] >= 0) close(pipes[i][STDIN_FILENO]);
            if(pipes[i][STDOUT_FILENO] >= 0) close(pipes[i][STDOUT_FILENO]);
        }


	for(i=0; i<argc; i++)
        {
                int stat;

                waitpid(children[i], &stat, 0);
		printf("Done waiting\n");
                fprintf(stderr, "Child %d returned %d\n",
                        i, WEXITSTATUS(stat));
        }
	return;
}

int main(void){
	
	char *argvp[MAXARG];
	int argc;
	bool eof;
	
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
