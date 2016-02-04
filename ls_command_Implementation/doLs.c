/*RACHITA SOWLE-201505613*/
#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <inttypes.h>
#include <sys/time.h>
#include "doLs.h"
#include "utility.h"
int doDirLs(char** dirArr, int num, char* path, int opt[6])
{
	int i=0;
	for(i=0; i<num; i++)
	{	
		struct dirent **namelist;
		char* fullPath = malloc(255);
		strcpy(fullPath,"");
		strcat(fullPath, dirArr[i]);
		if(opt[1])
		{

			strcpy(fullPath, path);
			if(strcmp(path,"./")!=0) 
			{
				strcat(fullPath,"/");
				strcat(fullPath, dirArr[i]);
			}
			else strcpy(fullPath, dirArr[i]);
		}
		int n = scandir(fullPath, &namelist, 0, alphasort);
		if(n<0)
			return -1;
		char* fileArr[n];
		int j=0;
		for(j=0; j<n; j++)
		{	
			fileArr[j] = malloc(255);
			fileArr[j] = namelist[j]->d_name;
		}
		
		printf("%s:\n",fullPath);
		if(doFileLs(fileArr, j, fullPath, opt)==-1)
			return -1;
	}
	return 1;
}

int doFileLs(char** fileArr, int num, char* path, int opt[6])
{
	//SORTING t and S
	if(opt[2])
		sortStringArr(fileArr,num,'S', path);
	else if(opt[4])
		sortStringArr(fileArr,num,'t', path);
	int i=0,d=0;
	char* dirArr[num];
	//FOR EACH FILE PRINT THE INFO
	for(i=0; i<num; i++)
	{
		int b=0,g=0; //COLORS
		if((!strcmp(fileArr[i],".") || !strcmp(fileArr[i],"..")) && !opt[5])
			continue;

		struct stat buffer;
		char* fullPath = malloc(255);
		strcpy(fullPath, path);
		strcat(fullPath, "/");
		strcat(fullPath, fileArr[i]);
		if(access(fullPath,F_OK) == -1)
		{
			printf("ls: cannot access %s: No such file or directory\n",fullPath);
			return -1;
		}
		if(lstat(fullPath,&buffer)<0)
			return -1;
		if(access(fullPath, X_OK) == 0) g=1;
		if(S_ISDIR(buffer.st_mode)) b=1;
		//IF LONG LISTING IS REQUIRED
		if(opt[0])
		{
			//INODE
			char* lname='\0';
			if(opt[3]) printf("%7ju ",(uintmax_t) buffer.st_ino);
			//FILE TYPE
			if(S_ISDIR(buffer.st_mode)) printf("d");
			else if(S_ISLNK(buffer.st_mode))
			{	
				printf("l");
				b=1;
				lname = malloc(buffer.st_size + 1);
				if (lname == NULL)  fprintf(stderr, "insufficient memory\n");
				int r = readlink(fullPath, lname, buffer.st_size+1);
				lname[r] = '\0';
			}
			else printf("-");
			//PRMISSIONS
			(buffer.st_mode & S_IRUSR) ? printf("r") : printf("-");
			(buffer.st_mode & S_IWUSR) ? printf("w") : printf("-");
			(buffer.st_mode & S_IXUSR) ? printf("x") : printf("-");
			(buffer.st_mode & S_IRGRP) ? printf("r") : printf("-");
			(buffer.st_mode & S_IWGRP) ? printf("w") : printf("-");
			(buffer.st_mode & S_IXGRP) ? printf("x") : printf("-");
			(buffer.st_mode & S_IROTH) ? printf("r") : printf("-");
			(buffer.st_mode & S_IWOTH) ? printf("w") : printf("-");
			(buffer.st_mode & S_IXOTH) ? printf("x ") : printf("- ");
			//LINKS
			printf("%ju ",(uintmax_t) buffer.st_nlink);
			//USER AND GROUP
			printf("%s ",getuname((int)buffer.st_uid));
			printf("%s ",getgname((int)buffer.st_gid));
			
			//SIZE
			printf("%*zu ",7,buffer.st_size);
			//LAST MODIFIED
			toLocalTime(buffer.st_mtime);
			//NAME
			if(b==1)
				(lname!=NULL) ? printf("\033[1;36m%s\x1b[0m -> %s\n",fileArr[i],lname) : printf("\033[1;34m%s\n\x1b[0m",fileArr[i]);
			else if(g==1)
				printf("\033[1;32m%s\n\x1b[0m",fileArr[i]);
			else
				printf("%s\n",fileArr[i]);
		}
		else//LIST FORMAT IS NOT REQUIRED
		{
			if(opt[3]) printf("%ju ",(uintmax_t) buffer.st_ino);
			if(b==1)
			{
				(S_ISLNK(buffer.st_mode)) ? printf("\033[1;36m%s\t\x1b[0m",fileArr[i]) : printf("\033[1;34m%s\t\x1b[0m",fileArr[i]);
			}
			else if(g==1)
				printf("\033[1;32m%s\t\x1b[0m",fileArr[i]);
			else
				printf("%s\t",fileArr[i]);
		}
		
		//IF REPEATED LISTING IS REQUIRED
		if(opt[1])
		{
			if((strcmp(fileArr[i],".") && strcmp(fileArr[i],"..")) && S_ISDIR(buffer.st_mode))
			{
				dirArr[d]=fileArr[i];
				d++;
			}
		}
	}
	

	if(d>0)
		if(doDirLs(dirArr, d, path, opt)==-1)
			return -1;

	printf("\n");
	return 1;
}

//GET THE OPTIONS FROM USER
char* checkAndPlace(char** argv, int argc, char** dirArr, char** fileArr)
{
	char* optCh = malloc(255);
	strcpy(optCh,"");
	int j=0,d=0,f=0,e=0;
	for(j=1; j<argc; j++)
	{
		if(argv[j][0]=='-' && argv[j][1]!='\0')
			strcat(optCh,argv[j]);
		else if(argv[j][0]=='-' && argv[j][1]=='\0')
			return "-";
		else if(argv[j][0]!='-')
		{
			struct stat buffer;
			char* fullPath = malloc(255);
			strcat(fullPath, argv[j]);
			if(access(fullPath,F_OK) == -1)
			{
				errFile = malloc(255);
				strcpy(errFile, fullPath);
				e=1;
				continue;
			}
			if(lstat(fullPath,&buffer)<0)
				continue;
			if(S_ISDIR(buffer.st_mode))
			{
				dirArr[d] = malloc(255);
				dirArr[d++] = argv[j];
			}
			else if(S_ISREG(buffer.st_mode))
			{
				fileArr[f] = malloc(255);
				fileArr[f++] = argv[j];
			}
		}
	}
	if(e) return strcat(optCh,"f");
	strcat(optCh,"\0");
	return optCh;
}

