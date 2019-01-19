#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "shell_fct.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>


//<--------historique des commands------>

void history_setup()
{
	using_history();
	stifle_history(50);
	read_history("/tmp/msh_history");	
}


void history_finish()
{
	append_history(history_length, "/tmp/msh_history");
	history_truncate_file("/tmp/msh_history", history_max_entries);
}



//<---------main-------->>

int main(int argc, char** argv)
{
	//Auxiliary variables
	int ret = MYSHELL_CMD_OK;
	char* readlineptr;
	struct passwd* infos;
	cmd  command;
	char str[1024];
	char hostname[256];
	char workingdirectory[256];

	//<-------------myshell start-------------->>
	history_setup();

	while(ret != MYSHELL_FCT_EXIT)
	{
		//Get your session info
        	infos=getpwuid(getuid());
		gethostname(hostname, 256);
		getcwd(workingdirectory, 256);
        	//Print it to the console
		sprintf(str, "\n{myshell}%s@%s:%s$ ", infos->pw_name, hostname, workingdirectory);

		readlineptr = readline(str);
		
		add_history(readlineptr);
		
		//Parse the comand
		parseMembers(readlineptr,&command);
		printCmd(&command);
		

		//Execute the comand
		//exit 
		if(strcmp(command.cmdMembers[0], "exit") == 0)  
            	{
                	exit(MYSHELL_FCT_EXIT);
            	}

		//cd   change directory
		if(strcmp(command.cmdMembersArgs[0][0],"cd")==0){   
        		mycd(&command);  
		}

		pid_t mpid;
		int status;
 		mpid = fork();            //fork a child to exec command
          	if(mpid==0){
                	int res;

                	res = exec_command(&command);
                	exit(res);
            	}
            	wait(&status);

		//Clean the house
		freeCmd(&command);

        	printf("testy");
        	history_finish();
   
        	free(readlineptr);
		readlineptr = NULL;
	}

	return 0;
}

