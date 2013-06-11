/**********************************************************************************************
*** Filename:
***		rand_insert.c
***	Copyright (c) 2013 Wistron
***	All rights reserved.
***
***	Description:
***		生成[0, maxval]范围内m个随机整数的有序序列，不允许重复。
***		要求
***		1. 使用链表实现;
***		2. 插入随机数时, 需要按序排列和判断是否重复；
***		3. “插入函数“insert()使用非递归形式.
***	Author/Creat Date:
*** 		Albert Yang, Jun 11, 13
***
***	Modification History:
***			
***	Note:
***
************************************************************************************************/

#include <stdlib.h>/*For malloc()*/
#include <stdio.h> /*For printf() scanf() */
#include <fcntl.h>/*For open() close()*/
#include <time.h>/*For srand() rand()*/
#define MAX   5000
#define M 1000

int store_bit[1+MAX/32];

int cnt;// the lenth of the linked list

/*
*the structure 
*	Parameters:
*		num: the job number
*		next: a pointer 
*/
typedef struct node{
	int num;
	struct node *next;
}*Nodep;

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
***		 Creat first node 
***	
***	Parameters:
*** 	headp: root pointer
***		newp: the new pointer that will be created
***		prep: the pointer before newp
***
***	Return Value:
***		Return root pointer.
*******************************************************************************************/
Nodep create(Nodep headp,int num)
{

	Nodep newp,prep;
	
	/*
	*allocate space for a structure pointer
	*/
	headp = newp = prep =(Nodep)malloc(sizeof(struct node));
	if(headp == NULL)
	{
		printf("Allocate space for structure pointer failed!\n");
		return (Nodep)(-1);
	}

	newp->num = num;	

	printf("\n");

	/*
	*Creat only one node;
	*/
	if(headp == NULL)
	{
		headp = newp;
		cnt = 0;
	}
	else
		prep->next = newp;
		
	prep = newp;
	newp -> next = NULL;
	
	cnt++;//employees's number adds.
	
	printf("Create a node successfully!\n");


	return headp;
}




/*****************************************************************************************
***	Description:		
***		 Insert nodes
***	
***	Parameters:
*** 	headp: root pointer
***		newp: the new pointer that will be created
***		p1: the current pointer
***		p2: the pointer before newp
***
***	Return Value:
***		Return root pointer.
*******************************************************************************************/
Nodep insert(Nodep headp,int num)
{
	Nodep p1,p2,newp;
	/*
	*allocate space for a structure pointer
	*/
	newp = (Nodep)malloc(sizeof(struct node));
	if(newp == NULL)
	{
		printf("Allocate space for structure pointer failed!\n");
		return (Nodep)(-1);
	}

	
	newp->num = num;

    p1 = headp;	
    p2 = NULL;

    while((p1->num < newp->num) && ( p1->next != NULL))
    {
            p2 = p1;
            p1 = p1->next;       
    }
   
   
	if(p1->num < newp->num)
	{
		p1->next = newp;
		newp->next = NULL;
	}
	else
	{
		newp->next = p1;
		if(p2==NULL)
			headp = newp;
		else
			p2->next = newp;
	}
            
    cnt++;

	return headp;
	

}




/*****************************************************************************************
***	Description:		
***		 Print all the data in the linked list
*******************************************************************************************/
void print(Nodep headp)
{
	int i;
	Nodep temp;
	
	temp = headp;


	printf("There are [%d] nodes, all the data is:\n",cnt);
	while(temp  != NULL)
	{
		
		printf("num[%d]=%d  ",i++,temp->num);
		if((i+1)%6==0)
			printf("\n");
		temp = temp-> next;
	}
}

/*****************************************************************************************
***	Description:		
***		the main function, call the subfunction and print the result.	
***	
***	Parameters:
***		i: for loop
***		num: rand number
***		total: create no duplicate numbers 
***
******************************************************************************************/

void main(void)
{
	int i,num;
	static int total;	
	srand((unsigned)time(NULL));
	Nodep headp = NULL;
	
	for(i=0;i<MAX;i++)
		set_bit_0(i);

	/*
	   *create first node
	   */
	num = rand()%MAX;
	set_bit_1(num);
	total = 1;
	headp = create(headp,num);
	
	
	/*
	   *Randomly generate M numbers without repeatedly
	   */
	for(i=0;i<3*MAX;i++)
	{	
		num = rand()%MAX;
		if(!check_bit_1(num))//如果没有重复，插入
		{	
			set_bit_1(num);
			total++;	
			headp = insert(headp,num);
		}
		
		if(total>=M)
			break;
	}	
	
		
	print(headp);
	
			
}

