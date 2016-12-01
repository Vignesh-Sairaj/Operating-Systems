#include <bits/stdc++.h>

#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>


void* intuser(void* intvar);

int main(void) {
    pthread_t tid;

    int intvar[] {4, 2, 3, 4, 5};

    pthread_create(&tid, NULL, &intuser, &intvar);
    pthread_join(tid, NULL);
    return 0;
}


void* intuser(void* intvar) {
    int* ans = (int*)intvar;
    int num = ans[0];

    int* arr = &ans[1];

    printf("{ ");
    for(int i=0; i < num; i++) {
        printf(" %d ", arr[i]);
    }
    printf("}\n");

    pthread_exit(NULL);
}
