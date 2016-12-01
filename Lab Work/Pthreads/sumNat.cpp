#include <iostream>
#include <vector>

#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

pthread_mutex_t mutexSum = PTHREAD_MUTEX_INITIALIZER;

using namespace std;

void* sumFn(void* arg);

int sum = 0;

int main(void) {
	int n;
	cout << "Enter upper limit of sequence: ";
	cin >> n;

	vector<pthread_t> tidArr;

	for (int i = 0; i < n; ++i) {
		pthread_t tid;
		tidArr.push_back(tid);
	}

	for (int i = 0; i < n; ++i)
	{
		int* n = new int{i+1};
		pthread_create(&tidArr[i], NULL, &sumFn, (void*)n);
	}

	for (int i = 0; i < n; ++i)
	{
		pthread_join(tidArr[i], NULL);
	}

	cout << "Sum: " << sum << endl;
}


void* sumFn(void* arg) {
	int n = * (int*) arg;
	delete (int*)arg;
	pthread_mutex_lock(&mutexSum); {
		sum += n;
	} pthread_mutex_unlock(&mutexSum);
	pthread_exit(NULL);
}