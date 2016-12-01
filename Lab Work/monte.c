#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

long in_circle = 0;

long points_per_thread;

pthread_mutex_t atttr= PTHREAD_MUTEX_INITIALIZER;

void *runner(void *param);

int main(int argc, char const *argv[])
{
	int i;
	if(argc!=3){
		fprintf(stderr, "Usage: %s <num_points> <numthreads>\n", argv[0]);
		return -1;
	}

	long points = atol(argv[1]);
	long threads = atol(argv[2]);

	 points_per_thread = points / threads;

	pthread_t *tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	tid = (pthread_t *)malloc(sizeof(pthread_t )*threads);

	for(i = 0; i<threads; i++){

		pthread_create(&tid[i],&attr,runner,NULL);

	}


	for(i = 0; i<threads; i++){

		pthread_join(tid[i],NULL);

	}

	printf("%f  \n",4*(double)in_circle/(double)points );


	return 0;
}

void *runner(void *param)
{
	double x,y;
	int i;
	int incircle = 0;
	srand(time(0));
	for (i = 0; i < points_per_thread; ++i)
	{

        x=(double)rand()/(double) RAND_MAX;
        y=(	double)rand()/(double) RAND_MAX;

        if( y <= sqrt(1-pow(x,2)) )

            incircle+=1;

	}

	pthread_mutex_lock(&atttr);

		in_circle = in_circle+incircle;

	pthread_mutex_unlock (&atttr);
}
