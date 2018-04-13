
#include "mongothread.h"
#include "atomic.h"
#include <sys/select.h>
#include <sys/time.h>

void init_threads()
{
    stop_flag = 0;
    list_size = 0;
    slist = elist = NULL;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	start();
}

void push_list(ReqList *req)
{
	pthread_mutex_lock(&mutex);
	if(slist == NULL)
		{
            slist = elist = req;
		}
	else
		{
			elist->next = req;
		    elist = req;
        }
    elist->next = NULL; //
	pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    ATOMIC_INC(&list_size);
}

ReqList *pop_list()
{
    pthread_mutex_lock(&mutex);
	ReqList *req = NULL;
    if(slist == NULL)
    {
       pthread_mutex_unlock(&mutex);
       return NULL;
    }		
	else
		{
			req = slist;
			slist = slist->next;
        }
    pthread_mutex_unlock(&mutex);
    ATOMIC_DEC(&list_size);
	return req;
}

int get_size()
{
    return list_size;
}


void* start_thread_work()
{
    pthread_t tid;
    tid = pthread_self();
    //printf("%lu thread start to work...\n", tid);
	for(;;)
		{
			if(stop_flag == 1)
				break;
			pthread_mutex_lock(&mutex);
			if(get_size() == 0)
				pthread_cond_wait(&cond, &mutex);
			pthread_mutex_unlock(&mutex);
           //取出指令请求mongo
			ReqList *req = pop_list();
			if(req && req->cmd)
				{
                    char str[100];
                    sprintf(str, "%lu thread excute cmd(%d)", tid, req->cmd->opcode);
                    write_log(str);
					do_work(req);
                }
		}
    //printf("%lu thread quit...\n", tid);
}

void start()
{
	int i=0;
	for(;i<THRD_NUM; ++i)
		{
			pthread_attr_t attr;
            pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);			
			pthread_create(&tid[i], NULL, (void*)start_thread_work, NULL);
		}
}

void stop()
{
	stop_flag = 1;
	pthread_mutex_lock(&mutex);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
	//等线程退出
	struct timeval delay;
	delay.tv_sec = 0;
	delay.tv_usec = 50000; //50ms
	select(0, NULL,NULL, NULL, &delay);
	
	while(slist)
		{
			ReqList *s = slist;			
			free(s);
			s = NULL;
			slist = slist->next;			
		}


	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}
