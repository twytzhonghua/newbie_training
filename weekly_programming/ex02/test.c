/**********************************************************************************************
*** Filename:
***			test.c
***	Copyright (c) 2013 Wistron
***	All rights reserved.
***
***	Description:
***		 Get how many ones in an integer's binary expression.
***
***	Author/Creat Date:
*** 		Albert Yang, Apri 12,13
***
***	Note:
***			Input an integer, then you will get the result.
***
************************************************************************************************/


#include <stdio.h>


/*****************************************************************************************
***	Description:		
***		 Get how many 1s in an integer's binary expression.
***	
***	Parameters:
***		i: 	the integer which you will input
***		test:	a data for test
***
***	Return Value:
***		Return count when this function is over.
*******************************************************************************************/
int count_bit_one(int i)
{
	int count = 0;
	unsigned int test = 1;

	while(test)
	{
		if(i & test)
		{
			count++;
		}
		
		test = test << 1;
	}
	
	return count;

}


/*****************************************************************************************
***	Description:		
***		the main function, call the subfunction and print the result.	
***	
***	Parameters:
***		number: the number you will input.
***     cnt  : the subfunction returns value
***
******************************************************************************************/

void main(void)
{

	int number,cnt;
	printf("Please input an integer:");
	scanf("%d",&number);
	
	cnt = count_bit_one(number);

	printf("There are total %d ones in this integer's binary expression.\n",cnt);

}

