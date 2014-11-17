/** @file log.h */

#ifndef __LOG_H_
#define __LOG_H_

typedef struct _log_t 
{
    int size;
    int capacity;
    char ** data; 
} log_t;

void double_capacity(log_t* l);
void log_init(log_t *l);
void log_destroy(log_t* l);
void log_push(log_t* l, const char *item);
char *log_search(log_t* l, const char *prefix);

#endif
