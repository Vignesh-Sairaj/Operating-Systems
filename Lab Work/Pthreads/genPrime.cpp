#include <iostream>
#include <vector>
#include <cmath>

#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

pthread_mutex_t mutexSum = PTHREAD_MUTEX_INITIALIZER;

using namespace std;

void* sumFn(void* arg);


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

	if (n >=2)
		cout << 2 << endl;

	for (int i = 3; i <= n; ++i) {
		bool isPrime = true;
		for (int j = 2; j*j <= i; ++j) {
			if(i%j == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime) {
			cout << i << endl;
		}
	}

	pthread_exit(NULL);
}