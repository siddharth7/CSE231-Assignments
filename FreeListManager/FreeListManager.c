#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define delimit " \t\r\n\a"
typedef struct managelist
{
	char *x;
	int size;
	struct managelist *next;
}managelist;
managelist *front,*rear,*temp,*front1;
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
void createnode()
{
	front=rear=NULL;
}
void addnode(int val)
{
    if (rear==NULL)
    {
        rear=(managelist*)malloc(sizeof(managelist));
        rear->next=NULL;
        rear->size=val;
        rear->x=(char*)malloc(val*sizeof(char));
        //printf("size:%d address:%p\n", rear->size,rear->x);
        front=rear;
    }
    else
    {
        temp=(managelist*)malloc(sizeof(managelist));
        rear->next=temp;
		temp->x=(char*)malloc(val*sizeof(char)); 
		temp->size=val;       
		temp->next=NULL;
		//printf("size:%d address:%p\n", temp->size,temp->x);
        rear=temp;
    }
}
void freenode(int val)
{
	if(front == NULL && rear==NULL)
		return;
	int i=1;
	int flag=0;
	temp=front;
	managelist *temp2=temp->next;
	if(val==1)
	{
		//printf("sdd\n");
		if(front->next == NULL)
		{
			//printf("aya\n");
			//temp2 = rear;
			//free(temp2);
			rear = NULL;
		}
		front=temp->next;
		free(temp);
	}
	else
	{
		i = 2;
		//managelist *prev = NULL;
		// if(temp2 == NULL)
		// 	printf("uuuu\n");
		// printf("ttttt\n");
		while(temp2 != NULL && temp2->next != NULL)
		{
			//printf("loop\n");
			if(i==val)
			{
				//printf("%d %d\n",temp2->size,i);
				temp->next=temp2->next;
				free(temp2);
			}
			else
			{
				//prev = temp;
				temp=temp->next;
				temp2=temp2->next;
			}
			i++;
		}

		if(i == val)
		{
			//printf("ytyt\n");
			temp->next = NULL;
			rear=temp;
			free(temp2);
		}
	}
}
void display()
{
	temp=front;
	if(temp == NULL && rear == NULL)
		return;
	while(temp!=rear)
	{
		printf("size:%d address:%p\n", temp->size,temp->x);
		temp=temp->next;
	}
	if(temp==rear)
	{
		printf("size:%d address:%p\n", temp->size,temp->x);
	}
}
int main(int argc, char const *argv[])
{
	createnode();
	char *line;
	char *pch;
	char *line_new[100];
	int i;
	do
	{
		printf(">");
		char *line;
		line=read_cmd_line();
		//printf("%s\n", line );
		i=0;
		int flag=1;
		char arg1[10],arg2[10];
		pch = strtok(line,delimit);
		for(i=0;i<100;i++)
		{
			line_new[i]=malloc(sizeof(char)*40);
		}
		i=0;
		while(pch != NULL)
		{
		    strcpy(line_new[i], pch);
		    pch = strtok (NULL, delimit);
		    i++;
		}
		
		line_new[i] = NULL;
		if(strcmp(line_new[0],"malloc")==0)
		{
			addnode(atoi(line_new[1]));
		}	
		else if(strcmp(line_new[0],"free")==0)
		{
			freenode(atoi(line_new[1]));
		}
		else if(strcmp(line_new[0],"print")==0)
		{
			display();
		}
	}while(1);
	return 0;
}