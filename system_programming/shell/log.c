/** @file log.c */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "log.h"
#define INIT_CAPACITY 10

/**
 * Initializes the log.
 *
 * You may assuem that:
 * - This function will only be called once per instance of log_t.
 * - This function will be the first function called per instance of log_t.
 * - All pointers will be valid, non-NULL pointer.
 *
 * @returns
 *   The initialized log structure.
 */
void log_init(log_t *l) 
{
    l->size=0;
    l->capacity=INIT_CAPACITY;
    l->data=malloc(sizeof(char*)*l->capacity);
}

/**
 * Frees all internal memory associated with the log.
 *
 * You may assume that:
 * - This function will be called once per instance of log_t.
 * - This funciton will be the last function called per instance of log_t.
 * - All pointers will be valid, non-NULL pointer.
 *
 * @param l
 *    Pointer to the log data structure to be destoryed.
 */
void log_destroy(log_t* l) 
{
    int i;
    for(i=0;i<=l->size;i++)
        free(l->data[i]);
    free(l->data);
    l->capacity=0;
    l->size=0;
}

/**
 * Push an item to the log stack.
 *
 *
 * You may assume that:
* - All pointers will be valid, non-NULL pointer.
*
 * @param l
 *    Pointer to the log data structure.
 * @param item
 *    Pointer to a string to be added to the log.
 */
void log_push(log_t* l, const char *item) 
{
    if(l->size == l->capacity)
        double_capacity(l);
    l->data[l->size+1] = malloc(strlen(item)+1);
    strcpy(l->data[l->size+1], item);
    l->data[l->size+1][strlen(item)]=0;
    l->size+=1;
}

/**
 * Preforms a newest-to-oldest search of log entries for an entry matching a
 * given prefix.
 *
 * This search starts with the most recent entry in the log and
 * compares each entry to determine if the query is a prefix of the log entry.
 * Upon reaching a match, a pointer to that element is returned.  If no match
 * is found, a NULL pointer is returned.
 *
 *
 * You may assume that:
 * - All pointers will be valid, non-NULL pointer.
 *
 * @param l
 *    Pointer to the log data structure.
 * @param prefix
 *    The prefix to test each entry in the log for a match.
 *
 * @returns
 *    The newest entry in the log whose string matches the specified prefix.
 *    If no strings has the specified prefix, NULL is returned.
 */
char *log_search(log_t* l, const char *prefix) 
{
    if(prefix==NULL)
        return NULL;
    int i, j;
    for(i=l->size; i>0;i--)
        for(j=0; j<(int)strlen(l->data[i]) && j<(int)strlen(prefix); j++)
            if( (l->data[i][j] == prefix[j]) && j==(int)strlen(prefix)-1 )
                return l->data[i];
    return NULL;
}
void double_capacity(log_t* l)
{
    l->capacity*=2;
    l->data=realloc(l->data, sizeof(char*)*l->capacity);    
}
