#include "shell_fct.h"



//global variable for alarmHandler
pid_t *pid;
int pcount;

void sigHandler(int sigNum)
{
    	if(pid != NULL)
  	{
		int i;
        	printf("5 seconds passed,the shell doesn't respond, stop the command\n");
        	
        	for(i = 0; i < pcount; i++){
            		kill(pid[i], SIGKILL);
	
		}	
        	exit(0);
   	}

}

int exec_command(cmd* cmd){

	pcount=cmd->nbCmdMembers;
	pid = NULL;
	pid = (int *)malloc(pcount * sizeof(int));

	if (cmd->nbCmdMembers == 0)
        	return 0;

	//<<------------------pipefd----------------------->>
	//creat two array declared in the structure cmd 

	/* initialize */
	cmd->infd = (int *)malloc(10*sizeof(int));
	cmd->outfd = (int *)malloc(10*sizeof(int));

	for (int i=0; i<10; ++i)
    	{
       		cmd->infd[i] = 0;
        	cmd->outfd[i] = 1;
   	}
	
	// redirect the STDIN or STDOUT if a redirection exists
	for(int i=0;i<cmd->nbCmdMembers;i++){
		if (strcmp(cmd->redirection[i][0],"")!=0){                         
			cmd->infd[0] = open(cmd->redirection[i][0], O_RDONLY);
		}
    		else if(strcmp(cmd->redirection[i][1],"")!=0)
    			{

        		if(cmd->redirectionType[i][1]==1){
            			cmd->outfd[cmd->nbCmdMembers-1] = open(cmd->redirection[i][1], O_WRONLY | O_CREAT | O_APPEND, 0666); 
			}
        		else if(cmd->redirectionType[i][1]==2){
            			cmd->outfd[cmd->nbCmdMembers-1] = open(cmd->redirection[i][1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
			}

    		}
	}
	


	int fd;
   	int fds[2];
    	for (int i=0; i<cmd->nbCmdMembers; ++i)    
    	{
        	// CREAT PIPE    if it is not the last member of the command,then creat a pipe
        	if (i<cmd->nbCmdMembers-1)
        	{
            		pipe(fds);
            		cmd->outfd[i] = fds[1];         
            		cmd->infd[i+1] = fds[0];
        	}

   		pid[i] = fork();

 		if (pid[i] == 0)
    		{
        		if (cmd->infd[i] != 0)            
        		{
            			close(0);
            			dup(cmd->infd[i]);      
        		}
        		if (cmd->outfd[i] != 1)
        		{
            			close(1);
            			dup(cmd->outfd[i]);
        		}

        		int j;
        		for (j=3; j<1024; ++j)
            			close(j);

        	execvp(cmd->cmdMembersArgs[i][0],cmd->cmdMembersArgs[i]);    //execvp    Execute the comand
    		}

        if ((fd = cmd->infd[i]) != 0)
            close(fd);

        if ((fd = cmd->outfd[i]) != 1)
            close(fd);
	

    }
	//watchdog
    	signal(SIGALRM, sigHandler); 
    	alarm(5);

        //Wait the last member of the command to finish
        int cnt = 0;
        while (wait(NULL) != -1 && cnt != cmd->nbCmdMembers) {
            cnt++;
        }
	

    return 0; 
}


//command cd
void mycd(cmd *cmd){ 
 
    	struct passwd *pwd;	//Used to get the parameter pw_dir
   	char pathname[100]; 
    	pwd=getpwuid(getuid()); 
 
    	if(cmd->nbMembersArgs[0]==1){           //If only one cd 
        	strcpy(pathname,pwd->pw_dir);       //Get pathname 
        	if(chdir(pathname)==-1){
            		perror("myshell: chdir");
            	exit(1);  
        	}  
    	}  
    	else{		//if there is a path  
        	if(chdir(cmd->cmdMembersArgs[0][1])==-1){
            		printf("myshell: cd: %s :No such file or directory\n",cmd->cmdMembersArgs[0][1]);	//print error message         
        	}     
   	}  

}  
