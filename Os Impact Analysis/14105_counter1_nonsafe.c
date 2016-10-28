#include <stdio.h>
#include <pthread.h>
#include <assert.h>
//run using gcc 14105_counter1_nonsafe.c -lpthread
#define MAX 1000000000
typedef struct count{
	long long int value;
}count;

void init(count *c)
{
	c->value=0;
}

void *increment(void *c)
{
	while(((count *)c)->value!=MAX)
		((count *)c)->value++;
	pthread_exit(0);
	return NULL;
}

long long int get(count *c)
{
	long long int rc=c->value;
	return rc;
}

int main(int argc, char *argv[])
{
	pthread_t t1;
 	count c;
 	init(&c);
 	int rc;
    rc=pthread_create(&t1, NULL, increment, &c); assert(rc==0);
    pthread_join(t1, 0);
    printf("counter value is :%lld\n",get(&c));
    return  0;
}