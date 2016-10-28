#include<stdio.h>
#include<pthread.h>
#include<assert.h>
//run using gcc -lpthread 14105_mtp_c1.c 
#define MAX 1000000000

int t;
typedef struct counte{
	long long int count;
	pthread_mutex_t mutex1;
}counte;

void *increment(void *arg)
{
	counte *c = (counte *)arg;
	while(c->count < 1000000000)
	{
		int rc = pthread_mutex_lock(&c->mutex1);
		assert(rc == 0);
		if(c->count < 1000000000)
			c->count++;
		rc = pthread_mutex_unlock(&c->mutex1);
		assert(rc == 0);
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	int i;
	for(i=2;i<=10;i++)
	{
		t=i;
		pthread_t t1[i+1];
        int counter=0;
        double total_time=0;
        while(counter<50)
        {
	        counte c;
	        c.count=0;
	        pthread_mutex_init(&(c.mutex1),NULL);
	        clock_t start, end;
	        double cpu_time_used;
		 	start = clock();
		 	int rc;
		 	int j;
		 	for(j=0;j<i;j++)
		 	{
		    	rc=pthread_create(&t1[j], NULL, increment, &c);
		    	assert(rc==0);
		    }
		    for(j=0;j<i;j++)
		 	{
		    	rc = pthread_join(t1[j], 0);
		    	assert(rc == 0);
		    }
		    end = clock();
		    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		   	total_time+=cpu_time_used;
		    counter++;
		    printf("counter value is :%lld\n",c.count);
		}
		printf("time taken %lf by %d threads\n",total_time/50, i);
	}
    return  0;
}
