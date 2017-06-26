/* 
 * File:   lib_monitor.c
 * Author: Romain Rivière, Alexandre Benazech, Vincent Pera
 *
 * Created on 20 May 2017
 */



#include "lib_monitor.h"



/**
	Function used by the TASK write_log in order to write events to a file
	The periodicity is 1s
	The function reads every message availible in the queue and parse them into the
		file log.xenolog
	
	Warning : This task goes into secondary mode (because of asprintf and fflush())

	@params void *arg
*/


RT_TASK write_log; 

RTIME begin_time;
RT_QUEUE log_queue;

FILE* log_file;

int recording_enabled;

void Write_log(void *arg)
{
	rt_task_set_periodic(NULL, TM_NOW, SECENTOP);
	while (1) {
		rt_task_wait_period(NULL);
		
		if(log_file != NULL){
			int err = sizeof(message_log);
			while(err == sizeof(message_log)){
				
				message_log m;
				err = rt_queue_read(&log_queue,&m,sizeof(message_log),SECENTOP / 10000);	
				
				if(err == sizeof(message_log)){
					char * line;
					if(m.type == MUTEX){

						asprintf(&line,"[%d] [MUTEX] %s: mutex: %s => %s", m.time, m.task_name,m.sarg0,  m.event);
					}
					else if (m.type == SEM){
						asprintf(&line, "[%d] [SEM] %s: sem: %s => %s (%d waiters)", m.time, m.task_name, m.sarg0, m.event, m.iarg0);

					}
					else if (m.type == TASK){

						asprintf(&line, "[%d] [TASK] %s => %s (prio : %d) ", m.time, m.task_name,  m.event, m.iarg0, m.iarg1);
					}
					else if (m.type == TASK_DELETE){

						asprintf(&line, "[%d] [TASK] %s => %s by %s (prio : %d) ", m.time, m.task_name, m.event,m.sarg0, m.iarg0);
					}
					
					rt_printf("Ecriture Log : %s!\n", line);
					rt_fprintf(log_file,"%s\n",line);
					fflush(log_file);
				}else{
					//do nothing
				}
			}
		}else{
			rt_printf("Log : Error, can't access to log file\n");
		}
	}
}


/**
	Initialize and start the recording of events (Tasks, semaphores and mutexes)

    @return 1 if the recording is successfully launched, else 0.
*/

int init_recording(){
			
			
			int ret_value = 0;
			//Opening log file
			log_file = fopen("log.xenolog","w");
			
			if(log_file == NULL){
				rt_printf("ERROR : Couldn't open log_file\n");
				ret_value =0;
			}
			else{
				// initialize reference time at the begining of the log
				begin_time =  rt_timer_ticks2ns(rt_timer_read());

				// creates queue for the messages
				rt_queue_create(&log_queue, "Log Queue", sizeof(message_log)*80, sizeof(message_log), Q_FIFO);
			
				// spawns the task that write to the log file 

				if (rt_task_spawn(&write_log, /* task descriptor */
					"Log Writter", /* name */
					0           /* 0 = default stack size */,
					2          /* priority */,
					T_JOINABLE, /* needed to call rt_task_join after */
					&Write_log,      /* entry point (function pointer) */
					NULL        /* function argument */) != 0)
				{
					rt_printf("rt_task_spawn error\n");
					ret_value=0;
					fclose(log_file);
					
				}else{
					recording_enabled = 1;
					ret_value = 1;
					rt_printf("Started recording events\n");
				}
				return ret_value;
			}

}

/**
	Stop the event collection
	Delete the queue, the task and then closes the log file.
*/
void stop_recording(){
		recording_enabled = 0;
		rt_queue_delete(&log_queue);
		rt_task_delete(&write_log);
		fclose(log_file);
		rt_printf("Stopped Recording events\n");
}

/**
	Writes a message into a queue
	
	@params RT_QUEUE *msg_queue : the address of the queue
	@params void * data : the address of message
	@params int size : the size of the data

	@return the error code return by the rt_queue
*/
int write_in_queue(RT_QUEUE *msg_queue, void * data, int size) {
	int err=0;
	err = rt_queue_write(msg_queue,data,size,Q_NORMAL);
	if (err<0){
		rt_printf("rt_queue_write error %d\n",err);
	}
    return err;
}



/**
	Get the time between the start of recording and when it is called

	@return the time in milliseconds
*/
int get_time_ms(){
	RTIME time = (rt_timer_ticks2ns(rt_timer_read())-begin_time)/1000000;
	return (int)time;
}


/**
	Builds a message to be sent to the queue

	@params message_log * m : address of the message
	@params int time : time of the creation (in ms)
	@params char * task_name : name of the task 
	@params char * event : the event that created that message
	@params char * sarg0 : generic string 
	@params char * sarg1 : generic string
	@params char * sarg2 : generic string
	@params int iarg0 : generic integer
	@params int iarg1 : generic integer

*/

