#define _REENTRANT
#include <stddef.h>
#include <stdlib.h>
#include <sched.h>
#include "sthread.h"

int sthread_sem_init(sthread_sem_t *sem, int count)
{
	if (count <= 0)
		return -1;
	sem->count = count;
	sem->guard = 0;
	sem->node = (struct node*) malloc(sizeof(struct node));
	if (sem->node == NULL)
		return -1;
	sem->node->thread = NULL;
	sem->node->next = NULL;
    return 0;
}

int sthread_sem_destroy(sthread_sem_t *sem)
{
	sem->count = 0;
	sem->guard = 0;
	while (sem->node != NULL)
	{
		struct node* node_temp = sem->node;
		sem->node = sem->node->next;
		free(node_temp);
		// printf("I am freeing node\n");
	}
	return 0;
}

int sthread_sem_down(sthread_sem_t *sem)
{
	while (test_and_set(&(sem->guard)))	/*Spin lock*/
		sched_yield();
    
	sem->count--;

	if (sem->count < 0)
	{
		/*Place the thread on the waitlist, sleep*/
		while (sem->node-> next != NULL)
			sem->node = sem->node->next;	

		struct node* node_temp = (struct node*) malloc(sizeof(struct node));
		if (node_temp == NULL)
			return -1;
		node_temp->thread = sthread_self();
		node_temp->next = NULL;
		sem->node->next = node_temp;
		sthread_suspend();
	}

	sem->guard = 0;	/*Unlock*/
    return 0;
}

int sthread_sem_try_down(sthread_sem_t *sem)
{
	int return_val = 0;
	
	while (test_and_set(&(sem->guard)))	/*Spin lock*/
		sched_yield();	
	
	if (sem->count <= 0)
	{
		return_val = -1;
	}

	sem->guard = 0;	/*Unlock*/
    return return_val;
}

int sthread_sem_up(sthread_sem_t *sem)
{
	while (test_and_set(&(sem->guard)))	/*Spin lock*/
		sched_yield();
	
	sem->count++;
	if (sem->count <= 0)
	{
		/*Remove the thread from the waitlist, wake up*/
		if (sem->node->next != NULL) /*Check if the waitlist is already empty*/
		{		
			sthread_t thread = sem->node->thread;
			struct node* node_temp = sem->node->next;
			free(sem->node);
			sem->node=node_temp;
			sthread_wake(thread);
		}
	}

	sem->guard = 0;	/*Unlock*/
    return -1;
}
