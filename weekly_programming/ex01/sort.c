/**********************************************************************************************
*** Filename:
***			sort.c
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

#include <stdio.h>

#define N 10000000

/*Note:
*	an integer has 32 bit, such as if there are 1,2,5,7,9,,23,31,    32,37,41,47
*		I want to use 1000 0000 1000 0000 0000 0010 1010 0110 		0000 0000 0000 0000 1000 0010 0010 0001 to show, 
*		and only two integers because an integer has 32 bit, two integers can show 64 integers at most.
*	if there are N = 10000000 numbers, I can use only 1+N/32 numbers of integer to show so much data
*/
int store_bit[1+N/32];


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
void set_bit_1(int i)
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
void set_bit_0(int i)
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
int check_bit_1(int i)
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
***		the main function, call the subfunction and print the result.	
***	
***	Parameters:
***		i: an integer in the document.
***
***
******************************************************************************************/

void main(void)
{

	int i,flag=1;

	/*
	*Initialize all data
	*/
	for(i = 0; i < N; i++)
		set_bit_0(i);


	/*
	*Get the number in document, and set the location's bit 1
	*/
	while(scanf("%d",&i) != EOF)
	{

		/*
		* i = -999  to finish inputting 
		*/
		if(i == -999 )
		{
			break;
		}
		else
			set_bit_1(i);
	}
	

	/*
	*check the number, and print it sequentially.
	*/
	for(i = 0; i < N; i++)
	{
		if(check_bit_1(i))
			printf("%d \n",i);
	}
	

}

