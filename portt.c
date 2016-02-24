/*
AUTHOR:  Garbaz
E-MAIL:  garbaz@t-online.de
PROJECT: A simple piece of software to [connect to a target host and] send data via the network.
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


#include "netlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BACKLOG 1
#define BUFFER_SIZE 4096

char *target_buffer, *port_buffer, read_buffer[BUFFER_SIZE];
char tcp;
int targetfd, bytes_sent, bytes_gone, bytes_read;
unsigned long overall_bytes_sent;
struct addrinfo *targetinfo;

void handle_args(int argc, char* argv[]);
void print_help(char* argv0);

int main(int argc, char* argv[])
{
	tcp = 1;
	
	handle_args(argc, argv);
	
	if(tcp)
	{
		printf("Connecing to %s on port %s...\n", target_buffer, port_buffer);
		if((targetfd = tconnect(target_buffer, port_buffer)) < 0)
		{
			fprintf(stderr, "ERROR (%d): ", targetfd);
			switch(targetfd)
			{
				case -1:
					fprintf(stderr, "Unable to resolve address");
					break;
				case -2:
					fprintf(stderr, "Unable to set up socket");
					break;
				case -3:
					fprintf(stderr, "Unable to connect to server");
					break;
				default:
					fprintf(stderr, "Unknown error");
			}
			fprintf(stderr, "\n");
			return 1;
		}
		printf("done!\n");
		
		printf("Sending...\n");
		setbuf(stdout, NULL);
		while((bytes_read = read(0, read_buffer, BUFFER_SIZE)) > 0)
		{
			bytes_gone = 0;
			while(bytes_gone < bytes_read)
			{
				if((bytes_sent = send(targetfd, read_buffer + bytes_gone, bytes_read - bytes_gone, 0)) < 1)
				{
					fprintf(stderr, "Unable to send package!\n");
					continue;
				}
				bytes_gone += bytes_sent;
			}
			overall_bytes_sent += bytes_gone;
			printf("\r%lu bytes sent...", overall_bytes_sent);
			memset(read_buffer, 0, bytes_read);
		}
		printf("\r%lu bytes sent!  \n", overall_bytes_sent);
	}
	else
	{
		printf("Setting up socket to connect to %s on port %s...\n", target_buffer, port_buffer);
		if((targetfd = usock(target_buffer, port_buffer, &targetinfo)) < 0)
		{
			fprintf(stderr, "ERROR (%d): ", targetfd);
			switch(targetfd)
			{
				case -1:
					fprintf(stderr, "Unable to resolve target address");
					break;
				case -2:
					fprintf(stderr, "Unable to set up socket");
					break;
				default:
					fprintf(stderr, "Unknown error");
			}
			fprintf(stderr, "\n");
			return 1;
		}
		printf("done!\n");
		
		printf("Sending...\n");
		setbuf(stdout, NULL);
		while((bytes_read = read(0, read_buffer, BUFFER_SIZE)) > 0)
		{
			bytes_gone = 0;
			while(bytes_gone < bytes_read)
			{
				if((bytes_sent = usend(targetfd, targetinfo, read_buffer + bytes_gone, bytes_read - bytes_gone)) < 1)
				{
					fprintf(stderr, "Unable to send package!\n");
					continue;
				}
				bytes_gone += bytes_sent;
			}
			overall_bytes_sent += bytes_gone;
			printf("\r%lu bytes sent...", overall_bytes_sent);
			memset(read_buffer, 0, bytes_read);
		}
	}
	printf("done!\n");
	printf("\r%lu bytes sent!  \n", overall_bytes_sent);
	freeaddrinfo(targetinfo);
	
	return 0;
}

void handle_args(int argc, char* argv[])
{
	char got_addr = 0, got_port = 0;
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
			else
			{
				if(got_addr == 0)
				{
					target_buffer = argv[i];
					got_addr = 1;
				}
				else
				{
					port_buffer = argv[i];
					got_port = 1;
				}
			}
		}
		if(!(got_addr && got_port))
		{
			print_help(argv[0]);
			exit(1);
		}
	}
	else
	{
		print_help(argv[0]);
		exit(1);
	}
}

void print_help(char* argv0)
{
	printf("\n ---portt---\n\n");
	printf("Sends data to given port on given host. Data is read from STDIN.\n\n");
	printf("Synopsis:\n");
	printf("%s [-u] [-t] [-h] ADDRESS PORT\n\n", argv0);
	printf("ADDRESS = The address to which to send\n");
	printf("PORT    = The port to which to send on target host\n\n");
	printf("+------------+--------------------------------+\n");
	printf("| PARAMETER  | FUNCTION                       |\n");
	printf("+------------+--------------------------------+\n");
	printf("| -h, --help | Print this help message        |\n");
	printf("| -u, --udp  | Run in UDP mode                |\n");
	printf("| -t, --tcp  | Run in TCP mode (default)      |\n");
	printf("+------------+--------------------------------+\n");
	printf("\nArguments are handled in the order they are given and the last one of a type will overwrite any previous ones of the same type!\n");
	printf("\nData will be read from stdin.\n");
}
