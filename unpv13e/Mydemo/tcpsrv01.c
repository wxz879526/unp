#include "unp.h"

void str_echo(int sockfd)
{
	ssize_t n;
	char buf[MAXLINE] = { 0 };
again:
	while ((n = read(sockfd, buf, MAXLINE)) > 0)
	{
		Writen(sockfd, buf, n);
	}

	if (n < 0 && errno == EINTR)
	{
		goto again;
	} else if (n < 0)
	{
		err_sys("str_echo: read err");
	}
}

int main()
{
	int listenSock = Socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in srvAddr;
	bzero(&srvAddr, sizeof(srvAddr));
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvAddr.sin_port = htons(SERV_PORT);
	Bind(listenSock, (SA*)&srvAddr, sizeof(srvAddr));

	void sig_chld(int);

	Listen(listenSock, LISTENQ);

	Signal(SIGCHLD, sig_chld);

	struct sockaddr_in cliAddr;
	bzero(&cliAddr, sizeof(cliAddr));
	pid_t childpid = 0;
	for ( ; ; )
	{
		socklen_t len = sizeof(cliAddr);
		int connfd = 0;
		if ((connfd = accept(listenSock, (SA*)&cliAddr, &len)) < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			else
			{
				err_sys("accept error");
			}
		}

		if ((childpid = fork()) == 0) // children process
		{
			Close(listenSock);
			str_echo(connfd);
			exit(0);
		}

		Close(connfd);
	}
}