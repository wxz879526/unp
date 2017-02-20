#include "unp.h"

void str_cli(FILE *fp, int sockfd)
{
	char sendLine[MAXLINE] = { 0 };
	char recvLine[MAXLINE] = { 0 };
	while (Fgets(sendLine, MAXLINE, fp) != NULL)
	{
		Writen(sockfd, sendLine, strlen(sendLine));

		if (Readline(sockfd, recvLine, MAXLINE) == 0)
		{
			err_quit("str_cli: server terminated prematurely");

		}

		fputs(recvLine, stdout);
	}
}


int main(int argc, char** argv)
{
	int connectfd = Socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in srvAddr;
	bzero(&srvAddr, sizeof(srvAddr));
	srvAddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &srvAddr.sin_addr.s_addr);
	srvAddr.sin_port = SERV_PORT;

	Connect(connectfd, (SA*)&srvAddr, sizeof(srvAddr));

	str_cli(stdin, connectfd);

	return 0;
}