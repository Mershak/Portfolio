/*
 * implementation for linked-list-based generic FIFO queue
 */

#include "ADTs/llistqueue.h"
#include <stdlib.h>

/* any other includes needed for the implementation */

typedef struct q_node{
	struct q_node *next;
	void *value;

} QNode;

typedef struct q_data {
    /* flesh out the instance specific data structure */
	long size;
	QNode *head;
	QNode *tail;
	void (*freeValue)(void *f);

} QData;
/* any other data structures needed */


static void q_destroy(const Queue *q) {
    /* implementation of the destroy() method */
	QData *qd = (QData *)q->self;
	
	QNode *d;
	for(d = qd->head; d != NULL; d = d->next)
		(*qd->freeValue)(d->value);

	QNode *p, *k = NULL;
	
	for(p= qd->head; p != NULL; p=k){
	k = p->next;
	free(p);
	}
	free(qd);
	free((void *)q);
	
}

static void q_clear(const Queue *q) {
    /* implementation of the clear() method */
	QData *qd = (QData *)q->self;
	
	QNode *d;
	for(d = qd->head; d != NULL; d = d->next)
		(*free)(d->value);

	QNode *p, *k = NULL;
	
	for(p= qd->head; p != NULL; p=k){
	k = p->next;
	free(p);
	}
	qd->head = qd->tail= NULL;
	qd->size= 0L;

}

static int q_enqueue(const Queue *q, void *element) {
    /* implementation of the enqueue() method */
	QData *qd = (QData *)q->self;
	QNode *new = (QNode *)malloc(sizeof(QNode));
	int status = (new != NULL);

	if(status){
	   
	    new->next=NULL;
	    new->value = element;

	    if(qd->size==0L){
		qd->head=qd->tail=new;
		qd->size++;
	    }
	    else{
	        qd->tail->next = new;
	        qd->tail=new;
	        qd->size++;
	    }

	}
	return status;
}

static int q_front(const Queue *q, void **element) {
    /* implementation of the front() method */
	QData *qd = (QData *)q->self;

	int status = (qd->size > 0L);

	if(status){
	*element = qd->head->value;
	}
	
	return status;

}

static int q_dequeue(const Queue *q, void **element) {
    /* implementation of the dequeue() method */
	QData *qd = (QData *)q->self;

	int status = (qd->size > 0L);

	if (status){
	
	QNode *p = qd->head;
	*element = p->value;
	if(qd->size==1L){
	    qd->head=qd->tail=NULL;
	}
	else{
	    qd->head = p->next;
	}

	free(p);
	qd->size--;
	}
	return status;
}

static long q_size(const Queue *q) {
    /* implementation of the size() method */
	QData *qd = (QData *)q->self;

	return qd->size;
}

static int q_isEmpty(const Queue *q) {
    /* implementation of the isEmpty() method */
	QData *qd = (QData *)q->self;

	return (qd->size == 0L);
}

static void **q_toArray(const Queue *q, long *len) {
    /* implementation of the toArray() method */
	QData *qd = (QData *)q->self;
	void **theArray = NULL;
	if(qd->size > 0L){
	    theArray = (void **)malloc(qd->size*(sizeof(void *)));
	    if (theArray != NULL){
		long i = 0L;
		QNode *p;
		for(p = qd->head; p!=NULL ; p = p->next){
		    theArray[i++]=p->value;
		}
	    }

	}
	if (theArray !=NULL){
	    *len = qd->size;
	}
	return theArray;
}

static const Iterator *q_itCreate(const Queue *q) {
    /* implementation of the itCreate() method */
	QData *qd = (QData *)q->self;
	const Iterator *it = NULL;
	long a = 0L;
	void **tmp = q_toArray(q,&a);
        if(tmp!=NULL){
	    it = Iterator_create(qd->size,tmp);
	    if(it==NULL){
		free(tmp);
	    }
	}
	return it;
}

static const Queue *q_create(const Queue *q);
/* this is just declaring the signature for the create() method; it's
   implementation is provided below */

static Queue template = {
    NULL, q_create, q_destroy, q_clear, q_enqueue, q_front, q_dequeue, q_size,
    q_isEmpty, q_toArray, q_itCreate
};

static const Queue *newQueue(void(*freevalue)(void *e)){
	
	Queue *q = (Queue *)malloc(sizeof(Queue));

	if (q !=NULL){
	    QData *qd = (QData *)malloc(sizeof(QData));
	    if(qd!=NULL){
		qd->size=0L;
		qd->head=NULL;
		qd->tail=NULL;
		qd->freeValue=freevalue;
		*q=template;
		q->self=qd;

	    }
	    else{
		free(q);
		q = NULL;
	    }
	}
	return q;
}


static const Queue *q_create(const Queue *q) {
    /* implementation of the create() method */
	QData *qd = (QData *)q->self;
	return newQueue(qd->freeValue);

}

const Queue *LListQueue(void (*freeValue)(void *e)) {
    /* implementation of the structure-specific constructor */
	return newQueue(freeValue);
}

const Queue *Queue_create(void (*freeValue)(void *e)) {
    /* implementation of the generic constructor */
	return newQueue(freeValue);
}
