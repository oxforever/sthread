#define _REENTRANT
#include "sthread.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

int sthread_sem_init(sthread_sem_t *sem, int count)
{
	if (count <= 0)
		return -1;
	sem -> count = count;
	sem -> guard = 0;
	sem -> node = (struct node*) malloc(sizeof(struct node));
	if (sem -> node == NULL)
		return -1;
	sem -> node -> thread = NULL;
	sem -> node -> next = NULL;
    return 0;
}

int sthread_sem_destroy(sthread_sem_t *sem)
{
	sem -> count = 0;
	sem -> guard = 0;
	while (sem -> node != NULL)
	{
		struct node* node_temp = sem -> node;
		sem -> node = sem -> node -> next;
		free(node_temp);
		// printf("I am freeing node\n");
	}
	return 0;
}

int sthread_sem_down(sthread_sem_t *sem)
{
	/* FILL ME IN! */
        return -1;
}

int sthread_sem_try_down(sthread_sem_t *sem)
{
	/* FILL ME IN! */
        return -1;
}

int sthread_sem_up(sthread_sem_t *sem)
{
	/* FILL ME IN! */
        return -1;
}
