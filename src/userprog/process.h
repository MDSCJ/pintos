#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "filesys/file.h"
#include "threads/synch.h"
#include "threads/thread.h"

struct process_child
	{
		tid_t tid;
		struct semaphore load_sema;
		struct semaphore exit_sema;
		bool load_success;
		bool exited;
		bool waited;
		int exit_status;
		int refs;
		char *cmdline;
		struct list_elem elem;
	};

struct file_descriptor
	{
		int fd;
		struct file *file;
		struct list_elem elem;
	};

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

#endif /* userprog/process.h */
