/**********************************************************************************************
*** Filename:
***		Linked_list.c
***	Copyright (c) 2013 Wistron
***	All rights reserved.
***
***	Description:
***		Creat a linked list, you can insert and delete nodes, print the list.
***	Author/Creat Date:
*** 		Albert Yang, Apri 18, 13
***
***	Modification History:
***			April 19, 13
***	Note:
***
************************************************************************************************/

#include <stdlib.h>/*For malloc()*/
#include <stdio.h> /*For printf() scanf() */
#include <string.h>/*For strcpy()*/



int cnt;// the lenth of the linked list

/*
*the structure 
*	Parameters:
*		num: the job number
*		name: the name of the employee
*		sex: the sex of the employee
*		age: emplyee's age
*		next: a pointer 
*/
typedef struct node{
	int num;
	char name[20];
	char sex[5];
	unsigned age;
	struct node *next;
}*Nodep;

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
Nodep creat(Nodep headp)
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

	/*
	*get a employee's information
	*/
	printf("Input the num>>:");
	scanf("%d",&newp->num);
	printf("Input the name>>:");
	scanf("%s",newp->name);
	printf("Input the sex>>:");
	scanf("%s",newp->sex);
	printf("Input the age>>:");
	scanf("%d",&newp->age);
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
Nodep insert(Nodep headp)
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

	/*
	*get a employee's information
	*/
	printf("Input the num>>:");
	scanf("%d",&newp->num);
	printf("Input the name>>:");
	scanf("%s",newp->name);
	printf("Input the sex>>:");
	scanf("%s",newp->sex);
	printf("Input the age>>:");
	scanf("%d",&newp->age);
	printf("\n");

    p1 = headp;	
    p2 = NULL;

    while((p1->num < newp->num) && ( p1->next != NULL))
    {
            p2 = p1;
            p1 = p1->next;       
    }
   
   	/*
	*If insert at first of list, or in the middle
	*/ 
    if(p1->next != NULL)
    {
            newp->next = p1;
            if(p2 == NULL)
            {
                    headp = newp;        
            }
            else
                    p2->next = newp;
    }
    else //insert node at the end of the list
    {
            p1->next = newp;
            newp->next = NULL;
    }
            
    cnt++;//employees's number adds.

	return headp;
	

}


/*****************************************************************************************
***	Description:		
***		 Delete one node 
***	
***	Parameters:
*** 	headp: root pointer
***		temp:  a middle pointer
***		p   : the pointer before temp
*** 	del_num: the num will be deleted
***
***	Return Value:
***		Return root pointer.
*******************************************************************************************/
Nodep del(Nodep headp)
{
	int del_num;
	Nodep temp, p;
	temp = headp;

	printf("Input del_num>>:");
	scanf("%d",&del_num);

	if(headp == NULL)//check the list is null or not
	{	printf("List is null!\n");
		cnt = 0;
		return headp;
	}
	else
	{	
		temp = headp;
		while((temp->num != del_num)&&(temp->next != NULL))
		{
			p = temp;
			temp = temp->next;
		}
	
		/*
		*If find the num, delete it.
		*/
		if(del_num == temp->num)
		{
			if(temp == headp)
			{
				printf("Delete num:%d\n",temp->num);
				headp = headp->next;
				cnt--;
				free(temp);
			}
			else
			{
				printf("Delete num:%d \n",temp->num);
				p->next = temp->next;
				cnt --;
	
				free(temp);
			}
		}
		else
			printf("Input error\n");
	}
	return headp;
}


/*****************************************************************************************
***	Description:		
***		 Print all the data in the linked list
*******************************************************************************************/
void print(Nodep headp)
{
	Nodep temp;
	
	temp = headp;


	printf("There are [%d] nodes, all the data is:\n",cnt);
	while(temp  != NULL)
	{
		printf("ID:%d name:%s sex:%s age:%d\n",temp->num,temp->name,temp->sex,temp->age);
		temp = temp-> next;
	}
}

/*****************************************************************************************
***	Description:		
***		the main function, call the subfunction and print the result.	
***	
***	Parameters:
***		cmd: the commands you want to handle
***		headp: root pointer
***		ins_flag: Input "i", the value become 1 only once.
***		del_flag: Input "i", the value become 1 only once.
***		prt_flag: Input "i", the value become 1 only once.
***
******************************************************************************************/

void main(void)
{
	int del_flag=0,ins_flag=0,prt_flag=0;
	Nodep headp = NULL;
	char cmd[3];
	headp = creat(headp);
	print(headp);
	while(1)
	{
		printf(">>:");
		scanf("%s",cmd);	
		
		if(!strcmp(cmd,"i")) ins_flag = 1;
		else if(!strcmp(cmd,"d")) del_flag = 1;
		else if(!strcmp(cmd,"p")) prt_flag = 1;
		else if(!strcmp(cmd,"q")) break;
		else printf("cmd error\n");


		if(ins_flag)
		{
			ins_flag = 0;
			headp = insert(headp);
		}
		if(del_flag)
		{
			del_flag = 0;
			headp = del(headp);
		}
		if(prt_flag)
		{
			prt_flag = 0;
			print(headp);
		}

			
			
	}

}

