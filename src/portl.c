/*
AUTHOR:  Garbaz
E-MAIL:  garbaz@t-online.de
PROJECT: A simple piece of software to [listen for an incoming connection on a port and] read data coming in.
LICENSE: 

	The MIT License (MIT)
	
	Copyright (c) 2016 
	
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

/*
TODO:
- Hex mode
- table mode
*/
#include "netlib.h"
#include "defaults.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define PRNT_VERB(...) if(verbose){printf(__VA_ARGS__);};
#define PRNT_ERR(...) if(!verbose && delta_bytes > BUFFER_SIZE){fprintf(stderr, __VA_ARGS__);delta_bytes = 0;};
#define BACKLOG 1
#define BUFFER_SIZE 4096

char port_buffer[6], recv_buffer[BUFFER_SIZE];
char tcp, verbose, persistent;
int targetfd, hostfd, bytes_recvd, delta_bytes;
unsigned long overall_bytes_recvd;
struct sigaction sa;
struct sockaddr_storage sender;
socklen_t senderlen;

void handle_args(int argc, char* argv[]);
void print_help(char* argv0);

void interrupt_handler(int sig)
{
	if(tcp)
	{
		tdisconnect(targetfd);
	}
	exit(0);
}

int main(int argc, char* argv[])
{
	tcp = 1;
	verbose = 0;
	persistent = 0;
	senderlen = sizeof(struct sockaddr);
	
	handle_args(argc, argv);
	
	sa.sa_handler = interrupt_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	
	if (sigaction(SIGINT, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	
	
	if(tcp)
	{
		PRNT_VERB("Setting up host on port %s...\n", port_buffer);
			if((hostfd = tcreate_host(port_buffer)) < 0)
			{
				fprintf(stderr, "ERROR (%d): ", hostfd);
				switch(hostfd)
				{
					case -1:
						fprintf(stderr, "Unable to resolve address");
						break;
					case -2:
						fprintf(stderr, "Unable to set up UNIX files descriptor");
						break;
					case -3:
						fprintf(stderr, "Unable to bind to port");
						break;
					case -4:
						fprintf(stderr, "Unable to force bind to port (Enable reuse of port)");
						break;
					default:
						fprintf(stderr, "Unknown error");
				}
				fprintf(stderr, "\n");
				return 2;
			}
			PRNT_VERB("done!\n");
	}
	
	do
	{
		if(tcp)
		{
			PRNT_VERB("Waiting for incoming connection on port %s\n", port_buffer);
			if((targetfd = tlisten_accept(hostfd, BACKLOG)) < 0)
			{
				fprintf(stderr, "ERROR (%d): ", targetfd);
				switch(targetfd)
				{
					case -1:
						fprintf(stderr, "Unable to listen for incoming connection");
						break;
					case -2:
						fprintf(stderr, "Unable to accept incoming connection");
						break;
					default:
						fprintf(stderr, "Unknown error");
				}
				fprintf(stderr, "\n");
				return 3;
			}
			PRNT_VERB("done!\n");
			
			PRNT_VERB("Listening on port %s...\n\n", port_buffer);
			setbuf(stdout, NULL);
			setbuf(stderr, NULL);
			while(1)
			{
				
				if((bytes_recvd = recv(targetfd, recv_buffer, BUFFER_SIZE, 0)) < 1)
				{
					break;
				}
				PRNT_VERB("Recieved %d bytes\n\n", bytes_recvd);
				for(int i = 0; i < bytes_recvd; i++)
				{
					putchar(recv_buffer[i]);
				}
				overall_bytes_recvd += bytes_recvd;
				delta_bytes += bytes_recvd;
				PRNT_ERR("\r%lu bytes recieved...", overall_bytes_recvd);
			}
			PRNT_ERR("\r%lu bytes recieved.  \n", overall_bytes_recvd);
			PRNT_VERB("\n\ndone!\n");
			tdisconnect(targetfd);
		}
		else
		{
			PRNT_VERB("Setting up host on port %s...\n", port_buffer);
			if((hostfd = ucreate_host(port_buffer)) < 0)
			{
				fprintf(stderr, "ERROR (%d): ", hostfd);
				switch(hostfd)
				{
					case -1:
						fprintf(stderr, "Unable to resolve address");
						break;
					case -2:
						fprintf(stderr, "Unable to set up UNIX files descriptor");
						break;
					case -3:
						fprintf(stderr, "Unable to bind to port");
						break;
					case -4:
						fprintf(stderr, "Unable to force bind to port (Enable reuse of port)");
						break;
					default:
						fprintf(stderr, "Unknown error");
				}
				fprintf(stderr, "\n");
				return 2;
			}
			PRNT_VERB("done!\n");
			
			PRNT_VERB("Listening on port %s...\n\n", port_buffer);
			setbuf(stdout, NULL);
			setbuf(stderr, NULL);
			while(1)
			{
				if((bytes_recvd = recvfrom(hostfd, recv_buffer, BUFFER_SIZE, 0, (struct sockaddr *)&sender, &senderlen)) < 1)
				{
					continue;
				}
				for(int i = 0; i < bytes_recvd; i++)
				{
					putchar(recv_buffer[i]);
				}
				overall_bytes_recvd += bytes_recvd;
				delta_bytes += bytes_recvd;
				PRNT_ERR("\r%lu bytes recieved...", overall_bytes_recvd);
			}
			PRNT_ERR("\r%lu bytes recieved.  \n", overall_bytes_recvd);
			PRNT_VERB("\n\ndone!\n");
		}
	}while(persistent);
	return 0;

}

void handle_args(int argc, char* argv[])
{
	char got_port = 0;
	if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
			{
				print_help(argv[0]);
				exit(0);
			}
			else if(strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--tcp") == 0)
			{
				tcp = 1;
			}
			else if(strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--udp") == 0)
			{
				tcp = 0;
			}
			else if(strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
			{
				verbose = 1;
			}
			else if(strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--persistent") == 0)
			{
				persistent = 1;
			}
			else
			{
				strcpy(port_buffer, argv[i]);
				got_port = 1;
			}
		}
	}
	if(!got_port)
	{
		strcpy(port_buffer, DEFAULT_PORT);
	}
}

void print_help(char* argv0)
{
	fprintf(stderr,"\n ---portl---\n\n");
	fprintf(stderr,"Listens on given port for incoming connections/packets and prints bytes to STDOUT.\n\n");
	fprintf(stderr,"Synopsis:\n");
	if(isatty(fileno(stdout))) fprintf(stderr,"\e[1m");
	fprintf(stderr,"portl [OPTIONS] [PORT]\n\n");
	if(isatty(fileno(stdout))) fprintf(stderr,"\e[0m");
	fprintf(stderr,"PORT = The port on which to listen\n\n");
	fprintf(stderr,"+------------------+--------------------------------+\n");
	fprintf(stderr,"| PARAMETER        | FUNCTION                       |\n");
	fprintf(stderr,"+------------------+--------------------------------+\n");
	fprintf(stderr,"| -u, --udp        | Run in UDP mode                |\n");
	fprintf(stderr,"| -t, --tcp        | Run in TCP mode (default)      |\n");
	fprintf(stderr,"| -h, --help       | Print this help message        |\n");
	fprintf(stderr,"| -v, --verbose    | Enable debugging output        |\n");
	fprintf(stderr,"| -p, --persistent | Always wait for new connection |\n");
	fprintf(stderr,"+------------------+--------------------------------+\n");
	fprintf(stderr,"\nArguments are handled in the order they are given and the last one of a type will overwrite any previous ones of the same type!\n");
}
