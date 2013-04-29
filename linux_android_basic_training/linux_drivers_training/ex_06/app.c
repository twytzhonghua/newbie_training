/*******************************************************************************
*** Filename:
***			app.c
***	Copyright (c) 2013 Wistron
***	All rights reserved.
***
***	Description:
***		a simple test app.
***
***	Author/Creat Date:
*** 		Albert Yang, Apri 29,13
***
***	Note:
*** 	
****************************************************************************/


#include <stdio.h>   /*For the printf/scanf ...*/ 
#include <stdlib.h>    /*For exit()*/
#include <sys/types.h>/*For size_t/....*/
#include <sys/stat.h>/*For some documents related information*/
#include <fcntl.h>/*For open()/close()/read()....*/
#include <string.h>




int main(void)
{
	int fd, ret;
	char buf[50]="just for test get time";
	

	/*open device*/
	fd = open("/dev/concur_race",O_RDWR);
	if(fd < 0 )
	{
	printf("open failed!\n");
	return -1;
	}
	else 
	printf("open successfully\n");
	
	ret = lseek(fd,0,SEEK_SET);
	


	/*
	*test write
	*/
	ret = write(fd,buf,20);
	if(ret < 0)
	{
		printf("write error \n");
		return -1;
	}

	ret = lseek(fd,0,SEEK_SET);
	
	close(fd);

	return 0;

}

