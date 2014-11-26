#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <string.h>
#include <semaphore.h>
#include "parser.h"
#include "queue.h"

typedef struct pool_t
{
    pthread_mutex_t q;
    pthread_mutex_t lock;
    sem_t sem_q;
    pthread_t *threads;
} pool_t;

pool_t* pool_init(int num_threads, int jobs);
void cleanup(pool_t* p, int num_threads);
void* locked_dequeue(pool_t* p);
void locked_enqueue(pool_t* p, void* job);

queue_t myq;
int satisfied[100];
int targets; 
int *num;

int main()
{
    queue_init(&myq);
    int numbers[100];
    targets=100;
    int i;
    for(i=0;i<100;i++)
    {
        numbers[i]=i;
        satisfied[i]=0;
        queue_enqueue(&myq, &numbers[i]);
    }
    
    int num_threads=5;
    pool_t* pool = pool_init(num_threads, targets);
    
    cleanup(pool, num_threads);
    pool=NULL;
    return 0;
}

void* work(void* arg)
{
    pool_t* pool = (pool_t*)arg;
    while(targets>0)
    {
        sem_wait(&(pool->sem_q));

        int *job = (int*)locked_dequeue(pool);
        
        if(job==NULL)
        {
            locked_enqueue(pool, NULL);
            sem_post(&(pool->sem_q));
            printf("%lu Exiting Inside!\n", pthread_self());
            pthread_exit(NULL);
        }
        else            
        {
            pthread_mutex_lock(&(pool->lock));
            printf("%d by %lu\n", *job, pthread_self());
            targets--;
            pthread_mutex_unlock(&(pool->lock));
        }
    }

    locked_enqueue(pool, NULL);
    sem_post(&(pool->sem_q));
    printf("%lu Exiting Outside!\n", pthread_self());
    pthread_exit(NULL);
}
void locked_enqueue(pool_t* pool,void * job)
{
    pthread_mutex_lock(&(pool->q));
    queue_enqueue(&myq, job);
    pthread_mutex_unlock(&(pool->q));
}

void* locked_dequeue(pool_t* pool)
{
    pthread_mutex_lock(&(pool->q));
    void *job = queue_dequeue(&myq);
    pthread_mutex_unlock(&(pool->q));
    return job;
}

pool_t* pool_init(int num_threads, int jobs)
{
    pool_t* p = malloc(sizeof(pool_t));
    pthread_mutex_init(&(p->q), NULL);
    pthread_mutex_init(&(p->lock), NULL);
    sem_init(&(p->sem_q),0,jobs);
    p->threads=malloc(sizeof(pthread_t)*num_threads);
    int i;
    num=malloc(sizeof(int)*num_threads);
    for(i=0;i<num_threads;i++)
    {
        num[i]=i;
        pthread_create(&(p->threads)[i], NULL, work, p);
    }
    return p;
}

void cleanup(pool_t* p, int num_threads)
{
    void* result;
    int i;
    for(i=0;i<num_threads;i++)
    {
        int r = pthread_join((p->threads)[i], &result);
        if(r==0)
            printf("pthread %d exited!\n", i);
    }
    pthread_mutex_destroy(&(p->lock));
    pthread_mutex_destroy(&(p->q));
    sem_destroy(&(p->sem_q));
    free(num);
    free(p->threads);
    p->threads=NULL;
    free(p);
    p=NULL;
    queue_destroy(&myq);
}
