// This project is an example of accessing reading and writing
// a file, using the std input/output to pass in out text
// To check for any memory leaks, run "valgrind ./EX_FILE_RW"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>	// Open()
#include <unistd.h>	// Close()
#include <stdlib.h>	// Malloc()

#define BUFF_SIZE 100
#define FILENAME "rw_file.txt"

int main(){
	
	// Temp character buffer
	char tempBuff[BUFF_SIZE];
	char *tempBuffptr;

	int fd;		// File descriptor
	int fa; 	// File access flag
	
	off_t curpos;	// Current File Position
	int len;	// End of File Position

	char *readBuffptr;	// Pointer to the buffer for reading file, created by malloc
	int   readBuffsize;	// Size of the read buffer
	ssize_t ret;		// Flag returned by read() function
	
	int i;

	for (i=0; i<BUFF_SIZE; i++)
		tempBuff[i] = 0;

	// Assign buffer pointer to start of char buffer
	tempBuffptr = &tempBuff[0];


	/*##### CREATE FILE AND STORE TEXT TO IT ######*/
	// Prompt user to enter some text
	printf("Input some text here:");
	
	// Fetch the text from std input and store at buffer pointer location
	fgets(tempBuffptr, BUFF_SIZE, stdin);

	// Open a file to store the text, write only. Create the file if it does not exist
	// with owner having read/write access
	fd = creat(FILENAME, S_IRUSR | S_IWUSR);
	//same as 'fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);'
	if (fd==-1)
		printf("File open error");

	// Write the std in to the file
	fa = write(fd, tempBuffptr, BUFF_SIZE);
	if (fa==-1)
		printf("File write error");

	// Close the file
	if (close (fd) == -1)	
		printf("Could not close file");



	/*#### APPEND TEXT TO AN EXISTING FILE ####*/
	// Prompt user for more text
	printf("Please enter some more text:");

	// Fetch the text from std input and store at buffer pointer location
	fgets(tempBuffptr, BUFF_SIZE, stdin);
	
	// Open the file in append, write only
	fd = open (FILENAME, O_WRONLY | O_APPEND);
	if (fd == -1)
		printf("File open error");

	// Append the std in to the file
	fa = write (fd, tempBuffptr, BUFF_SIZE);
	if (fa == -1)
		printf("File write error");

	// Close the file
	if (close (fd) == -1)	
		printf("Could not close file");



	/*#### READ ALL THE CONTENTS OF AN EXISTING FILE AND PRINT ###*/
	// Open the file for reading
	fd = open (FILENAME, O_RDONLY);

	// Seek the end of the file to determine it's length
	curpos = lseek (fd, 0, SEEK_CUR);		// Get current file position
	if (curpos == (off_t) -1)
		printf("Error with lseek, seeking current position");

	len = lseek (fd, 0, SEEK_END);		// Seek end of file position
	if (curpos == (off_t) -1)
		printf("Error with lseek, seeking end of file position");

	curpos = lseek (fd, (off_t) curpos, SEEK_SET);	// restore file position back to original position
	if (curpos == (off_t) -1)
		printf("Error with lseek, restoring current position");

	// Dynamically create memory based on the files size
	readBuffptr = (char *) malloc(len+1);
	readBuffsize = len;

	printf("End position is: %d\n",readBuffsize);	

	// Read the entire contents of the file	
	while (len != 0 && (ret = read (fd, readBuffptr, len)) !=0 ){
		if (ret == -1) {
			//Leaving error reporting until I fully understand
			//if (errno == EINTR)
			//	continue;
			//perror ("read");
			printf("Error reading file");
			break;
		}
		
		len -= ret;
		readBuffptr += ret;
	}

	// Close the file
	if (close (fd) == -1)	
		printf("Could not close file");

// Use for debugging pointer addresses
//printf("ReadBuffptr - read done: 0x%08x\n", (int) readBuffptr);

	// Print out the contents of the file				
	readBuffptr -= readBuffsize;		// Set buffer pointer back to start


	printf("Contents of the file are:");
	for (i=0; i<readBuffsize;i++){
		printf("%c",*(readBuffptr+i));	// Prints the file output
	}

	printf("\n");

	// Free the dynamically allocated memory, by malloc()
	free(readBuffptr); 

	return(0);
}
