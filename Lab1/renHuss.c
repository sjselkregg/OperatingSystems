
/*
	ren.c
	Last Modified: Feb. 11, 2025
	Purpose: Demonstrate use of system calls 
	To Compile: gcc ren.c
*/
#include <stdio.h>

int main()
{
	printf("Renaming test1 to test2:");
	int r = rename ("test1", "test2");
	printf("Renamed successfully\n");
	return 0;
}