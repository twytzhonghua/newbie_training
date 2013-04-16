
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
#include "cmd.h"/*For cmd in ioctl function*/


int main(void)
{
	int fd,ret;
	char buf[20];
	
	/*open device*/
	fd = open("/dev/test",O_RDWR);
	if(fd < 0 )
	{
		printf("open failed!\n");
		return -1;
	}
	else 
		printf("open successfully\n");

	/*
	*Read data from kernel
	*/
	ret = read(fd,buf,20);
	if (ret == -1)
	{
		printf("Read Failed.\n");
		exit(1);
	} 
	
	printf("Read file is [%s]\n",buf);
	
	/*
	*Write data to kernel.
	*/
	ret = write(fd,"This is what I want to write",20);
	if(ret!= 20)
	{
		printf("Error writing to the file.\n");
		exit(1);
	}

	
	ioctl(fd,HELLO,0);

	/*
	*Close device.
	*/
	close(fd);

	printf("close successfully!\n");

	return 0;
}
