/**********************************************************************************************
*** Filename:
***			order_write.c
***	Copyright (c) 2013 Wistron
***	All rights reserved.
***
***	Description:
***			输入：一个文件中，含有小于n且没有重复的k个整数(其中n=10,000,000; k=5,000,000);
***			输出：请按升序排列输入的整数列表！
***			限制条件：最多有（大约）1MB的内存空间可用。
***
***	Author/Creat Date:
*** 		Albert Yang, May 12,13
***
***	Modification History:
***			May 15, 13
***	Note:
***
************************************************************************************************/

#include <stdio.h>/**/
#include <stdlib.h>/**/
#include <fcntl.h>/*For open() close()*/
#include <time.h>/*For srand() rand()*/
#define N   10000000
#define LEN 5000000


/*
*共用内存为 1000000/64*4KB = 0.625M 内存
*/
int store_bit[1+N/64];

/*****************************************************************************************
***	Description:		
***			set the location's bit value to be 1.
***	
***	Parameters:
***		i: 	the integer which you will input
***		location:	which location in 32 bits
***	Note:
***		if i = 38, location = 38%32 = 6; 
***					store_bit[1] |= (1<<6) , 0000 0000 0000 0000 0000 0000 0100 0000 
**					set the location's bit value 1.
*******************************************************************************************/
void set_bit_1(unsigned i)
{
	int location;
	location = i%32;
	store_bit[i/32] |= (1 << location);

}


/*****************************************************************************************
***	Description:		
***			set the location's bit value to be 0, initialize all data
***	
***	Parameters:
***		i: 	the integer which you will input
***		location:	which location in 32 bits
***	Note:
***		if i = 38, location = 38%32 = 6; 
***					store_bit[1] &= ~(1<<6);  1111 1111 1111 1111 1111 1011 1111, 0 is the location in 32 bits
**					set the location's bit value 0.
*******************************************************************************************/
void set_bit_0(unsigned i)
{
	int location;
	location = i%32;
	store_bit[i/32] &= ~(1 << location);

}

/*****************************************************************************************
***	Description:		
***			check the location's bit value is 1 or 0, if it's 1, print the number.
***	
***	Parameters:
***		i: 	the integer in data
***		location:	which location in 32 bits
***		temp	:	test the location's bit is 1 or 0
***	Note:
***		if i = 38, location = 38%32 = 6; 
***				temp =	store_bit[1] & (1<<6)  , 0000 0000 0000 0000 0000 0000 0100 0000 
***				temp = 1;
***	Return value：
***			if temp != 0, return 1, else return 0.
*******************************************************************************************/
int check_bit_1(unsigned i)
{
	int location, temp;
	location = i%32;
	temp = store_bit[i/32] & (1 << location); 
	if(temp)
		return 1;
	else
		return 0; 

}


/*****************************************************************************************
***	Description:		
***		 write ordered  into orderArray.txt in two times
***	
***	Parameters:
*** 		count: times = 2, write the first number(count == 1)the lseek must be changed the end
***			i: for loop
*** 		data: store the temp data that will be written in 
***			ret: return value
***
***	Return Value:
***		Return ret.
*******************************************************************************************/
int write_in(int fd, int times)
{
	int i,ret,count=0;
	int data;
	switch(times)
	{
		case 1:
			fd = open("orderArray.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR );
			
			
			for(i=0;i<LEN;i++)
			{
				if(check_bit_1(i))
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
						printf("write 1 error\n");
						goto out;
					}
		
				}//end of check_bit_1()
		
			}
			
			break;
	
		case 2:
			fd = open("orderArray.txt", O_RDWR, S_IRUSR | S_IWUSR );
			
			
			for(i=0;i<LEN;i++)
			{
				if(check_bit_1(i))
				{
					count++;
					if(count==1)
					{
						ret = lseek(fd,0,SEEK_END);//write at the end of the file, don't cover the file.
						if(ret<0)
						{
							printf("lseek error\n");
							goto out;
						}
					}
					else
					{
						ret = lseek(fd,0,SEEK_CUR);//write at the end of the file, don't cover the file.
						if(ret<0)
						{
							printf("lseek error\n");
							goto out;
						}
					}

					data = i+5000000;
		  			ret = write(fd,&data,4);
					if(ret<0)
					{
						printf("write 2 error\n");
						goto out;
					}
		
				}//end of check_bit_1(i)
			}//end of for()
	
			break;
	
	
		default:
			break;
		}
out: 
	close(fd);
	return ret;

}


/*****************************************************************************************
***	Description:		
***		 read data from randArray.txt in two times
***	
***	Parameters:
*** 		i: for loop variable
*** 		data: temp value
***			ret: return value
***		
***
***	Return Value:
***		Return ret.
*******************************************************************************************/
int readOrder(int fd,int times)
{
	int i,ret;
	int data;

	
	
	fd = open("randArray.txt", O_RDWR, S_IRUSR | S_IWUSR );
	if(fd<0)
	{
		printf("open read first error\n");
		ret = fd;
		goto out;
	}

	for(i=0;i<LEN;i++)
	{
		ret = lseek(fd,0,SEEK_CUR);
		if(ret < 0)
		{
			printf("lseek error\n");
			goto out;
		}

		ret = read(fd,&data,4);
		if(ret<0)
		{
			printf(" read first error\n");
			ret = fd;
			goto out;
		}
		
		switch(times)
		{
			case 1:	
				if(data<5000000)
					set_bit_1(data);
				break;
		
			case 2:
				if(data>=5000000)
				{
					data -= 5000000; 
					set_bit_1(data);
				
				}
				break;

			default:
				break;
		}

	}//end of for

out:	close(fd);
	return ret;
	

}
/*****************************************************************************************
***	Description:		
***		 the main function, read random numbers and write into randArray.txt in two times, then
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
	int i,fd,ret;
	int test_read[1000];

	start = clock();

	for(i=0;i<(N/64+1)*32;i++)
		set_bit_0(i);

	readOrder(fd,1);
	write_in(fd,1);
	
	for(i=0;i<(N/64+1)*32;i++)
		set_bit_0(i);
	readOrder(fd,2);
	write_in(fd,2);

	
	/*
	*read some data from orderArray.txt to verify
	*/
	fd = open("orderArray.txt",O_RDWR);
	if(fd<0)
	{
		printf("open 2 error\n");
		ret = fd;
		goto out;
	}

	
	for(i=0;i<800;i++)
	{
		ret = lseek(fd,0,SEEK_CUR);
		if(ret < 0)
		{
			printf("lseek error\n");
			goto out;
		}

			

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



	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("Totally costs %f seconds\n",duration);

out:	
	close(fd);
	
	return ret;

}