void init_message(message_log * m, message_type type, int time, const char * task_name, const char * event, const char * sarg0, const char * sarg1, const char * sarg2, int iarg0, int iarg1){
	
	m->type = type;
	m->time = time;
	m->iarg0 = iarg0;
	m->iarg1 = iarg1;
	if(sarg0 != NULL){
		memcpy(m->sarg0, sarg0, SARG_SIZE);		
	}
	if(sarg1 != NULL){
		memcpy(m->sarg1, sarg1,SARG_SIZE);		
	}
	if(sarg2 != NULL){
		memcpy(m->sarg2, sarg2, SARG_SIZE);		
	}
	if(task_name != NULL){
		memcpy(m->task_name, task_name, SARG_SIZE);		
	}
	if(event != NULL){
		memcpy(m->event, event, SARG_SIZE);		
	}
	
	
	
}

/**
 
	generic method to log en event regarding a mutex

	@params RT_MUTEX * mut : the mutex
	@params char * event : the event (waiting, acquired, released... )

*/

void log_mutex_generic(RT_MUTEX * mut, const char * event){
	if(recording_enabled){
		message_log m;
		RT_TASK_INFO task_info;
		RT_MUTEX_INFO mut_info;
		rt_task_inquire(NULL, &task_info);
		rt_mutex_inquire(mut, &mut_info);
		
		init_message(&m, MUTEX, get_time_ms(), task_info.name, event, mut_info.name,NULL, NULL, 0,0);
		
		int n=0;
		n = rt_queue_write(&log_queue,&m,sizeof(message_log),Q_NORMAL);
		if (n<0){
			rt_printf("rt_queue_write error %d\n",n);
		}
	}	 
}


/**
	logs when the task waits for a mutex
	must be written just before the rt_mutex_acquire();
	@params RT_MUTEX * mut : the mutex
*/
void log_wait_for_mutex(RT_MUTEX  * mut){
	log_mutex_generic(mut, "waiting");
}

/**
	logs when the task has acquired a mutex
	must be written just after the rt_mutex_acquire();
	@params RT_MUTEX * mut : the mutex
*/
void log_mutex_acquired(RT_MUTEX * mut){
	log_mutex_generic(mut, "acquired");
}

/**
	logs when the task releases a mutex
	must be written just after the rt_mutex_release();
	@params RT_MUTEX * mut : the mutex
*/
void log_mutex_released(RT_MUTEX * mut){
	log_mutex_generic(mut, "released");
}

/**
 
	generic method to log en event regarding a semaphore

	@params RT_SEM * sem : the semaphore
	@params char * event : the event (waiting, entered, signaled... )

*/
void log_sem_generic(RT_SEM * sem, const char * event){
	if(recording_enabled){
		message_log m;
		RT_TASK_INFO task_info;
		RT_SEM_INFO sem_info;
		rt_task_inquire(NULL, &task_info);
		rt_sem_inquire(sem, &sem_info);
		init_message(&m, SEM, get_time_ms(), task_info.name, event, sem_info.name,NULL, NULL,sem_info.nwaiters,0);
		write_in_queue(&log_queue,&m,sizeof(message_log));
	}
	  
}

/**
	logs when the task is waiting at a semaphore
	must be written just before the rt_sem_p();
	@params RT_MUTEX * mut : the mutex
*/
void log_sem_waiting(RT_SEM * sem){
	log_sem_generic(sem, "waiting");
}
/**
	logs when the task has entered a semaphore
	must be written just after the rt_sem_p();
	@params RT_MUTEX * mut : the mutex
*/

void log_sem_entered(RT_SEM * sem){
	log_sem_generic(sem, "entered");
}

/**
	logs when the task signals a semaphore
	must be written just before the rt_sem_v();
	@params RT_MUTEX * mut : the mutex
*/
void log_sem_signaled(RT_SEM * sem){
	log_sem_generic(sem, "signaled");
}


/**
 
	generic method to log en event regarding a task

	@params RT_TASK * task : the task
	@params char * event : the event (waiting, entered, signaled... )

*/
void log_task_generic(RT_TASK * task,  const char * event){
	if(recording_enabled){
		message_log m;
		RT_TASK_INFO self_task_info;
		int err = rt_task_inquire(task, &self_task_info);
		if(err==0){
			init_message(&m, TASK, get_time_ms(), self_task_info.name, event, NULL,NULL, NULL,self_task_info.prio, 0);
			write_in_queue(&log_queue,&m,sizeof(message_log));
		}
	}


}

/**
	logs when a task begins
	must be called at the begining of the function
	no need to give a reference to the task
*/
void log_task_entered(){
	log_task_generic(NULL, "entered");
}
/**
	logs when a task ends
	must be called at the end of the function
	no need to give a reference to the task
*/
void log_task_ended(){
	log_task_generic(NULL, "ended");
}

/**
	logs when a task is deleted
	must be called at before the rt_delete_task()
	@params RT_TASK * task : address of the task that is deleted
*/
void log_task_deleted(RT_TASK * task){

	if(recording_enabled){
		message_log m;
		RT_TASK_INFO self_task_info;
		RT_TASK_INFO param_task_info;
		int err_self = rt_task_inquire(NULL, &self_task_info);
		if(err_self==0){
			int err_param = rt_task_inquire(task, &param_task_info);
			if(err_param==0){
				init_message(&m, TASK_DELETE, get_time_ms(), param_task_info.name, "deleted", self_task_info.name,NULL, NULL,param_task_info.prio, 0);		
				write_in_queue(&log_queue,&m,sizeof(message_log));
			}
		}
	}
}


