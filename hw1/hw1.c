#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/time.h>
#include<math.h>

#define SIZE 256
	
void cal(double data[], double size)
{
	double sum=0,avg=0,sd=0;
	for(int i=0;i<size;i++)
		sum+=data[i];
	avg = sum/size;
	
	printf("<sum>: %f us\n", sum);
	printf("<avg>: %f us\n",avg);
	
	// 평균 구하기
	
	double diff;
	sum = 0;
	for (int i = 0; i < size; i++) {
    	diff = data[i] - avg;
    	sum += diff * diff;
	}
	sd = sqrt(sum / size);
	
	printf("<sd>: %f\n",sd);

	// 표준편차 구하기
}

int main(int argc, char* argv[])
{
	int start, end; // histogram을 만들 파일의 시작과 끝 번호 각각 argv[1], argv[2]
	int hisSum[SIZE]; // histogram.bin의 정보를 저장, 가공할때 사용할 배열
	char* fileName = (char *)malloc(sizeof(char) * 20); // data파일의 이름을 저장할 문자열
	int fileSize; // data파일의 크기를 저장할 변수
	
	int fd1, fd2; // 각각 histogram.bin, data%d.bin의 파일 기술자

	if(argc!=3)
	{
		fprintf(stderr, "Usage: %s <start> <end>\n", argv[0]);
		exit(1);
	} // 프로세스의 입력이 정확한지 확인
	
	if((fd1 = open("histogram.bin", O_RDWR)) < 0)
	{
		printf("file open error(fd1): %d\n",fd1);
		exit(1);
	} // histogram.bin 파일이 열리는지 확인
	
	start = atoi(argv[1]);
	end = atoi(argv[2]);

	struct timeval startTime, endTime; // 수행 시작시간과 끝시간을 저장할 구조체 변수
	double diffTime;
	double totalTime[end-start]; // 한번의 수행에 사용된 시간을 임시저장할 변수와 각 단계의 수행시간을 저장할 배열
	
	for(int i=start;i<=end;i++)
	{
		gettimeofday(&startTime, NULL); // 시작시간 저장
		read(fd1,hisSum,SIZE*sizeof(int));
		sprintf(fileName,"data%d.bin",i); // open할 data파일 이름 생성

		lseek(fd1,0,SEEK_SET);
		
		if((fd2 = open(fileName, O_RDONLY)) < 0)
	    {
			printf("file open error(fd2): %d\n",fd2);
			exit(1);
	    } // data 파일이 열리는지 확인
		
		fileSize = lseek(fd2,0,SEEK_END); // data파일의 크기
		
		lseek(fd2,0,SEEK_SET);
		
		unsigned char histogram[fileSize]; // 현 data파일을 불러올 배열
		read(fd2,histogram,fileSize);
		
		for(int i=0;i<fileSize;i++)
			hisSum[histogram[i]]++; // histogram 생성	
		
		write(fd1, hisSum, SIZE*sizeof(int));
		
		close(fd2);
		
		gettimeofday(&endTime, NULL); // 종료시간 저장
		diffTime = endTime.tv_usec >= startTime.tv_usec ? (double)( endTime.tv_usec - startTime.tv_usec ) : (double)( endTime.tv_usec - startTime.tv_usec + 1000000.0);
		totalTime[i-start] = diffTime;
		printf("<%d> %f us\n", i, totalTime[i-start]);
	}
	
	cal(totalTime,end-start+1);

	free(fileName);

	close(fd1);
	return 0;
}