#include <iostream>
#include <vector>
#include <cmath>

#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

pthread_mutex_t mutexSum = PTHREAD_MUTEX_INITIALIZER;

using namespace std;

void* sumFn(void* arg);

void swap(int* a, int* b);

int main(void) {
	int n;
	cout << "Enter upper limit of sequence: ";
	cin >> n;

	pthread_t tid;

	int* arg = new int{n};
	pthread_create(&tid, NULL, &sumFn, (void*)arg);
	pthread_join(tid, NULL);
	return 0;
}


void* sumFn(void* arg) {

	int n = * (int*) arg;
	delete (int*)arg;

	int n0 = 0, n1 = 1;

	cout << "0: " << n0 << endl;
	cout << "1: " << n1 << endl;

	for (int i = 2; i < n; ++i)
	{
		n0 = n0+n1;
		cout << i << ": " << n0 << endl;

		swap(&n0, &n1);
	}

	pthread_exit(NULL);
}

void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
	return;
}