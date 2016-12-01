#include <pthread.h>
#include <bits/stdc++.h>
#include<algorithm>
using namespace std;

int num;
int* arr;


void * avg_func(void* param) {
    int avg;
    int sum=0;
    for(int i=0;i<=num;i++)
    {
        sum=sum+arr[i];
    }
    avg=sum/num;
    pthread_exit((void *)avg);
}

void * min_func(void* param) {
    int min=arr[1];
    for(int i=2;i<=num;i++)
    {
        if(min>=arr[i])
        min=arr[i];
    }
    pthread_exit((void *)min);
}
void * max_func(void* param)
{
    int max=arr[1];
    for(int i=2;i<=num;i++)
    {
        if(max<=arr[i])
        max=arr[i];
    }
    pthread_exit((void *)max);
}



int  main()
{
    int avg_ans,max_ans,min_ans;
    pthread_t thread_avg,thread_min,thread_max;
    cout<<"\n enter  n:";
    cin>>num;

    arr = new int[num];

    for(int i=0;i<num;i++)
    {
        cout<<"\n Enter the "<<i<<"th number of the array=";
        cin>>arr[i];
    }
    int var=pthread_create(&thread_avg,NULL,&avg_func,NULL);
    if(var==0)
    cout<<"\n CREATED THREAD\n ";

    var=pthread_create(&thread_min,NULL,&min_func,&num);
    if(var==0)
    cout<<"\n CREATED THREAD\n ";
    var=pthread_create(&thread_max,NULL,&max_func,&num);
    if(var==0)
    cout<<"\n CREATED THREAD\n ";

    pthread_join( thread_avg,(void**)&avg_ans);
    pthread_join( thread_max,(void**)&max_ans);
    pthread_join( thread_min,(void**)&min_ans);

    cout<<"\n the avg="<<avg_ans<<"\n the min="<<min_ans<<"\n the max="<<max_ans;
    pthread_exit(NULL);

    return 0;
}
