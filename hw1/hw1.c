#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#define SIZE 256


int main(int argc, char* argv[])
{
	int start = 0;
	int end = 0;
	char hisSum[SIZE];
	
	int fd1;
	int fd2;

	if(argc!=3)
	{
		printf("Enter two integers to represent the range\n");
		exit(1);
	}
	
	if(	fd1 = open("histogram.bin", O_RDWR) < 0)
	{
		printf("file open error(fd1): %d\n",fd1);
		exit(1);
	}
	else
	{
		printf("before\n");
		read(fd1,hisSum,SIZE);
		for(int i=0;i<SIZE;i++)
			puts(hisSum);
	}
	
	start = atoi(argv[1]);
	end = atoi(argv[2]);
	
	for(int i=start;i<=end;i++)
	{
		int histogram[SIZE] = {0,};
		if(fd2 = open("data1.bin", O_RDONLY) < 0)
	    {
			printf("file open error(fd2): %d\n",fd2);
			exit(1);
	    }
		printf("%d\n",fd2);
		close(fd2);
	}
	
	close(fd1);
	return 0;
}