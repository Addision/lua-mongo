//����mongo�������

#ifndef __MONGO_THRD_H
#define __MONGO_THRD_H

#include <pthread.h>
#include "mongonew.h"

#define THRD_NUM 5  //default create 5 threads
pthread_t tid[THRD_NUM];

pthread_mutex_t mutex;
pthread_cond_t cond;

int stop_flag;
int list_size;

ReqList *slist;  //����ʼָ��
ReqList *elist;  //����βָ��

void init_threads(); //thread������ں���
void start();
void stop();
int get_size();
void push_list(ReqList *req);
ReqList *pop_list();
void* start_thread_work();
#endif
