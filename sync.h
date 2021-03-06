#ifndef _STHREAD_SYNC_H_
#define _STHREAD_SYNC_H_

struct node
{
	sthread_t thread;
	struct node *next;
};

struct sthread_sem_struct 
{
	int count;
	unsigned long guard;
	struct node* node;
};

typedef struct sthread_sem_struct sthread_sem_t;

int sthread_sem_init(sthread_sem_t *sem, int count);
int sthread_sem_destroy(sthread_sem_t *sem);
int sthread_sem_down(sthread_sem_t *sem);
int sthread_sem_try_down(sthread_sem_t *sem);
int sthread_sem_up(sthread_sem_t *sem);

#endif
