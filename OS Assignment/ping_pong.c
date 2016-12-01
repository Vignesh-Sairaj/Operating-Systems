#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>

void * ping(void *par);
void * pong(void *par);

sem_t sping,spong;

int main()
{

pthread_t pi,po;
pthread_attr_t attr;

pthread_attr_init(&attr);
sem_init(&sping,0,0);
sem_init(&spong,0,0);


pthread_create(&pi,&attr,ping,NULL);
pthread_create(&po,&attr,pong,NULL);

pthread_join(pi,NULL);
pthread_join(po,NULL);

return 0;

}

void * ping(void *par)
{

while(1)
	{
		sem_wait(&sping);
		printf("PING\n");
		sem_post(&spong);
	}
}


void * pong(void *par)
{

while(1)
	{
		printf("PONG\n");
		sem_post(&sping);
		sem_wait(&spong);
	}
}
