#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

void header()
{
	printf("%10s %30s %10s %10s %20s","PID","Command","STATE","PPID","VM Size (bytes)\n");
printf("-----------------------------------------------------------------------------------------\n");

}
int main()
{
int pid, num, ppid, size;
struct dirent *ep;
FILE *file;
char buff[1000],command[1000];
DIR* dp;
char state;
header();

dp = opendir ("/proc");
 if (dp != NULL)
 {
 while (ep = readdir (dp))
 {

 pid = strtol(ep->d_name, NULL, 10);
 if( ( ep->d_type == DT_DIR ) && ( pid > 0) )
 {
 //puts (ep->d_name);
sprintf(buff,"/proc/%d/stat",pid);
	if((file = fopen(buff,"r")) == NULL)
{
	printf("Opening file!");
	exit(1);
}

	fscanf(file,"%d %s %c %d %lu", &num, command, &state,  &ppid, &size);
	printf("%10d %30s %10c %10d %12lu\n", num, command, state, ppid, size);
 }
 }
 closedir(dp);
 }
 else
 perror ("Couldn't open the directory");
return 1;
}
