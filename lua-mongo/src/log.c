
#include "log.h"

const char *prefix = ".mlog";
static char logname[30] = {'\0'};

char *get_time(char *tim, int len, int flag)
{
    memset(tim, '\0', sizeof(tim));
    time_t t = time(0);
    if(flag == 0) //0 day str, 1 time str
        strftime(tim, len,"%Y%m%d", localtime(&t));
    else
        strftime(tim, len,"%H:%M:%S ", localtime(&t));
    return tim;
}

int write_log(char *logstr)
{
    char llog[1024];
    memset(llog, '\0', sizeof(llog));
    char tistr[50];
    get_time(tistr, 50, 1);
    
    strcpy(llog, tistr);
    strcat(llog, logstr);
    strcat(llog, "\n");

    //write file
    set_log_name();
    FILE *fp = fopen(logname, "a+");
    if(!fp)
        {
            printf("open log error:%s\n", logname);
            return -1;
        }
    fputs(llog, fp);
    fclose(fp);
    return strlen(llog);
}

void set_log_name()
{
    char fname[30];
    get_time(fname, 30, 0);
    strcat(fname, prefix); //20170504.log
    if(IS(fname, logname, 30))
        return;
    memset(logname, '\0', sizeof(logname));
    strcpy(logname, fname);
}


/* int main() */
/* { */
/*     write_log("hahasssssss"); */
/*     return 0; */
/* } */
