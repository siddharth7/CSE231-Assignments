#include<stdio.h>
#include<pthread.h>
#include<assert.h>
//run using gcc -lpthread 14105_counter2_safe.c 
#define MAX 1000000000
typedef struct count{
	long long int value;
    pthread_mutex_t mutex1;
}count;
void init(count *c)
{
	c->value=0;
    c->mutex1=(pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
}
void *increment(void *c)
{
	while(((count *)c)->value<MAX)
		{
            pthread_mutex_lock(&(((count *)c)->mutex1));
            if(((count *)c)->value<MAX)
                ((count *)c)->value++;
            pthread_mutex_unlock(&(((count *)c)->mutex1));
        }
	pthread_exit(0);
	return NULL;
}
long long int get(count *c)
{
	pthread_mutex_lock(&(((count *)c)->mutex1));
    long long int rc=c->value;
    pthread_mutex_unlock(&(((count *)c)->mutex1));
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