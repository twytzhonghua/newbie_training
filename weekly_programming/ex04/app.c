/**********************************************************************************************
*** Filename:
***		app.c
***	Copyright (c) 2013 Wistron
***	All rights reserved.
***
***	Description:
***		show two methods of ascending sort 1000,0000 numbers of integer 
***
***	Author/Creat Date:
*** 		Albert Yang, May 3,13
***
***	Note:
***
************************************************************************************************/


#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define N 10000


void sort1(int a[], int n)
{
    int i,j;
    int temp;

    for(i=1;i<n;i++)
    for(j=i;j>0;j--)
    {	
	if(a[j-1] > a[j])
        {
	    temp = a[j];
	    a[j] = a[j-1];
	    a[j-1] = temp;
	}
    }
}



void sort2(int a[], int n)
{
    int i,j;
    int temp;

    for(i=1;i<n;i++)
    {
	temp = a[i];//把temp放在第二个内循环外部
	for(j=i;j>0;j--)
	{
		if(a[j-1] > temp)
		a[j] = a[j-1];
	}
	a[j] = temp;//把a[j]赋值放在第二个内循环外部
    }
}





void main(void)
{
    int a[N];
    int i;
    clock_t start,finish;
    double duration;
    int cmd;

    printf("Use sort1(1) or sort2(2)>>:");
    scanf("%d",&cmd);

    /*
    *随机产生10000个数
    */
    srand( (unsigned)time( NULL));
    for(i=0;i<N;i++)
    {
	a[i] = rand()%20000;
    }

    for(i=0;i<N;i++)
    printf("a[%d] = %d\n",i,a[i]);
    
    switch(cmd)
    {	
	case 1:
	 start = clock();
   	 sort1(a,N);
   	 finish = clock();
   	 duration = (double)(finish - start) / CLOCKS_PER_SEC;

   	 printf("sort1 cost %f seconds\n",duration);
 	 break;
	

	case 2:
   	 start = clock();
   	 sort2(a,N);
   	 finish = clock();
   	 duration = (double)(finish - start) / CLOCKS_PER_SEC;

   	printf("sort1 cost %f seconds\n",duration);
   	break;


	default:
		break;
   
   
   }

}
















