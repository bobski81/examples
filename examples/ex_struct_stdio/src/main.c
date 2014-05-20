#include <stdio.h>
#include "message.h"
#include "main.h"

int main()
{	
	int i;
	char tempBuff[SIZE];
	Person newPerson;
	Person *personPtr = &newPerson;

	for (i=0;i<SIZE;i++){
		personPtr->name[i]=0;
		personPtr->year[i]=0;
		tempBuff[i]=0;
	}

	printf("Please Enter your name: ");
	if (message(personPtr->name,SIZE) != 1)
		printf("Error with name entry");

	printf("Please Enter your year: ");
	if (message(personPtr->year,SIZE) != 1)
		printf("Error with year entry");

	printf("How many eyes do you have? ");
	if (message(tempBuff,SIZE) != 1)
		printf("Error with noEyes entry");
	sscanf(tempBuff,"%d", &personPtr->face.noEyes);
	

	printf("Your name is ");
	for (i=0;i<SIZE;i++)
		printf("%c",personPtr->name[i]);	// Prints out name

	printf("and you selected the year ");
	for (i=0;i<SIZE;i++)
		printf("%c",personPtr->year[i]);	// Prints out year

	printf("also, you have %d eyes\n", personPtr->face.noEyes);	// Prints out no. eyes

	return(0);
}
