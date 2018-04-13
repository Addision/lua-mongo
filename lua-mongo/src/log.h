
#ifndef __LOG_H
#define __LOG_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>


//char logname[30];
static char logname[30];

#define IS(v1, v2, n) ((v1) && (v2) && strncmp((v1), (v2), n)==0)

void set_log_name();
int write_log(char *logstr);
char *get_time(char *tim, int len, int flag);
#endif
