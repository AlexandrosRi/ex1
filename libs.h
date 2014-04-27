#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXC 1001

int getMeANumber(void);

char *commands12[];

//gcc -o progname prog1.c prog2.c 
