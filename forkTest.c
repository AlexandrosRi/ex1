#include "libs.h"

int main(void) {
	pid_t pid;
	
	
while (1){
	
	printf("myshell1 ");
	
	char cmdline[MAXC];
	char *pch;
	char *pch1[1];
	int cnt=0;
  
	fgets(cmdline, 1001, stdin);
	//cmdline[1000]=0;
	/*
	pch = strtok (cmdline," \t\n");
	while (pch != NULL)
	{
		strcpy(pch1[cnt], pch);
		printf("%d: %s", cnt, pch1[cnt]);
		pch = strtok (NULL, " \t\n");
		cnt++;
	}
	char *cmdls;
	cmdls = pch1[0];*/
	switch(pid = fork()) {
	case -1:
		perror("fork"); /* something went wrong */
		exit(1); /* parent exits */
	case 0:
		pch = strtok (cmdline," \t\n");
		while (pch != NULL)
		{
			strcpy(pch1[cnt], pch);
			printf("%d: %s", cnt, pch1[cnt]);
			pch = strtok (NULL, " \t\n");
			cnt++;
		}
		char *cmdls;
		cmdls = pch1[0];
	
		if (pch1[1] != NULL){
			printf(" CHILD: My PID is %d\n", getpid());
			printf(" CHILD: unknown command\n");
		}
		printf(" CHILD: My PID is %d\n", getpid());
		execvp(pch1[0], pch1); 
		perror("Error exec!\n");
		exit(1);
	default:
		printf("PARENT: My PID is %d\n", getpid());
		wait(NULL);
		}
}
	return 0;
}
