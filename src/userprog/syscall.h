#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include "threads/synch.h"

extern struct lock filesys_lock;

void syscall_init (void);
void acquire_filesys_lock (void);
void release_filesys_lock (void);

#endif /* userprog/syscall.h */
