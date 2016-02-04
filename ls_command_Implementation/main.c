/*RACHITA SOWLE-201505613*/
#include <malloc.h>
#include "doLs.h"
#include <string.h>
#include <string.h>
#include <inttypes.h>

int main(int argc, char* argv[])
{
	int opt[6] = {0,0,0,0,0,0};
	if(argc == 1)
	{
		char* dirArr[1] = {"./"};
		doDirLs(dirArr, 1, "./", opt);
		return 0;
	}
	char* dirArr[argc]; 
	char* fileArr[argc];
	int m=0;
	for(m=0; m<argc; m++)
	{
		dirArr[m] = '\0';
		fileArr[m] = '\0';
	}

	int j=0;
	char* optCh = malloc(255);
	if(strcpy(optCh,checkAndPlace(argv, argc, dirArr, fileArr))==NULL)
		return 0;
	if(!strcmp(optCh,"-")) { printf("ls: invalid option -- '-'\n"); return 0; }
	for(j=0; optCh[j]!='\0'; j++)
	{
		switch(optCh[j])
		{
			case 'l': { opt[0]=1; break; }
			case 'R': { opt[1]=1; break; }
			case 'S': { opt[2]=1; opt[4]=0; break; }
			case 'i': { opt[3]=1; break; }
			case 't': { opt[4]=1; opt[2]=0; break; }
			case 'a': { opt[5]=1; ;break; }
			case 'f': { printf("ls: cannot access %s: No such file or directory\n",errFile); return 0; }
			case '-': { break; }
			default : { printf("ls: invalid option -- '%c'\n",optCh[j]); return 0; }
		}
	}

	if(fileArr[0]=='\0' && dirArr[0]=='\0')
		dirArr[0] = "./";

	int dsz=0,fsz=0;
	while(dirArr[dsz]!='\0') dsz++;
	while(fileArr[fsz]!='\0') fsz++;

	if(fsz)
		if(doFileLs(fileArr,fsz,"./",opt)==-1) return 0;
	if(dsz)
		if(doDirLs(dirArr, dsz, "./", opt)==-1) return 0;

	return 0;
}

