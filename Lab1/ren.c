#include <stdio.h>

int main(){
	const char *oldpath = "little dawg";
	const char *newpath = "big dawg";

	int guy = rename(oldpath, newpath);
	
	if(guy==-1){
		printf("Nonononono\n");
	}else{
		printf("You did it bud!\n");
	}
	
	
	/*
	could put
	if(guy){
		printf("GoodJOB");
	}else{
		printf("Bad job");
	}
	*/
	
	return 0;
	
}
	