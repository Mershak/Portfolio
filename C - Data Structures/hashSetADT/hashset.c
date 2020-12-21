#include "hashset.h"  /* the .h file does NOT reside in /usr/local/include/ADTs */
#include <stdlib.h>
/* any other includes needed by your code */
#define UNUSED __attribute__((unused))

#define DEFAULT_CAPACITY 16
#define MAX_CAPACITY 134217728L
#define DEFAULT_LOAD_FACTOR 0.75
#define TRIGGER 100

typedef struct s_node{
	struct s_node *next;
	void *value;

} SNode;

typedef struct s_data {
	long size;
	long capacity;
	long changes;
	double load;
	double loadFactor;
	double increment;
	SNode **array;
	int (*cmpFxn)(void *first, void *second);
	long (*hashFxn)(void *number, long N);
	void (*freeValue)(void *element);
} SData;

static void purge(const SData *sd, void (*freeValue)(void *element)){
	
	for(int i = 0L; i < sd->capacity; i++){
	    SNode *p, *q;
	    p = sd->array[i];
	    while (p!=NULL){
	    	if(freeValue != NULL)
		    (freeValue)(p->value);
		q = p->next;
		free(p);
		p = q;
		
	    }
	   sd->array[i]=NULL; 
	}

}

static void s_destroy(const Set *s) {
	SData *sd = (SData *)s->self;
	purge(sd, sd->freeValue);
	free(sd->array);
	free(sd);
	free((void *)s);

}

static void s_clear(const Set *s) {
    /* implement the clear() method */
	SData *sd = (SData *)s->self;
	purge(sd, sd->freeValue);
	sd->size = 0;
	sd->load = 0.0;
	sd->changes = 0; 
}

static int add_toBucket(const SData *sd, long i, void *element){
	SNode *new = (SNode *)malloc(sizeof(SNode));
	int status = (new != NULL);
	if(status){
	    	SNode *b = sd->array[i];
		new->next=NULL;
		new->value = element;

	    if(b==NULL){
	        sd->array[i] = new;
	    }
	    else{
		while (b!=NULL){
		    if (b->next == NULL){
			b->next = new;
			break;
		    }
		    b=b->next;
		}
	    }
	}
	return status;
}

static void resize(SData *sd){
	int N;
	SNode *p, **array, *q;
	long i, j;

	N = 2 * sd->capacity;

        if (N > MAX_CAPACITY)
	    N = MAX_CAPACITY;
	if (N == sd->capacity)
	    return;

	array = (SNode **)malloc(N * sizeof(SNode *));

	if (array == NULL)
	    return;

	for (j=0; j<N; j++){
	    array[j] = NULL;
	}

	for (i=0; i < sd->capacity; i++){
	    for(p = sd->array[i]; p!=NULL;p=q){
		q=p->next;
		j = (sd->hashFxn)(p->value,N);
		p->next = array[j];
		array[j]=p;

	    }
	}
	purge(sd,sd->freeValue);
	free(sd->array);
	sd->array = array;
	sd->capacity = N;
	sd->load /= 2.0;
	sd->changes = 0;
	sd->increment = 1.0 / (double)N;
		

}

static int s_contains(const Set *s, void *member) {
    /* implement the contains() method */
	SData *sd = (SData *)s->self;
	long i = (sd->hashFxn)(member,sd->capacity);
	SNode *p = sd->array[i];
	if (p==NULL){
	    return 0;
	}
	else{
	    SNode *q;
	    for (q = p; q != NULL; q=q->next){
	        if (((sd->cmpFxn)(q->value, member)) == 0){
		        return 1;
	        }
	    }
	}
    return 0;
}

static int s_add(const Set *s, void *member) {
    /* implement the add() method */
	SData *sd = (SData *)s->self;
	int i;
	int ans = 0;

	if(sd->changes > TRIGGER){
	    sd->changes = 0;
	    if(sd->load > sd->loadFactor)
		    resize(sd);
	}

	if(s_contains(s,member)){
	    return ans;
	}
	else{
	    i = (sd->hashFxn)(member, sd->capacity);
	    ans = add_toBucket(sd, i ,member);
	    sd->size++;
	    sd->load += sd->increment;
	    sd->changes++;
	    return 1;
	}
}

