/*RACHITA SOWLE-201505613*/
#include <time.h>
int doFileLs(char* fileArr[], int num, char* path, int opt[6]);
int doDirLs(char* dirArr[], int num, char* path, int opt[6]);
char* checkAndPlace(char** argv, int argc, char** dirArr, char** fileArr);
void sortFileArr(char** fileArr,int n,char c, char* path);
char* errFile;
