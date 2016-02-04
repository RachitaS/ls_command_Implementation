/*RACHITA SOWLE-201505613*/
#include <time.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "utility.h"

void toLocalTime(time_t mtime)
{
	struct tm *tm = localtime(&mtime);
	char* month[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	printf("%*s %*d %02d:%02d ",3,month[(tm->tm_mon)],2, tm->tm_mday, tm->tm_hour, tm->tm_min);

}
char* getuname(int uid)
{
	struct passwd pwd = *getpwuid(uid);
	return pwd.pw_name;
}
char* getgname(int gid)
{
	struct group grp = *getgrgid(gid);
	return grp.gr_name;
}
void sortStringArr(char** fileArr, int n, char c, char* path)
{
	int i=1, j=0;
	char* tmp = malloc(255);

	for(i=0;i<n-2;i++)
	{
		for(j=0; j<n-i-1;j++)
		{
			if(compare(fileArr[j],fileArr[j+1],c,path)<0)
			{
				tmp = fileArr[j];
				fileArr[j] = fileArr[j+1];
				fileArr[j+1]= tmp;
			}
		}
	}

}

int compare(char* s1, char* s2, char c, char* path)
{
	if(c=='n')
	{
		if(strcmp(s1,s2)<0) return -1;
		else return 1;
	}
	else
	{
		struct stat buffer1;
		struct stat buffer2;
		char* fullPath1 = malloc(255);
		strcpy(fullPath1, path);
		strcat(fullPath1, "/");
		strcat(fullPath1, s1);
		char* fullPath2 = malloc(255);
		strcpy(fullPath2, path);
		strcat(fullPath2, "/");
		strcat(fullPath2, s2);
		if(lstat(fullPath1,&buffer1)<0) printf("ERROR\n");
		if(lstat(fullPath2,&buffer2)<0) printf("ERROR\n");
		if(c=='t')
		{
			if(difftime(buffer1.st_mtime,buffer2.st_mtime)<0) return -1;
			else return 1;
		}
		else
		{
			if(buffer1.st_size<buffer2.st_size) return -1;
			else return 1;
		}

	}
	return 1;
}


