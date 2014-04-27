#include "libs.h"

int main(void) {
	pid_t pid;
	
	char cm[50];
	char* rea[1];
while (1){
	
	printf("myshell1 ");
	//fgets(cm, 49, stdin);
	//cm[49] = 0;
	scanf("%s", &cm);
	rea[0] = cm;
	switch(pid = fork()) {
	case -1:
		perror("fork"); /* something went wrong */
		exit(1); /* parent exits */
	case 0:
	
		printf(" CHILD: My PID is %d\n", getpid());
		printf(" CHILD: My parent's PID is %d\n", getppid());
		printf(" CHILD: My parent's command is %s\n", &rea);
		execvp(rea[0], commands12); 
		perror("Error exec!\n");
		exit(1);
	default:
		printf("PARENT: My PID is %d\n", getpid());
		printf("PARENT: My child's PID is %d\n", pid);
		printf("PARENT: I'm now waiting for my child to exit()...\n");
		wait(NULL);
		printf("PARENT: I'm out of here\n");
		}
}
	return 0;
}
