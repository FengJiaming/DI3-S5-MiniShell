#include "cmd.h"

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <pwd.h> 

//Your imports come here

//Terminate shell
#define MYSHELL_FCT_EXIT 1

//Execute a command
int exec_command(cmd *c);

//Execute command cd
void mycd(cmd *cmd);
