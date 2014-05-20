// This project is an example of accessing the serial port
// and sending & receiving messages

#include <sys/types.h>
#include <sys/stat.h>
#include<sys/poll.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
#define BAUDRATE B9600
/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyS0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

#define fdsTIMEOUT 30	// 30 second timeout

volatile int STOP=FALSE; 

int main(void)
{
	int ret,res=0;
	struct termios oldtio,newtio;
	char buf[255];

	struct pollfd fds[2];

	/* 
	  Open modem device for reading and writing and not as controlling tty
	  because we don't want to get killed if linenoise sends CTRL-C.
	*/
	fds[0].fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY);		// Set file descriptor for ttyS0
	fds[0].events = POLLIN;			// Read ttyS0


	tcgetattr(fds[0].fd,&oldtio); /* save current serial port settings */
	memset(&newtio, 0, sizeof(newtio)); /* clear struct for new port settings */

	/* 
	  BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
	  CRTSCTS : output hardware flow control (only used if the cable has
		    all necessary lines. See sect. 7 of Serial-HOWTO)
	  CS8     : 8n1 (8bit,no parity,1 stopbit)
	  CLOCAL  : local connection, no modem contol
	  CREAD   : enable receiving characters
	*/
	newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	 
	/*
	  IGNPAR  : ignore bytes with parity errors
	  ICRNL   : map CR to NL (otherwise a CR input on the other computer
		    will not terminate input)
	  otherwise make device raw (no other input processing)
	*/
	newtio.c_iflag = IGNPAR | ICRNL;
	 
	/*
	 Raw output.
	*/
	newtio.c_oflag = 0;
	 
	/*
	  ICANON  : enable canonical input
	  disable all echo functionality, and don't send signals to calling program
	*/
	newtio.c_lflag = ICANON;
	 
	/* 
	  initialize all control characters 
	  default values can be found in /usr/include/termios.h, and are given
	  in the comments, but we don't need them here
	*/
	newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
	newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
	newtio.c_cc[VERASE]   = 0;     /* del */
	newtio.c_cc[VKILL]    = 0;     /* @ */
	newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
	newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
	newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
	newtio.c_cc[VSWTC]    = 0;     /* '\0' */
	newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
	newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
	newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
	newtio.c_cc[VEOL]     = 0;     /* '\0' */
	newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
	newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
	newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
	newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
	newtio.c_cc[VEOL2]    = 0;     /* '\0' */

	/* 
	 now clean the modem line and activate the settings for the port
	*/
	tcflush(fds[0].fd, TCIFLUSH);
	tcsetattr(fds[0].fd,TCSANOW,&newtio);

	/*
	  terminal settings done, now handle input
	  In this example, inputting a 'z' at the beginning of a line will 
	  exit the program.
	*/
	while (STOP==FALSE) {     /* loop until we have a terminating condition */
	/* read blocks program execution until a line terminating character is 
	    input, even if more than 255 chars are input. If the number
	    of characters read is smaller than the number of chars available,
	    subsequent reads will return the remaining chars. res will be set
	    to the actual number of characters actually read */

		ret = poll(fds, 1, fdsTIMEOUT * 1000);
		if (ret == -1){
			//perror("poll");
			printf("poll error");
			return 1;
		}
		
		if (!ret){
			printf("Timeout of %d seconds has elapsed\n",fdsTIMEOUT);
			return 0;
		}

		if (fds[0].revents & POLLIN){		// Check if fd on ttyS is available to read
			
			res = read(fds[0].fd,buf,255); 
			
			buf[res]=0;             		// set end of string, so we can printf
			
			printf(":%s:%d\n", buf, res);		// Print string at buff, and size of read()
		}

		if (buf[0]=='z') STOP=TRUE;
	}
	/* restore the old port settings */
	tcsetattr(fds[0].fd,TCSANOW,&oldtio);

	return(0);
}