static int s_isEmpty(const Set *s) {
    /* implement the isEmpty() method */
	SData *sd = (SData *)s->self;
        return (sd->size == 0L);
}

static int s_remove(const Set *s, void *member) {
    /* implement the remove() method */
	SData *sd = (SData *)s->self;
	long i = (sd->hashFxn)(member,sd->capacity);
	SNode *q;
	SNode *prev = NULL;
	for (q=sd->array[i]; q!=NULL; q=q->next){
	    if(((sd->cmpFxn)(q->value, member))==0){

		if (sd->freeValue != NULL){
		    (sd->freeValue)(q->value);
		}
		
		if(prev ==NULL){
		    sd->array[i] = q->next;
		}
		else{
		    prev->next = q->next;
		}

		sd->size--;
		sd->load -= sd->increment;
		sd->changes++;
		free(q);
		q=NULL;

		return 1;
		    
	    }
	    prev=q;
	}
    return 0;
}

static long s_size( const Set *s) {
    /* implement the size() method */
	SData *sd = (SData *)s->self;
    return sd->size;
}

static void **s_toArray(const Set *s, long *len) {
    /* implement the toArray() method */
	SData *sd = (SData *)s->self;
	void **tmp = NULL;
	if(sd->size > 0L){
	    size_t nbytes =(sd->size * sizeof(void *));
	    tmp = (void **)malloc(nbytes);
	    if (tmp != NULL){
		int index = 0;
		for(int i = 0; i < sd->capacity; i++){
		    SNode *q;
		    for(q = sd->array[i]; q != NULL; q=q->next){
			tmp[index] = q->value;
			index++;
		    }
		}
	    }
	}
	*len = sd->size;
        return tmp;
}

static const Iterator *s_itCreate(UNUSED const Set *s) {
    /* implement the itCreate() method */
	SData *sd = (SData *)s->self;
	const Iterator *it = NULL;
	long len = 0L;
	void **tmp = s_toArray(s,&len);
	if (tmp != NULL){
	    it = Iterator_create(sd->size,tmp);
	    if(it==NULL){
		free(tmp);
	    }
	}

    return it;
}

//static const Set *s_create(const Set *s);

static Set template = {
    NULL, s_destroy, s_clear, s_add, s_contains, s_isEmpty, s_remove,
    s_size, s_toArray, s_itCreate
};



static const Set *newHashSet(void (*freeValue)(void*), 
		   int (*cmpFxn)(void*, void*),
                   long capacity, double loadFactor,
                   long (*hashFxn)(void *m, long N)) {

	long N;
	double lf;
	Set *s = (Set *)malloc(sizeof(Set));
	
	if (s != NULL){
	    SData *sd = (SData *)malloc(sizeof(SData));
	    if (sd != NULL){
		N = ((capacity > 0) ? capacity : DEFAULT_CAPACITY);
	        if (N > MAX_CAPACITY)
	       	    N = MAX_CAPACITY;
	 	lf = ((loadFactor > 0.000001) ? loadFactor : DEFAULT_LOAD_FACTOR);
		SNode **array = (SNode **)malloc(N * sizeof(SNode *));
		if(array != NULL){
		    sd->array = array;
		    for (long i = 0L; i < N; i++){
			array[i] = NULL;

	       	    }
		    sd->size = 0L;
		    sd->freeValue = freeValue;
		    sd->hashFxn = hashFxn;
		    sd->cmpFxn = cmpFxn;
		    sd->capacity = N;
		    sd->loadFactor = lf;
		    sd->changes = 0L;
		    sd->increment = 1.0 / (double)N;
		    *s = template;
		    s->self = sd;
		}
		else{
		    free(sd);
		    free(s);
		    s=NULL;

		}
	    }
	    else{
	    	free(s);
		s=NULL;

	    }

	}

	
	return s;
}
const Set *HashSet(void (*freeValue)(void*), int (*cmpFxn)(void*, void*),
                   long capacity, double loadFactor,
                   long (*hashFxn)(void *m, long N)
                  ) {
    /* construct a Set instance and return to the caller */
    return newHashSet(freeValue,cmpFxn,capacity,loadFactor,hashFxn);
}
