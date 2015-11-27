#include<stdio.h>
#include<pthread.h>
#include<assert.h>
#include<time.h>
//run using gcc -lpthread 14105_stp.c 
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
    int counter = 0;
    double total_time=0;;
    while(counter<50)
        {
            pthread_t t1;
            clock_t start, end;
            double cpu_time_used;
            count c;
            start = clock();
            init(&c);
            int rc;
            rc=pthread_create(&t1, NULL, increment, &c); assert(rc==0);
            pthread_join(t1, 0);
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            total_time+=cpu_time_used;
            //printf("time taken %lf\n",cpu_time_used);
            //printf("counter value is :%lld\n",get(&c));
            counter++;
        }
        printf("Average time taken on 50 iterations is %lf\n",total_time/50);
    return  0;
}