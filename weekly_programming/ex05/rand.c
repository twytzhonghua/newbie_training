/**********************************************************************************************
*** Filename:
***			rand.c
***	Copyright (c) 2013 Wistron
***	All rights reserved.
***
***	Description:
***			ascending sort 1000,0000 numbers of integer 
***
***	Author/Creat Date:
*** 		Albert Yang, Apri 24,13
***
***	Modification History:
***			Apri 26, 13
***	Note:
***
************************************************************************************************/

#include <stdio.h>/**/
#include <stdlib.h>/**/
#include <fcntl.h>/*For open() close()*/
#include <time.h>/*For srand() rand()*/
#define N   10000000
#define LEN 5000000  



int fd,ret;
int test_read[1000];

/*****************************************************************************************
***	Description:		
***		 Creat random numbers and write into randArray.txt 
***	
***	Parameters:
*** 		m: the numbers you want to create random
***		n: the extent you can create
***		
***
***	Return Value:
***		Return ret.
*******************************************************************************************/
int rand_write(unsigned int m, unsigned int n)
{
	unsigned int select = m;
	unsigned int remaining = n;
	int i;

	fd = open("randArray.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR );
	
	
	for(i=0;i<n;i++)
	{
		/*
		 *create random numbers and write it into randArray.txt
		 * 
		 *Note:
		 *	i=[0,n) so the numbers are written in randArray.txt in ascending order, 
		 * if(rand()%remaining<select) is true, then select-1, and remaining-1
		 *if select become 0  break;
		 */
		if(rand()%remaining<select)
		{
			ret = lseek(fd,0,SEEK_CUR);//write at the end of the file, don't cover the file.
			if(ret<0)
			{
				printf("lseek error\n");
				goto out;
			}
		
  			ret = write(fd,&i,4);
			if(ret<0)
			{
				printf("write error\n");
				goto out;
			}

			select--;
		}
		remaining--;

		if(select == 0 )
			break;
	}

out: 
	close(fd);
	return ret;

}


/*****************************************************************************************
***	Description:		
***		 read some data from array.txt 
***	
***	Parameters:
*** 		i: for loop variable
***		
***		
***
***	Return Value:
***		Return ret.
*******************************************************************************************/
int read_print(void)
{
	int i;
	fd = open("randArray.txt",O_RDWR);
	if(fd<0)
	{
		printf("open 2 error berore read\n");
		ret = fd;
		goto out;
	}

	/*
	*read some data from array.txt to verify
	*/
	for(i=0;i<1000;i++)
	{
		ret = lseek(fd,0,SEEK_CUR);
		if(ret < 0)
		{
			printf("lseek error\n");
			goto out;
		}

		/*
		*check read
		*/
		ret = read(fd,&test_read[i],4);
		if(ret < 0)
		{
			printf("Read error\n");
			goto out;
		}

		printf("test_read[%d]=%d  ",i,test_read[i]);
		if(i%6 == 0)
			printf("\n");
	}


out:	
	close(fd);
	
	return ret;
}

/*****************************************************************************************
***	Description:		
***		 the main function, create random numbers and write into randArray.txt, then
*** 	read from the randArray.txt to verify.
***	
***	Parameters:
*** 		start: the start time
***		finish: the finish time 
***		duration: cost time
***
***	Return Value:
***		Return 0.
*******************************************************************************************/
int main(void)
{

	clock_t start,finish;
	double duration;

	
	srand((unsigned)time(NULL));
	/*
	* Randomly generated LEN numbers
	*/
	start = clock();
	rand_write(LEN,N);
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("create and write costs %f seconds\n",duration);//print the cost time of write
	
	/*
	*read some data from array.txt to verify
	*/
	start = clock();
	read_print();
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("read costs %f seconds\n",duration);
	

	return 0;

}

