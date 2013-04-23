/*******************************************************************************
*** Filename:
***			app.c
***	Copyright (c) 2013 Wistron
***	All rights reserved.
***
***	Description:
***		test the char driver device.
***
***	Author/Creat Date:
*** 		Albert Yang, Apri 16,13
***
***	Note:
****************************************************************************/


#include <stdio.h>   /*For the printf/scanf ...*/ 
#include <stdlib.h>    /*For exit()*/
#include <sys/types.h>/*For size_t/....*/
#include <sys/ioctl.h>/*For the ioctl*/
#include <sys/stat.h>/*For some documents related information*/
#include <fcntl.h>/*For open()/close()/read()....*/
#include <string.h>


#include "cmd.h"


int main(void)
{
	int fd, ret;
	int i=1,j;
	char buf[50];
	
	printf("Input buf>>:");
	scanf("%s",buf);

	/*open device*/
	fd = open("/dev/misc_ioctl",O_RDWR);
	if(fd < 0 )
	{
	printf("open failed!\n");
	return -1;
	}
	else 
	printf("open successfully\n");
	
	ret = lseek(fd,0,SEEK_SET);
	
	/*
	*check ioctl_write successfully or not
	*/	
	ret = ioctl(fd,WRITE_IN,buf);
	if(ret < 0)
	{
		printf("ioctl error \n");
		return -1;
	}

	ret = lseek(fd,0,SEEK_SET);
	
	/*
	*check read succesffuly or not 
	*/
	ret = read(fd,buf,50);
	if(ret<0)
	{
		printf("Read failed!\n");
		return -1;
	}
	printf("Read buf is:%s\n",buf);

	
	ret = lseek(fd,0,SEEK_SET);

	/*
	*check ioctl_clear successfully or not
	*/	
	ret = ioctl(fd,CLEAR);
	if(ret < 0)
	{
		printf("ioctl error \n");
		return -1;
	}

	

	ret = lseek(fd,0,SEEK_SET);
	/*
	*check read succesffuly or not 
	*/
	ret = read(fd,buf,20);
	if(ret<0)
	{
		printf("Read failed!\n");
		return -1;
	}
	printf("Read buf is:%s\n",buf);
	close(fd);
	

	return 0;

}

