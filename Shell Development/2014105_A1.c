#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define delimit " \t\r\n\a"
void clearsc()
{
  const char* CLEAR_SCREE_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO,CLEAR_SCREE_ANSI,12);
}
void execute_pipes(char *line)
{
	int pipe_fd[2],ppipe_r,status;
	pid_t lpid;
	int ini, ter;
	int i;
	//printf("%s\n", line);
	char *newline[100];
	char *array_arg[100][100];
		//printf("input hai ye %s\n",cp);
		for (i = 0;i<100; i++) {
			newline[i]=strtok(line, delimit);
			 if (newline[i]==NULL)
			 	break;
            line=NULL;
            //printf("%s\n",newline[i]);
        }
        int x=1,y=0;
        for(i=0;newline[i]!=NULL;i++)
        {
        	//printf("%s\n",newline[i]);
        	if(strcmp(&newline[i][0],"|")==0)
        	{
        		array_arg[x-1][y]=NULL;
        		y=0;
        		x++;
        		//printf("sdfsdfsf\n");
        	}
        	else
        	{
        		array_arg[x-1][y]=newline[i];
        		//printf("%d %d %s\n",x-1,y, array_arg[x-1][y]);
        		y++;
        		
        	}

        }
        for(i=0;i<x;i++)
        {
        	ini = (i == 0);
            ter = (i ==x- 1);
        	if(!ter)
        	{
        		pipe(pipe_fd);
        	}
        	pid_t pid=fork();
        	if(ter)
        	{
        		lpid=pid;
        	}
        	if(pid==0)
        	{
        		if(ini)
        		{
        				dup2(pipe_fd[1], 1); 
                        close(pipe_fd[1]); 
                        close(pipe_fd[0]);
                } 
                else if(ter) 
                {
                        dup2(pipe_fd[0], 0);
                        close(pipe_fd[0]);
                } 
                else 
                { 
                        dup2(ppipe_r, 0); 
                        close(ppipe_r);
                        dup2(pipe_fd[1], 1); 
                        close(pipe_fd[1]);
                }
                execvp(array_arg[i][0], array_arg[i]);

        	}
        	else 
        	{ 
                    if(ini) 
                    {
                        close(pipe_fd[1]);
                        ppipe_r = pipe_fd[0];
                    } 
                    else if(ter) 
                    {
                        close(pipe_fd[0]);
                        while(wait(&status)!=lpid) 
                        {

                        } 
                        fflush(stdout);
                    } 
                    else 
                    { 
                        close(pipe_fd[1]);
                        close(ppipe_r);
                        ppipe_r = pipe_fd[0];
                    }
        	}

		}

}
char *read_cmd_line()
{
	char *input_line=malloc(sizeof(char)*1000);
	int character;
	int pointer=0;
	for(;;)
	{
		character=getchar();
		if(character==EOF || character=='\n')
		{
			input_line[pointer]='\0';
			//break;
			return input_line;
		}
		else
		{
			input_line[pointer]=character;
		}
		pointer++;
	}

}
void sighandle(int num)
{
	char cwd[1000];
	fprintf(stderr, "\n");
	if (getcwd(cwd, sizeof(cwd)) != NULL)
       			{
       				fprintf(stderr, "%s >", cwd);
       			}
       			//printf("ssdsdsfs\n");
	return;
}
int main()
{
	//char *line;
	char *line;
	char linec[500];
	int i;
	int flag=1;
	char basedir[1000];
	char *dirr;
	signal(SIGINT,sighandle);
	do{
			char cwd[1000];
   			if (getcwd(cwd, sizeof(cwd)) != NULL)
       			{
       				fprintf(stdout, "%s >", cwd);
       				if(flag==1)
       					{
       						strcpy(basedir,cwd);
       						flag=0;
       						dirr=basedir;
       						strcat(basedir,"/hist.txt");
       						//printf("%s\n",basedir);
       					}
       			}
   			else
       			perror("error");
       		line = read_cmd_line();
       		if(strlen(line)==0)
       		{

       		}
       		else
       		{
       		FILE *fp = fopen(dirr,"a");
       		fprintf(fp, "%s\n", line);
       		fclose(fp);
       		//linec=line;
       		strcpy(linec,line);
			char *pch;
			char *line_new[100];
			int pointer2=0;
			int number_of_pipes=0;
			int number_of_out=0;
			int number_of_in=0;
			pch = strtok(line,delimit);
			for(i=0;i<100;i++)
			{
				line_new[i]=malloc(sizeof(char)*40);
			}
			i=0;
			int len;
			while(pch != NULL)
			{
			    strcpy(line_new[i], pch);
			    //printf ("%s\n",line_new[i]);
			    if(strcmp(line_new[i],"|")==0)
			    	number_of_pipes++;
			    else if(strcmp(line_new[i],">")==0)
			    	number_of_in++;
			    else if(strcmp(line_new[i],"<")==0)
			    	number_of_out++;
			    pch = strtok (NULL, delimit);
			    i++;
			}
			len = i;
			line_new[i] = NULL;
			if(number_of_pipes)
			{
				//printf("pipes detected ;)\n");
				execute_pipes(linec);
			}
			else if(number_of_in && number_of_out)
			{
				FILE *fp;
				int status1;
				char *newline[100];
				char *array_arg[100][100];
					//printf("input hai ye %s\n",cp);
				char *line=linec;
					for (i = 0;i<100; i++) {
						newline[i]=strtok(line, delimit);
						 if (newline[i]==NULL)
						 	break;
			            line=NULL;
			            //printf("%s\n",newline[i]);
			        }
			        int x=1,y=0;
			        for(i=0;newline[i]!=NULL;i++)
			        {
			        	//printf("%s\n",newline[i]);
			        	if(strcmp(&newline[i][0],">")==0 || strcmp(&newline[i][0],"<")==0)
			        	{
			        		array_arg[x-1][y]=NULL;
			        		y=0;
			        		x++;
			        		//printf("sdfsdfsf\n");
			        	}
			        	else
			        	{
			        		array_arg[x-1][y]=newline[i];
			        		//printf("%d %d %s\n",x-1,y, array_arg[x-1][y]);
			        		y++;
			        		
			        	}

        			}
    
        			
			}
			else if(number_of_in)
			{
				FILE *fp;
				int status1;
				char *newline[100];
				char *array_arg[100][100];
					//printf("input hai ye %s\n",cp);
				char *line=linec;
					for (i = 0;i<100; i++) {
						newline[i]=strtok(line, delimit);
						 if (newline[i]==NULL)
						 	break;
			            line=NULL;
			            //printf("%s\n",newline[i]);
			        }
			        int x=1,y=0;
			        for(i=0;newline[i]!=NULL;i++)
			        {
			        	//printf("%s\n",newline[i]);
			        	if(strcmp(&newline[i][0],">")==0)
			        	{
			        		array_arg[x-1][y]=NULL;
			        		y=0;
			        		x++;
			        		//printf("sdfsdfsf\n");
			        	}
			        	else
			        	{
			        		array_arg[x-1][y]=newline[i];
			        		//printf("%d %d %s\n",x-1,y, array_arg[x-1][y]);
			        		y++;
			        		
			        	}

        			}
        			pid_t pid;
        			pid=fork();
        			if(pid==0)
        			{
        				fp = fopen(array_arg[1][0], "a");
						dup2(fileno(fp), 1);
						execvp(array_arg[0][0], array_arg[0]);
						break;
					}
					else
						waitpid(pid, &status1, 0);

			}
			else if(number_of_out)
			{
				FILE *fp;
				int status1;
				char *newline[100];
				char *array_arg[100][100];
					//printf("input hai ye %s\n",cp);
				char *line=linec;
					for (i = 0;i<100; i++) {
						newline[i]=strtok(line, delimit);
						 if (newline[i]==NULL)
						 	break;
			            line=NULL;
			            //printf("%s\n",newline[i]);
			        }
			        int x=1,y=0;
			        for(i=0;newline[i]!=NULL;i++)
			        {
			        	//printf("%s\n",newline[i]);
			        	if(strcmp(&newline[i][0],"<")==0)
			        	{
			        		array_arg[x-1][y]=NULL;
			        		y=0;
			        		x++;
			        		//printf("sdfsdfsf\n");
			        	}
			        	else
			        	{
			        		array_arg[x-1][y]=newline[i];
			        		//printf("%d %d %s\n",x-1,y, array_arg[x-1][y]);
			        		y++;
			        		
			        	}

        			}
        			pid_t pid;
        			pid=fork();
        			if(pid==0)
        			{
						fp = fopen(array_arg[1][0], "r");
						dup2(fileno(fp), 0);
						//printf("sdfsfsfd\n");
						execvp(array_arg[0][0], array_arg[0]);
						break;
					}
					else
						waitpid(pid, &status1, 0);

			}
			else
			{
				if(strcmp(line_new[0],"cd")==0)
				{
					//printf("transfering\n");
			
						if(line_new[1]==NULL)	
						{
							//printf("Home Directory\n");
							chdir(basedir);

						}
						else
						{
							chdir(line_new[1]);
						}
					
		
				}
				else if(strcmp(line_new[0],"clear")==0)
				{
					
					pid_t pid;
					pid = fork();
					if(pid==0)
					{
						clearsc();	
					}
					else if (pid < 0)
					{                  
	    				printf( "Error clearing screen!\n");    
	    				exit(1);
					}	
					else
					{                       
	 	    			pid = waitpid(pid, NULL, 0);   
					}

				}
				else if(strcmp(line_new[0],"ls")==0)
				{
					pid_t pid;
					pid = fork();
					if(pid==0)
					{
						i = 0;
						while(i < len)
						{
							line_new[i] = strdup(line_new[i]);
							i++;
						}
						execvp(line_new[0],line_new);
					}
					else if (pid < 0)
					{                  
	    				printf( "Error performing ls!\n");    
	    				exit(1);
					}	
					else
					{                       
	 	    			pid = waitpid(pid, NULL, 0);   
					}				
				}
				else if(strcmp(line_new[0],"exit")==0)
				{
					_exit(0);
				}
				else if(strcmp(line_new[0],"history")==0)
				{
						FILE *fp1 = fopen(dirr,"r");
						if(fp1!=NULL)
						{
							char gline[128];
							while(fgets(gline,sizeof gline,fp1)!=NULL)
      						{
         						fputs(gline,stdout); 
      						}
      						fclose(fp1);
					
					}
       			}
       			else if(strcmp(line_new[0],"help")==0)
       			{
       				printf("--------------Shell developed by Siddharth Singh(2014105)---------\n");
       				printf("--------------Functionalities are listed below--------------------\n");
       				printf("--------------cd--------------------------------------------------\n");
       				printf("--------------clear-----------------------------------------------\n");
       				printf("--------------exit------------------------------------------------\n");
       				printf("--------------multiple piping-------------------------------------\n");
       				printf("--------------help------------------------------------------------\n");
       				printf("--------------redirections(one at a time)-------------------------\n");

       			}
				else
				{
					
					if(strlen(line)!=0)
						{
							pid_t pid;
							pid = fork();
							if(pid==0)
							{
								i = 0;
								while(i < len)
								{
									line_new[i] = strdup(line_new[i]);
									i++;
								}
								execvp(line_new[0],line_new);
							}
							else if (pid < 0)
							{                  
			    				printf( "Error performing!\n");    
			    				exit(1);
							}	
							else
							{                       
			 	    			pid = waitpid(pid, NULL, 0);   
							}	
						}
					}	
			}


       }
		}while(1);
	
   return 0;
}