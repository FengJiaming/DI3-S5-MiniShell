#include "cmd.h"


//Prints the command
void printCmd(cmd *cmd){
	
	//print initCmd
	printf("initCmd='%s'\n",cmd->initCmd);
	
	//print nbCmdmembers
	printf("nbCmdmembers=%d\n",cmd->nbCmdMembers);
	
	//printf each cmd_members
	for(int i=0;i<cmd->nbCmdMembers;i++){
		if(strcmp(cmd->cmdMembers[i],"")!=0)
			printf("cmd_members[%d]=%s\n",i,cmd->cmdMembers[i]);
	}
	
	//print nbMemberArgs     the number of each member of the command 
	for(int i=0;i<cmd->nbCmdMembers;i++){
		if(cmd->nbMembersArgs[i]!=0)
			printf("nbMemberArgs[%d]=%d\n",i,cmd->nbMembersArgs[i]);	
	}
		
	//print Cmdmemberargs     evert args of each member of the command
	for(int i=0;i<cmd->nbCmdMembers;i++){	
		if(strcmp(cmd->cmdMembers[i],"")!=0)
			for(int j=0;j<cmd->nbMembersArgs[i]+1;j++){
				if(cmd->cmdMembersArgs[i][j]==NULL){
					printf("Cmdmember_arg[%d][%d]=NULL\n",i,j);
					break;
				}
				printf("Cmdmember_arg[%d][%d]=%s\n",i,j,cmd->cmdMembersArgs[i][j]);
				
		}
	}
	
	//print redirection   
	for(int i=0;i<cmd->nbCmdMembers;i++){
		
		//print the redirection of each member of the command if redirection exists
		if(strcmp(cmd->redirection[i][0],"")!=0){
			printf("redirection[%d][STDIN]=%s\n",i,cmd->redirection[i][0]);
		}else{
			printf("redirection[%d][STDIN]=NULL\n",i);
		}

		if(strcmp(cmd->redirection[i][1],"")!=0){
			printf("redirection[%d][STDOUT]=%s\n",i,cmd->redirection[i][1]);
		}else{
			printf("redirection[%d][STDOUT]=NULL\n",i);
		}

		if(strcmp(cmd->redirection[i][2],"")!=0){
			printf("redirection[%d][STDERR]=%s\n",i,cmd->redirection[i][2]);
		}else{
			printf("redirection[%d][STDERR]=NULL\n",i);
		}

	}

	//print redirectiontype     APPEND vs  OVERRIDE
	for(int i=0;i<cmd->nbCmdMembers;i++){
		if(cmd->redirectionType[i][1]==1){
			printf("redirection_type[%d][STDOUT]=APPEND\n",i);
		}else if(cmd->redirectionType[i][1]==2){
			printf("redirection_type[%d][STDOUT]=OVERRIDE\n",i);
		}else if(cmd->redirectionType[i][1]==0){
			printf("redirection_type[%d][STDOUT]=NULL\n",i);
		}
	}
	
}



