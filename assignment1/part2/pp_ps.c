
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

/************************
 * statfile:
 * read statfile and
 * copy vals
 ************************/
void statfile(char *statf, char command[256], char state[256], long unsigned *vsz, long unsigned *rss, long unsigned *utime, long unsigned *stime, long unsigned *starttime);

/************************
 * uptimefile:
 * read and copy uptime
 ************************/
void uptimefile(float *uptime);

/************************
 * compare:
 * compare function to
 * sotr proc array
 ************************/
int compare(const void *p, const void *q);

char sort_type[5];

/************************
 * struct process:
 * stores data for output
 ************************/
struct process {
	long unsigned pid;
	char command[256];
	char state[256];
	float cpu;
	float mem;
	long unsigned vsz;
	long unsigned rss;
};

int main(int argc, char *argv[]) {
	if (argc != 2) {
		perror("Usage: ./pp_ps -[cpu | mem | pid | com]\n");
		return -1;
	}
	strcpy(sort_type, argv[1]);

	DIR *dp;
	struct dirent *ep;
	unsigned long pid;

	long phys_pages = sysconf(_SC_PHYS_PAGES);
	long page_size = sysconf(_SC_PAGE_SIZE);
	long phys_mem_size = phys_pages * page_size;

	//open file /proc/uptime
	float *uptime = malloc(sizeof(float));
	uptimefile(uptime);

	struct process processes[3000];
	int num_processes = 0;

	dp = opendir("/proc");
	if (dp != NULL) {
		while (ep = readdir(dp)) {
			pid = strtol(ep->d_name, NULL, 10);
			if ((ep->d_type == DT_DIR) && (pid > 0)) {
				char command[256];
				char state[256];
				long unsigned *vsz = malloc(sizeof(long unsigned));
				long unsigned *rss = malloc(sizeof(long unsigned));
				long unsigned *utime = malloc(sizeof(long unsigned));
				long unsigned *stime = malloc(sizeof(long unsigned));
				long unsigned *starttime = malloc(sizeof(long unsigned));

				//get statfile name
				char statf[20];
				snprintf(statf, sizeof statf, "%s%s%s", "/proc/", ep->d_name, "/stat");

				//open file /proc/<pid>/stat and do calculations
				statfile(statf, command, state, vsz, rss, utime, stime, starttime);
				float mem = (*rss * getpagesize() * 100) / phys_mem_size;
				float proctime = (*utime + *stime) / sysconf(_SC_CLK_TCK);
				float realtime = *uptime - (*starttime / sysconf(_SC_CLK_TCK));
				float cpu = proctime * 100 / realtime;

				//pass to struct array
				processes[num_processes].pid = pid;
				strcpy(processes[num_processes].command, command);
				strcpy(processes[num_processes].state, state);
				processes[num_processes].vsz = *vsz;
				processes[num_processes].rss = *rss;
				processes[num_processes].cpu = cpu;
				processes[num_processes].mem = mem;
				++num_processes;

				free(vsz);
				free(rss);
				free(utime);
				free(stime);
				free(starttime);
			}
		}
		closedir(dp);

		//sort output
		qsort((void*)processes, num_processes, sizeof(processes[0]), compare);

		//print output
		printf("%-22s%-22s%-22s%-22s%-22s%-22s%-22s\n", "PID", "COMMAND", "STATE", "CPU", "MEM", "VSZ", "RSS");
		for (int i = 0; i < num_processes; ++i) {
			printf("%-22lu%-22s%-22s%-22f%-22f%-22lu%-22lu\n",
				processes[i].pid,
				processes[i].command,
				processes[i].state,
				processes[i].cpu,
				processes[i].mem,
				processes[i].vsz,
				processes[i].rss
			);
		}
	}
	else {
		perror("Couldn't open the /proc directory");
		return -1;
	}
	free(uptime);
}

void statfile(char *statf, char command[256], char state[256], long unsigned *vsz, long unsigned *rss, long unsigned *utime, long unsigned *stime, long unsigned *starttime) {
	FILE *fp = fopen(statf, "r");

	if (fp != NULL) {
		char in[10000];
		while (fgets(in, sizeof in, fp) != NULL) {
			char *tmp = strtok(in, " ");
			for (int i = 0; i < 24; ++i) {
				tmp = strtok(NULL, " ");
				switch (i) {
					case 0:
						strcpy(command, tmp);
						break;
					case 1:
						strcpy(state, tmp);
						break;
					case 12:
						*utime = strtol(tmp, NULL, 10);
						break;
					case 13:
						*stime = strtol(tmp, NULL, 10);
						break;
					case 20:
						*starttime = strtol(tmp, NULL, 10);
					case 21:
						*vsz = strtol(tmp, NULL, 10);
						break;
					case 22:
						*rss = strtol(tmp, NULL, 10);
						break;
				}
			}
		}
	}
	fclose(fp);
}

void uptimefile(float *uptime) {
	FILE *fp = fopen("/proc/uptime", "r");
	if (fp != NULL) {
		char in[10000];
		if (fgets(in, sizeof in, fp) != NULL) {
			char *tmp = strtok(in, " ");
			*uptime = strtof(tmp, NULL);
		}
		else {
			perror("Could not read from /proc/uptime");
			exit(-1);
		}
		fclose(fp);
	}
	else {
		perror("Could not open /proc/uptime");
		exit(-1);
	}
}

int compare(const void *p, const void *q) {
	if (strcmp(sort_type, "-cpu") == 0) {
		if (((struct process *) p)->cpu > ((struct process *) q)->cpu)
			return -1;
		else
			return 1;
	}
	else if (strcmp(sort_type, "-mem") == 0) {
		if (((struct process *) p)->mem > ((struct process *) q)->mem)
			return -1;
		else
			return 1;
	}
	else if (strcmp(sort_type, "-pid") == 0) {
		if (((struct process *) p)->pid > ((struct process *) q)->pid)
			return -1;
		else
			return 1;
	}
	else if (strcmp(sort_type, "-com") == 0) {
		return strcmp(((struct process *) p)->command,
			      ((struct process *) q)->command);
	}
	else {
		perror("Usage: ./pp_ps -[cpu | mem | pid | com]\n");
		exit(-1);
	}
	return 0;
}