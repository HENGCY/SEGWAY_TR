/* 
 * File:   lib_monitor.h
 * Author: Romain Rivière, Alexandre Benazech, Vincent Pera
 *
 * Created on 20 May 2017
 */




#ifndef LIB_MONITOR_H
#define LIB_MONITOR_H


#include "../includes.h"

#define SARG_SIZE 20


typedef enum {SEM, MUTEX, TASK, TASK_DELETE} message_type;

typedef struct
{
  message_type type;
  int time;
  char task_name[SARG_SIZE];
  char event[SARG_SIZE];
  char sarg0[SARG_SIZE];
  char sarg1[SARG_SIZE];
  char sarg2[SARG_SIZE];
  int iarg0;
  int iarg1;
  
} message_log;

#ifdef	__cplusplus
extern "C" {
#endif
void stop_recording();
int init_recording();
void log_wait_for_mutex(RT_MUTEX  * mut);
void log_mutex_acquired(RT_MUTEX * mut);
void log_mutex_released(RT_MUTEX * mut);
void log_sem_waiting(RT_SEM * sem);
void log_sem_entered(RT_SEM * sem);
void log_sem_signaled(RT_SEM * sem);
void log_task_entered();
void log_task_ended();
void log_task_deleted(RT_TASK * task);
#ifdef	__cplusplus
}
#endif
#endif
