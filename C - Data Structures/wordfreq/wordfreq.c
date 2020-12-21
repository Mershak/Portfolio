
//Author: Mert Yapucuoglu
//CIS212
//Prints the words one by one in a new line from the given files

#include <stdio.h>
#include <stdlib.h>
#include "ADTs/stringADT.h"
#include "ADTs/arraylist.h"
#include "ADTs/hashcskmap.h"
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "sort.h"


#define USESTR "usage: %s [-lp] [FILE]...\n"

int keycmp(void *x1, void *x2){
    MEntry *m1 = (MEntry *) x1;
    MEntry *m2 = (MEntry *) x2;
    return strcmp((char *)m1->key, (char *)m2->key);

}


int rkeycmp(void *x1, void *x2){
    MEntry *m1 = (MEntry *) x1;
    MEntry *m2 = (MEntry *) x2;
    return strcmp((char *)m2->key, (char *)m1->key);

}

int valcmp(void *x1, void *x2){
    MEntry *m1 = (MEntry *) x1;
    MEntry *m2 = (MEntry *) x2;
    return (m1->value <  m2->value);

}


int rvalcmp(void *x1, void *x2){
    MEntry *m1 = (MEntry *) x1;
    MEntry *m2 = (MEntry *) x2;
    return (m1->value >  m2->value);

}

void printMap(bool flags[], const CSKMap *m){
	
	long n;
	MEntry **mes = m->entryArray(m,&n);

 	if (flags[0] && flags[2]){
	    sort((void **)mes,n,rkeycmp);
	    for(int i=0; i < n; i++){
		    if(i==(n)){
		        printf("%8ld %s",(long)mes[i]->value, (char *)mes[i]->key);
		    }
		    else
			printf("%8ld %s\n", (long)mes[i]->value, (char *)mes[i]->key);
	    }

	    free((void *)mes);
	    return;
	}
	else if (flags[1] && flags[2]){
	    sort((void **)mes,n,rvalcmp);
	    for(int i=0; i < n; i++){
		    if(i==(n)){
		        printf("%8ld %s",(long)mes[i]->value, (char *)mes[i]->key);
		    }
		    else
			printf("%8ld %s\n", (long)mes[i]->value, (char *)mes[i]->key);
	    }

	    free((void *)mes);
	    return;
	}
	else if (flags[0]){
	    sort((void **)mes,n,keycmp);
	    for(int i=0; i < n; i++){
		    if(i==(n)){
		        printf("%8ld %s",(long)mes[i]->value, (char *)mes[i]->key);
		    }
		    else
			printf("%8ld %s\n", (long)mes[i]->value, (char *)mes[i]->key);
	    }

	    free((void *)mes);
	    return;
	}
	else if (flags[1]){
	    sort((void **)mes, n,valcmp);
	    for(int i =0; i < n; i++){
		    if(i==(n)){
		        printf("%8ld %s",(long)mes[i]->value, (char *)mes[i]->key);
		    }
		    else
			printf("%8ld %s\n",(long)mes[i]->value, (char *)mes[i]->key);
	    }

	    free((void *)mes);
	    return;
	}
	
	else{
	    const Iterator *it = m->itCreate(m);
	    while (it->hasNext(it)){
		MEntry *me;
		(void)it->next(it,(void**)&me);
		printf("%8ld %s\n",(long)me->value, (char *)me->key);
	    }
	    
	it->destroy(it);
	free((void *)mes);
	return;
	}

}

void wordperline(bool flags[],FILE *fp,const CSKMap *m){

	char buf[BUFSIZ];
	const ArrayList *list;
	const Iterator *it;
	const String *stADT = String_create("");
	
	if(stADT== NULL){
	    fprintf(stderr,"Error instantiating StringADT");
	    return;
	}
	
	
	while (fgets(buf,sizeof buf, fp) != NULL){
		stADT->clear(stADT);
		stADT->append(stADT,buf);

		if(flags[3])
			stADT->lower(stADT);
		if(flags[4])
			stADT->translate(stADT,"[:punct:]",' ');

		stADT-> replace(stADT,"\n"," ");
		list = stADT->split(stADT," ");
		if (list == NULL){
		    continue;
		}
		it = list->itCreate(list);
		if(it==NULL){
		    fprintf(stderr, "Error creating iterator");
		    return;
		}
	
		while(it->hasNext(it)){
			char *word;
			it->next(it,(void **)&word);
			if(strlen(word)>0){
				if(m->containsKey(m,word)){
					long count;
					m->get(m,word,(void *)&count);
					count++;
					m->put(m,word,(void *)count);
				}
				else{
					m->put(m,word,(void *)1L);
				}
			}
		}
		it->destroy(it);
		list->destroy(list);
	}
	
	stADT->destroy(stADT);
}




int main(int argc, char *argv[]){

	int opt;
		
	bool flags[5];
	flags[0]=flags[1]=flags[2]=flags[3]=flags[4]=false;

	int exitStatus = EXIT_SUCCESS;

	opterr=0;

	const CSKMap *m = HashCSKMap(0L,0.0,NULL);

	if (m==NULL){
	    fprintf(stderr,"Unable to instantiate CSKMap\n");
	    return EXIT_FAILURE;
	}

	while((opt = getopt(argc, argv,"afilp")) != -1){
	
		switch(opt){
			case 'a':
				flags[0]=true;
				break;
			case 'f':
				flags[1]=true;
				break;
			case 'i':
				flags[2]=true;
				break;
			case 'l':
				flags[3]=true;
				break;
			case 'p':
				flags[4]=true;
				break;
			default: fprintf(stderr,"%s: illegal option, '-%c' \n",argv[0],optopt);
				 goto cleanup;
		}

	}

	if(flags[0] && flags[1]){
	    fprintf(stderr, "Cant both have -a and -f\n");
	    goto cleanup;
	}
	if(flags[2] && !(flags[0] || flags[1])){
	    fprintf(stderr, "Can't -i without -a or without -f\n");
	    goto cleanup;
	}

	if(argv[optind] !=NULL){

		for(int i = 0; (optind+i)<argc; i++){
	
			if(access(argv[optind+i],R_OK)==-1){
				fprintf(stderr, "Given file %s doesn't exist in the directory\n",argv[optind+i]);
				goto cleanup;
			}

				FILE *file = fopen(argv[optind+i],"r");
				wordperline(flags,file,m);
				fclose(file);
		}
		printMap(flags,m);
	}
	
	else{
		wordperline(flags,stdin,m);
		printMap(flags,m);
	
	}

	m->destroy(m);
	return exitStatus;
	
cleanup:
	m->destroy(m);
	return EXIT_FAILURE;
	
	
}

