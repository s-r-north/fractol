#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct s_butt
{
	int num;
}t_butt;

int arr[250];

/* This is our thread function.  It is like main(), but for a thread*/
void *threadFunc(void *arg)
{
	t_butt *boi = (t_butt*)arg;
	int n = (250 / 5) * boi->num;
	int i = 0;
	while(i < 50)
	{
		usleep(1);
		arr[n] = n * 5;
		printf("threadFunc says: %d arr[%d] = %d\n",boi->num,n,arr[n]);
		++n;
		++i;
	}

	return NULL;
}

int main(void)
{
	pthread_t pth[5];	// this is our thread identifier
	int i = -1;
	t_butt boi[5];

	//char s[5] = "abcde";

	while (++i < 5)
	{
		boi[i].num = i;
		pthread_create(&pth[i],NULL,threadFunc,&boi[i]);
	}
	
	// while(i < 100)
	// {
	// 	usleep(1);
	// 	arr[n] = n * 2;
	// 	printf("main is running...arr[%d] = %d\n",n,arr[n]);
	// 	++n;
	// 	++i;
	// }

	//printf("main waiting for thread to terminate...\n");
	i = -1;
	while (++i < 5)
		pthread_join(pth[i],NULL);

	return 0;
}