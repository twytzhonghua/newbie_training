/**********************************************************************************************
*** Filename:
***			qsort.c
***	Copyright (c) 2013 Wistron
***	All rights reserved.
***
***	Description:
***			快速排序使用分治法（Divide and conquer）策略来把一个串行（list）分为两个子串行（sub-lists）。
***		编写快速排序算法，并尝试优化之，并分别记录“测试数据”为“随机数”/“相等数”/“递增数”的情况下，所耗用的时间。
***
***	Author/Creat Date:
*** 		Albert Yang, Jun 10,13
***
***	Modification History:
***			Jun 11, 13
***	Note:
***
************************************************************************************************/
#include <stdio.h>
#include <fcntl.h>/*For open() close()*/
#include <time.h>/*For srand() rand()*/

#define LEN 10000



int x[LEN];


/*****************************************************************************************
***	Description:		
***		 sorting array .
*******************************************************************************************/
void qsort(int a[],int left,int right)
{
	int i,j,temp;
	i=left;
	j=right;
	temp=a[left];
	if(left>right)
		return;
	while(i!=j)/*找到最终位置*/
	{
		while(a[j]>=temp && j>i)
			j--;
		if(j>i)
		a[i++]=a[j];

		while(a[i]<=temp && j>i)
			i++;
		if(j>i)
			a[j--]=a[i];
	}
	a[i]=temp;
	qsort(a,left,i-1);/*递归左边*/
	qsort(a,i+1,right);/*递归右边*/
}


/*****************************************************************************************
***	Description:		
***		 order rand_array .
*******************************************************************************************/
int order_rand_array(int fd,int ret, int i)
{
	fd = open("rand_array.txt",O_RDWR,S_IRUSR | S_IWUSR);
	if(fd<0)
	{
		printf("open 1 failed!\n");
		close(fd);
		return -1;
	}
	
	for(i=0;i<LEN;i++)
	{
		ret = lseek(fd,0,SEEK_CUR);//write at the end of the file, don't cover the file.
		if(ret<0)
		{
   		 printf("lseek error\n");
   		 close(fd);
		 return -1;
		}
	
		ret	 = read(fd,&x[i],4);
		if(	ret<0)
		{
   		 printf("read 1 error\n");
   		 close(fd);
		 return -1;
		}

	}
	
	printf("****This is rand_array***********\n");
	for(i=0;i<100;i++)
	{
		printf("x[%d]=%d  ",i,x[i]);
		if((i+1)%6==0)
			printf("\n");
	}
	
	printf("\n");
	close(fd);
	qsort(x,0,LEN-1);
	for(i=0;i<100;i++)
	{
		printf("x[%d]=%d  ",i,x[i]);
		if((i+1)%6==0)
			printf("\n");
	}

}


/*****************************************************************************************
***	Description:		
***		 order equal_array .
*******************************************************************************************/
int order_equal_array(int fd,int ret,int i)
{
	fd = open("equal_array.txt",O_RDWR,S_IRUSR | S_IWUSR);
	if(fd<0)
	{
		printf("open 2 failed!\n");
		close(fd);
		return -1;
	}
	
	for(i=0;i<LEN;i++)
	{
		ret = lseek(fd,0,SEEK_CUR);
		if(ret<0)
		{
   		 printf("lseek error\n");
   		 close(fd);
		 return -1;
		}
	
		ret	 = read(fd,&x[i],4);
		if(	ret<0)
		{
   		 printf("read 2 error\n");
   		 close(fd);
		 return -1;
		}

	}
	
	printf("****This is equal_array***********\n");
	for(i=0;i<100;i++)
	{
		printf("x[%d]=%d  ",i,x[i]);
		if((i+1)%6==0)
			printf("\n");
	}
	close(fd);
	printf("\n");
	qsort(x,0,LEN-1);
	for(i=0;i<100;i++)
	{
		printf("x[%d]=%d  ",i,x[i]);
		if((i+1)%6==0)
			printf("\n");
	}
}

/*****************************************************************************************
***	Description:		
***		 order increase_array .
*******************************************************************************************/
int order_increase_array(int fd,int ret,int i)
{
	fd = open("increase_array.txt",O_RDWR,S_IRUSR | S_IWUSR);
	if(fd<0)
	{
		printf("open 3 failed!\n");
		close(fd);
		return -1;
	}
	
	for(i=0;i<LEN;i++)
	{
		ret = lseek(fd,0,SEEK_CUR);
		if(ret<0)
		{
   		 printf("lseek error\n");
   		 close(fd);
		 return -1;
		}
	
		ret	 = read(fd,&x[i],4);
		if(	ret<0)
		{
   		 printf("read 3 error\n");
   		 close(fd);
		 return -1;
		}

	}
	printf("****This is increase_array***********\n");
	for(i=0;i<100;i++)
	{
		printf("x[%d]=%d  ",i,x[i]);
		if((i+1)%6==0)
			printf("\n");
	}
	close(fd);
	printf("\n");
	qsort(x,0,LEN-1);
	for(i=0;i<100;i++)
	{
		printf("x[%d]=%d  ",i,x[i]);
		if((i+1)%6==0)
			printf("\n");
	}
}

int main()
{
	int fd,ret,i;
 	clock_t start,finish;
	double duration;
	/*
	 *count sort rand_array cost time
 	*/
	start = clock();
	order_rand_array(fd,ret,i);
	finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("sort rand_array costs %f seconds\n",duration);
	/*
 	*count sort increase_array cost time
	*/
	start = clock();
	order_increase_array(fd,ret,i);

	finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("sort increase_array costs %f seconds\n",duration);
	/*
 	*count sort equal_array cost time
 	*/
	start = clock();
	order_equal_array(fd,ret,i);
	finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("sort equal_array costs %f seconds\n",duration);

	
	return 0;
}