//Initializes the initial_cmd, membres_cmd et nb_membres fields
void parseMembers(char *inputString,cmd *cmd){
	
	//<<------initcmd-------->>
	
	cmd->initCmd = NULL;
	cmd->initCmd=(char*)malloc(strlen(inputString)+1);

	strcpy(cmd->initCmd,inputString);
	

	//<<------nbCmdmembers------>>
	char *p = inputString;  
    	
	char c='|';  
    	int num = 0; 

    	while (*p != '\0'){  
        if (*p == c){  
            num++;  
        }  
        p++;  
   	}  
    	num = num + 1;  
    	
  	cmd->nbCmdMembers=num;
	
	//<<-----------cmdMembers------------->>
	
	/* initialize */
	cmd->cmdMembers = (char **)malloc((cmd->nbCmdMembers) * sizeof(char *));
	for (int i = 0; i < cmd->nbCmdMembers; i++)  
    	{  
        	cmd->cmdMembers[i] = (char *)malloc(100);  
        	memset(cmd->cmdMembers[i],0,100);  
    	}  
	

	char *l = inputString;  
    	char *q = inputString;  
   	
    	for (int i = 0; i < cmd->nbCmdMembers; i++){  
        	while (*q != '\0' && *q != c){ 	   //search "|" or "\0"  
            		q++;  
        	}  
        	char *part = (char *)malloc(100 * sizeof(char));
		 
        	int length = q - l;
		
        	memcpy(part, l, length);	//copy the string with length  
        	part[length] = '\0';  
  
        	l=q; 
		l++;
       	 	q++;

		cmd->cmdMembers[i] = part;

		if(cmd->cmdMembers[i][0] == ' '){     //remove the left space
			int j = 1;
			while(cmd->cmdMembers[i][j]!='\0'){
				cmd->cmdMembers[i][j-1] = cmd->cmdMembers[i][j];
				j++;
			}
			cmd->cmdMembers[i][j-1] = '\0';
		}
	}


	//<<----------------cmdMembersarg,nbMemberargs,redirection and redirectionType------------------>>

	
	// initialize cmdMembersArgs
	cmd->cmdMembersArgs = (char ***) malloc(cmd->nbCmdMembers * sizeof(char ** ));
    		
	for (int i = 0; i < cmd->nbCmdMembers; ++i){
		cmd->cmdMembersArgs[i] = NULL;
		cmd->cmdMembersArgs[i] = (char **) malloc(10 * sizeof(char * ));
			for(int j = 0; j < 10; ++j){
    				cmd->cmdMembersArgs[i][j] = (char * )malloc(100 /* * sizeof(char )*/);
				memset(cmd->cmdMembersArgs[i][j],0,100);
		}
	}

	// initialize nbMembersArgs
	cmd->nbMembersArgs = (unsigned int *)malloc(10*sizeof(int));
	for (int i=0; i<10; ++i)
    	{
       		cmd->nbMembersArgs[i]=0;
   	 }

	// initialize redirection
	cmd->redirection = (char ***) malloc(10 * sizeof(char ** ));
    		
	for (int i = 0; i < 10; ++i){
		cmd->redirection[i] = (char **) malloc(3 * sizeof(char * ));
		for(int j = 0; j < 3; ++j){
    			cmd->redirection[i][j] = (char * )malloc(100);
			memset(cmd->redirection[i][j],0,100);
		}
	}

	// initialize redirectionType
	cmd->redirectionType=(int**)malloc(10*sizeof(int*));
	for (int i = 0; i < 10; i++)  
    	{  
        	cmd->redirectionType[i] = (int *)malloc(2 * sizeof(int));  
        	memset(cmd->redirectionType[i],0,2 * sizeof(int));  
    	}  


	//parse each cmdmember to get each member's arguments,number of the arguments ,redirection and type of the redirection
 
	for (int i=0;i<cmd->nbCmdMembers;i++){       
		char *temp = strdup(cmd->cmdMembers[i]);

		char *substr = strtok(temp," ");  //use strtok to seperate every members ,substr is is the first substring
 		int j=0;
       		while (substr != NULL) {    
                	strcpy(cmd->cmdMembersArgs[i][j],substr); //Copy the newly divided substring substr into the cmdMembersArgs[i][j] to be stored
			j++;  			    
                	substr = strtok(NULL," ");
        	}
		cmd->cmdMembersArgs[i][j]=NULL;
		
		
		int count=0;    //count is used to count the number of arguments

		//now,deal with the redirection and redirectiontype
		
		for(int k=j-1;k>=0;k--){    //At this moment,variable j is the number of arguments

			//------------------------stdin---------------
		   	if(strcmp(cmd->cmdMembersArgs[i][k],"<")==0){
				cmd->redirection[i][0]=cmd->cmdMembersArgs[i][k+1];
				cmd->cmdMembersArgs[i][k]=NULL;
				cmd->cmdMembersArgs[i][k+1]=NULL;

			}

			//------------------------stdout---------------
			else if(strcmp(cmd->cmdMembersArgs[i][k],">>")==0){
				cmd->redirection[i][1]=cmd->cmdMembersArgs[i][k+1];
				cmd->cmdMembersArgs[i][k]=NULL;
				cmd->cmdMembersArgs[i][k+1]=NULL;
				cmd->redirectionType[i][1]=1;         //redirectionType=APPEND
	
			}else if(strcmp(cmd->cmdMembersArgs[i][k],">")==0){
				cmd->redirection[i][1]=cmd->cmdMembersArgs[i][k+1];
				cmd->cmdMembersArgs[i][k]=NULL;
				cmd->cmdMembersArgs[i][k+1]=NULL;
				cmd->redirectionType[i][1]=2;       //redirectionType=OVERRIDE
			}
		
		}
		//<<--------------------nbMembersArgs-------------------->>
		for(int k=0;k<j;k++){	
			if(cmd->cmdMembersArgs[i][k]!=NULL){
				count++;
			}
		}
		cmd->nbMembersArgs[i]=count;

	}

	
		
	//parse command finish
}

//Frees memory associated to a cmd
void freeCmd(cmd *cmd){
  	
	//<<------------------freeinitcmd-------------------->>
	free(cmd->initCmd);


	//<<------------------freecmdMembers----------------->>
	for (int i = 0; i < cmd->nbCmdMembers; i++)  
    	{  
        	free(cmd->cmdMembers[i]); 
    	}  
	free(cmd->cmdMembers);
	cmd->cmdMembers = NULL;



	//<<------------------freecmdMembersArgs----------------->>
	for(int i=0;i < cmd->nbCmdMembers; i++)
    	{
		for(int j = 0; j < 10; ++j){
			free(cmd->cmdMembersArgs[i][j]);
		}
		free(cmd->cmdMembersArgs[i]);
    	}
	free(cmd->cmdMembersArgs);
	cmd->cmdMembersArgs = NULL;
	free(cmd->nbMembersArgs);


	//<<------------------freeredirection----------------->>
  	
    	for(int i=0;i < cmd->nbCmdMembers; i++)
    	{
		for(int j = 0; j < 3; ++j){
			free(cmd->redirection[i][j]);
		}
		free(cmd->redirection[i]);
		free(cmd->redirectionType[i]);
    	}
    	free(cmd->redirection);
    	free(cmd->redirectionType);
    	cmd->redirection = NULL;
    	cmd->redirectionType = NULL;




	
}
