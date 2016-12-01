#include <bits/stdc++.h>

#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFSZ 20

using namespace std;


int genCount = 0;
int Buf[BUFSZ];

int in = 0, out = 0;

sem_t fullCount;
sem_t emptyCount;

pthread_mutex_t rw_mut;

void* producer(void* nt);
void* consumer(void* nt);


int main(void) {
    srand(time(NULL));
    cout << "There!" << endl;
    sem_init(&fullCount, 0, 0);
    sem_init(&emptyCount, 0, BUFSZ);
    pthread_mutex_init(&rw_mut, NULL);

    pthread_t pr, cs;

    pthread_create(&pr, NULL, &producer, NULL);
    pthread_create(&cs, NULL, &consumer, NULL);

    pthread_join(pr, NULL);
    pthread_join(cs, NULL);
    return 0;
}


void* producer(void* nt) {

    int sval;
    do {

        if (rand()%2 == 0) {
            sleep(1);
        }

        sem_getvalue(&emptyCount, &sval);
        cout << "\n\nProducer: Waiting for emptyCount to be +ve, current val = " << sval << endl;

        sem_wait(&emptyCount);
        pthread_mutex_lock(&rw_mut);
        {
            cout << "\n\nProducer: entering write block..." << endl;
            cout << "Writing " << genCount << " to buffer..." << endl;
            Buf[in] = genCount;
            genCount++;
            in = (in+1)%BUFSZ;

            sem_getvalue(&fullCount, &sval);
            cout << "Incrementing fullCount, current val = " << sval << endl;
        }
        pthread_mutex_unlock(&rw_mut);
        sem_post(&fullCount);
    } while(true);
    pthread_exit(NULL);
}
void* consumer(void* nt) {
    do {
        int sval;
        if (rand()%2 == 0) {
            sleep(1);
        }
        sem_getvalue(&fullCount, &sval);
        cout << "\n\nConsumer: Waiting for fullCount be +ve, current val = " << sval << endl;

        sem_wait(&fullCount);
        pthread_mutex_lock(&rw_mut);

        {
            cout << "\n\nConsumer: entering read block..." << endl;
            cout << "Reading " << Buf[out] << " from buffer..." << endl;
            out = (out+1)%BUFSZ;

            sem_getvalue(&emptyCount, &sval);
            cout << "Incrementing emptyCount, current val = " << sval << endl;
        }
        pthread_mutex_unlock(&rw_mut);
        sem_post(&emptyCount);
    } while(true);
    pthread_exit(NULL);
}
