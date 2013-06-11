/**********************************************************************************************
*** Filename:
***			create.c
***	Copyright (c) 2013 Wistron
***	All rights reserved.
***
***	Description:
***			ascending sort 1000,0000 numbers of integer 
***
***	Author/Creat Date:
*** 		Albert Yang, Jun 10,13
***
***	Modification History:
***			
***	Note:
***
************************************************************************************************/

#include <stdio.h>/**/
#include <stdlib.h>/**/
#include <fcntl.h>/*For open() close()*/
#include <time.h>/*For srand() rand()*/
#define N   1000000
#define LEN 10000  



int fd,ret;
/*****************************************************************************************
***	Description:		
***		 Creat random numbers and write into rand_array.txt 
***	
***	Parameters:
*** 		m: the numbers you want to create random
***		n: the extent you can create
***		
***
***	Return Value:
***		Return ret.
*******************************************************************************************/
int rand_array(unsigned int m, unsigned int n)
{
	int i,tp;

	fd = open("rand_array.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR );
	
	
	for(i=0;i<n;i++)
	{
		tp = rand()%500000;
//		printf("tp = %d\n",tp);
		ret = lseek(fd,0,SEEK_CUR);//write at the end of the file, don't cover the file.
		if(ret<0)
		{
			printf("lseek error\n");
			goto out;
		}
	
		ret = write(fd,&tp,4);
		if(ret<0)
		{
			printf("write error\n");
			goto out;
		}


	}

out: 
	close(fd);
	return ret;

}
/*****************************************************************************************
***	Description:		
***		 Creat increase numbers and write into increase_array.txt 
***	
***	Parameters:
*** 		m: the numbers you want to create random
***		n: the extent you can create
***		
***
***	Return Value:
***		Return ret.
*******************************************************************************************/
int increase_array(unsigned int m, unsigned int n)
{
	unsigned int select = m;
	unsigned int remaining = n;
	int i;

	fd = open("increase_array.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR );
	
	
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
***		 Creat equal numbers and write into equal_array.txt 
***	
***	Parameters:
*** 		m: the numbers you want to create random
***		n: the extent you can create
***		tp: the test equal value=39
***
***	Return Value:
***		Return ret.
*******************************************************************************************/
int equal_array(unsigned int m, unsigned int n)
{
	int i,tp=39;

	fd = open("equal_array.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR );
	
	
	for(i=0;i<n;i++)
	{
		ret = lseek(fd,0,SEEK_CUR);
		if(ret<0)
		{
			printf("lseek error\n");
			goto out;
		}
	
		ret = write(fd,&tp,4);
		if(ret<0)
		{
			printf("write error\n");
			goto out;
		}

	}

out: 
	close(fd);
	return ret;

}




/*****************************************************************************************
***	Description:		
***		 the main function, create random numbers and write into randArray.txt
***		Creat equal numbers and write into equal_array.txt,Creat increase numbers 
***		and write into increase_array.txt
***		
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
	increase_array(LEN,N);
	rand_array(LEN,N);
	equal_array(LEN,N);
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("create and write costs %f seconds\n",duration);//print the cost time of write
	

	return 0;

}

