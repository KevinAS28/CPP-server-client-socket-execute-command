#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <csignal>
#include <cstdlib>
#include <unistd.h>
#include <string.h>//strlen
#include <iostream>
#include <stdlib.h>
#include <sstream>

template <class any>
std::string to_string (any wow)
{
	std::string to_return;
	std::stringstream kevin;
	kevin << wow;
	kevin >> to_return;
	return to_return;
}
int finish = 0, sock = socket(AF_INET, SOCK_STREAM, 0);

char *getLine(){
	char temp, *results = (char*) calloc(sizeof(char), 1);
	int x = 0;
	while (1)
	{
		temp = getc(stdin);
		if (temp==EOF|temp=='\n')
		{
			break;
		}
		results[x] = temp;
		x++;
		results = (char*) realloc(results, (sizeof(char)*x)+1);
	}
	return results;
}

template <class any>
int checkChar(any* a0, int a1, any* a2, int a3)
{
	int terbanyak = a1? a1>= a3:a3,  yes=1;

	for (; terbanyak >=0; terbanyak--)
	{
		if (a0[terbanyak]==a2[terbanyak])
		{
			continue;
		}
		yes = 0;
		break;
	}
	return yes;
}
void stop(int sign)
{
	finish = 1;
	try{
		close(sock);
	}catch (...)
	{

	}
	printf("Stop!\n");
	exit(0);
}

char buff[256] = "";

char* receiver(int the_sock, int ender='\0')
{
	char *to_return = (char*) calloc(sizeof(char), 100), temp[1];
	read(the_sock, to_return, 100);

	// int tanda=0;
	// while (1)
	// {
	// 	read(the_sock, temp, 1);
	// 	if ((int)temp[0]==ender){break;}
	// 	to_return[tanda] = temp[0];
	// 	tanda++;
	// 	to_return = (char*)	realloc(to_return, sizeof(char) * (tanda+1));
	// }
	return to_return;
	

}
void sender(int the_sock, char *str, int lenstr)
{
	char temp[1];
	for (int i = 0; i < lenstr; i++)
	{
		temp[0] = str[i];
		write(the_sock,  temp, 1);
	}
}


void Client_Handler(int new_sock, sockaddr_in client)
{
	printf("Connection!!\n");
	char ip[15];
	int port;
	inet_ntop(AF_INET, &client.sin_addr, ip, sizeof(ip));
	port = ntohs(client.sin_port);
	printf("Receive connection from %s:%d\n", ip, port);

	//recv(new_sock, buff, sizeof(buff), 0);
	char *yay = receiver(new_sock);
	printf("%s\n", yay);
	free(yay);

	printf("command: ");
	char *command = getLine();
	printf("\n%s\n", command);
	//char *command1 = getLine();

	write(new_sock, command, 100);
	//sender(new_sock, command, 100);
	//sender(new_sock, command1, strlen(command1));
	close(new_sock);

	return;	

}

int main()
{
	sockaddr_in server_addr;
	int port = 9231;
	int opt = 1;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_family = AF_INET;
	setsockopt(sock, SOL_SOCKET,SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(sock, (struct sockaddr*) &server_addr, sizeof(server_addr))<0)
		{printf("cannot bind\n");
	::stop(10);}
	listen(sock, 5);
	signal(SIGINT, ::stop);

	sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	printf("Listening....\n");
	
	while (!(::finish))
	{
		int newsock = accept(sock, (struct sockaddr *) &client_addr, &client_len);		
		std::thread obj(::Client_Handler, newsock, client_addr);
		obj.join();
	}
	return 0;
}
