/* ---------------------------------------------------------------------------
** Operating Systems Y02S03 Nov 2015 
** Assignment 5 File System Implementation
** 
** Author: Varun Jain 14170, Siddharth Singh 14105
** -------------------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>
# include <dirent.h> 
# include <string.h> 

int a, nbytes = 262144;
char *nblock;

void createBitMaps(int disk)
{
	char diskNumStr[6];
	sprintf(diskNumStr, "disk%d", disk);
	char line[5000];
	FILE *file;
	file = fopen(diskNumStr, "rb");
	int nolines=0;

	char inodeData[(((nbytes/4096)-3)/2)];

	while (fgets(line,5000, file)!=NULL)
	{
		if(nolines >= (((nbytes/4096)-3)/2)+3)
		{
			if (strlen(line) == 1)
			{
				inodeData[nolines - ((((nbytes/4096)-3)/2)+3)] = '0';
			}
			else
			{
				inodeData[nolines - ((((nbytes/4096)-3)/2)+3)] = '1';
			}
		}
		nolines++;
	}
	fclose(file);
	writeData(disk, -((((nbytes/4096)-3)/2)+3)+1, inodeData);
	writeData(disk, -((((nbytes/4096)-3)/2)+3)+2, inodeData);
	return 0;
}

int createSFS(char *filename, int nbytes)
{
	DIR *d;
	struct dirent *dir;
	d = opendir(".");
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (strcmp(dir->d_name, filename) == 0)
			{
				FILE *fp;
				char *eachEle;
				char *line = malloc(4096);
				fp = fopen(filename, "r+");
				while (fgets(line, 4096, fp) != NULL)  
				{
					eachEle = strtok(line, " ");
					while (eachEle != NULL) 
					{
						return atoi(eachEle);
					}
				}
				free(line);  
				fclose(fp);
			}
		}
		closedir(d);
	}
	FILE *fp;
	fp = fopen(filename, "w+");
	char *buffer;
	a=((nbytes/4096)-3)/2;
	a = a * 4096;
	fprintf(fp,"27 %d 1024 ", nbytes);
	fprintf(fp,"1024 %d %d\n", a,a);
	int i;
	for(i=1;i<(nbytes/(4*1024));i++)
	{
		fprintf(fp,"\n");
	}
	fclose(fp);
	printf("File System Created\n");
	return 27;
}

int writeData(int disk, int blockNum, void *block)
{
	char *newblock=(char *)block;
	if(strlen(newblock)>4096)
	{
		printf("Data too big!\n");
		return 0;
	}
	int chk = 0;
	char diskNumStr[6];
	sprintf(diskNumStr, "disk%d", disk);
	DIR *d;
	struct dirent *dir;
	d = opendir(".");
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (strcmp(dir->d_name, diskNumStr) == 0)
			{
				chk = 1;
			}
		}
		closedir(d);
	}
	if (chk == 1)
	{
		FILE *file,*fp;
		file=fopen(diskNumStr,"rb");
		fp=fopen("disktemp","wb");
		char line[5000];
		char read;
		int nolines=0;
		while (fgets(line,5000, file)!=NULL)
    	{
			if((nolines==((((nbytes/4096)-3)/2)+3 + blockNum)) )
			{
				size_t len = strlen(newblock);
    			char *temp = malloc(len + 2);
    			strcpy(temp, newblock);
    			temp[len] = '\n';
    			temp[len+1] = '\0';
				fprintf(fp, temp);
				printf("Written at: %d\n",blockNum);
			}
			else
			{
				fprintf(fp,line);
			}
			nolines++;
    	}
    	fclose(fp);
		fclose(file);
		remove(diskNumStr);
		rename("disktemp",diskNumStr);
	}
	else
	{
		printf("No file found\n");
	}
	return 1;
}

int readData(int disk, int blockNum, void *block)
{
	int chk = 0;
	char line[5000];
	char diskNumStr[6];
	sprintf(diskNumStr, "disk%d", disk);
	DIR *d;
	struct dirent *dir;
	d = opendir(".");
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (strcmp(dir->d_name, diskNumStr) == 0)
			{
				chk = 1;
			}
		}
		closedir(d);
	}
	if(chk==1)
	{
		FILE *file,*fp;
		file=fopen(diskNumStr, "rb");
		int nolines=0;
		while (fgets(line,5000, file)!=NULL)
    	{
			if(nolines == (((nbytes/4096)-3)/2)+3 +blockNum)
			{
				
				nblock = line;
				return strlen(line)-1;
			}
			nolines++;
    	}
	}
	else
	{
		printf("No file found\n");
	}
	return 0;
}

int writeFile(int disk, char *fileName, void *block)
{
	char diskNumStr[6];
	sprintf(diskNumStr, "disk%d", disk);
	char *newblock=(char *)block;
	FILE *file;
	file = fopen(diskNumStr,"rb");
	char line[5000];
	int nolines = 0;

	if(strlen(newblock) < 4095)
	{
		while (fgets(line,5000, file)!=NULL)
		{
			if(((strlen(line) == 2) || (strlen(line) == 1)) && (nolines >= (((nbytes/4096)-3)/2)+3)  )									
			{
				fclose(file);

				size_t len = strlen(newblock);
    			char temp[len + 8];
    			sprintf(temp, "%s %d %d", fileName, strlen(newblock), nolines - ((((nbytes/4096)-3)/2)+3));
				writeData(disk, nolines - 2*((((nbytes/4096)-3)/2)+3)+3, temp);
				writeData(disk, nolines - ((((nbytes/4096)-3)/2)+3), newblock);
				createBitMaps(disk);
				break;			
			}
			nolines++;
		}
	}
	else
	{
		char div1[4094];
		div1[0] = newblock[0];
		int i;
		for(i=1; i<=strlen(newblock); i++)
		{
			if(i % 4094 == 0)
			{
				fclose(file);
				writeFile(disk, fileName, div1);
				memset(div1, 0, sizeof(div1));
				div1[i%4094]=newblock[i];
			}
			else
			{
				div1[i%4094]=newblock[i];
			}
		}
		if (strlen(div1) > 0)
		{
			fclose(file);
			writeFile(disk, fileName, div1);
		}
	}
	
	return 1;
}

int readFile(int disk, char* filename, void* block)
{
	char diskNumStr[6];
	sprintf(diskNumStr, "disk%d", disk);
	char *newblock=(char *)block;
	FILE *file;
	file = fopen(diskNumStr,"rb");
	char line[5000];
	int flag=0;
	int nolines = 0;
	while (fgets(line,5000, file)!=NULL)
	{
		char ar[100],ar2[10],ar3[100];
		int countr=0;
		if(strlen(line)>1)
		{
			while(line[countr]!=' ')
			{
				ar[countr]=line[countr];
				countr++;
			}
			ar[countr]='\0';
			countr=0;
			int spaces=0;
			int pos=0;
			while(line[countr]!='\0')
			{
				if(spaces==2)
				{
					ar2[pos]=line[countr];
					pos++;
				}
				if(line[countr]==' ')
				{
					spaces++;
				}				
				countr++;
			}
			ar2[pos]='\0';
			if(strcmp(filename,ar)==0)
			{
				if(flag == 0)
				{
					strcpy(ar3,ar);
					flag=1;
					readData(disk,atoi(ar2), &nblock);
				}
				else
				{
					if(strcmp(ar,ar3)==0)
					{
						readData(disk,atoi(ar2), &nblock);
					}
					else
					{
						return 0;
					}
				}
				
			}
			strcpy(ar3,ar);
		}

	}
		return 0;
}

void print_inodeBitmaps(int fileSystemId)
{
	readData(0, -((((nbytes/4096)-3)/2)+3)+1 ,&nblock);
	printf("Inode Bit Map: %s", nblock);
}

void print_dataBitmaps(int fileSystemId)
{
	readData(0, -((((nbytes/4096)-3)/2)+3)+2 ,&nblock);
	printf("Data Bit Map : %s", nblock);
}

int main()
{
	// int fileSysId = createSFS("disk0", nbytes);			// init file system on disk0 of ntfs: 27, size = 262144B
	// printf("File System is: %d\n", fileSysId);

	// int val2 = writeData(0, -31,"a");

	// int val = readData(0,-30,&nblock);
	// printf("Block Size: %d\n",val);
	// printf("Block Read: %s", nblock);
	
	// writeFile(0, "file1", "abc");


	int val2 = readFile(0, "file2", &nblock);
	printf("Read Data: %s", nblock);
	
	// print_inodeBitmaps(createSFS("disk0", nbytes));
	// print_dataBitmaps(createSFS("disk0", nbytes));

	return 0;
}

