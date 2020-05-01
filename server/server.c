#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "mysql.c"
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<signal.h>


General g = {},g1 = {},g2 = {};
int listenfd;
int cli_count = 0;
int clients[10];

void sigint_handler(int sig)
{
	for (int i = 0; i < cli_count; ++i)
	{
		close(clients[i]);
		printf("%d is closed\n", clients[i]);
	}
	close(listenfd);
    exit(0);
}

void *server_handler(void *fd_pointer);

int main(int argc, char const *argv[])
{
	int connfd, *new_sock;
	socklen_t clilen;
	struct 	sockaddr_in cliaddr, servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1 )
	{
		perror("Could not create socket\n");
	}
	puts("Socket Created");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(8888);
	 int on = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0)
    {
        /* some error has occurred, etc */
    }
	if(bind(listenfd, (struct sockaddr *)&servaddr,sizeof(servaddr))<0)
	{
		perror("Bind Failed");
		return 1;
	}
	puts("Bind Success");
	listen(listenfd, 5);

	puts("Waiting for connections");
	clilen =sizeof(cliaddr);

	while((connfd = accept(listenfd, (struct sockaddr *)&cliaddr,&clilen)))
	{
		signal(SIGINT, sigint_handler);
		puts("Connection accepted");

		pthread_t server_thread;
		new_sock = malloc(1);
		*new_sock = connfd;
		clients[cli_count++] = connfd;
		pthread_create(&server_thread, NULL, server_handler, (void *)new_sock);
		pthread_t trans;
		pthread_create(&trans, NULL, &transaction, NULL);
	}

	if(connfd < 0)
	{
		perror("accept fail");
		return 1;
	}
	return 0;
}

void *server_handler(void *fd_pointer)
{
	int sock  = *(int *)fd_pointer;
	int read_size,write_size;
	char *message;
	static char client_message[2000];

	static int send_once = 0;
	if(send_once < 1)
	{
		send_once++;
	}

	while((read_size = recv(sock, (General *)&g1, sizeof(General), 0)) > 0)
	{
		printf("Read size %d \n", read_size);
		if(g1.command == 1){
			g2 = register_user(g1.username, g1.password, g1.visa_card_number);
			write(sock, (General *)&g2, sizeof(General));
		}
		else if(g1.command == 2){
			g2 = login_user(g1.username, g1.password);
			write(sock, (General *)&g2, sizeof(General));
		}
		else if(g1.command == 3){
			g2 = available_lots();
			g2.command = (sizeof(g2.lt)/sizeof(lot) > 0) ? (1) : (0);
			write(sock, (General *)&g2, sizeof(General));
		}
		else if(g1.command == 4){
			// printf("--->%d - %.2f - %d - %d\n", g1.bid_lot_id, g1.bid_amount, g1.bidder_client_id, g1.bidder_client_id);
			g2.command = create_bid(g1.bid_lot_id, g1.bid_amount, g1.bidder_client_id);
			for(int  i = 0; i < cli_count; i++)
				write(clients[i], (General *)&g2, sizeof(General));
		}
		else if(g1.command == 5){
			g2.command = (delete_user(g1.username))? (2) : (0);
			write(sock, (General *)&g2, sizeof(General));
		}
		else if(g1.command == 6){
			g2.command = (create_lot(g1.lt[0].min_price, g1.lt[0].title, g1.lt[0].description, g1.lt[0].owner_id))? (3) : (0);
			write(sock, (General *)&g2, sizeof(General));
		}
		else if(g1.command == 7){
			g2 = update_user(g1.client_id,g1.username, g1.password, g1.visa_card_number);
			write(sock, (General *)&g2, sizeof(General));
		}
	}

	if(read_size == 0){
		puts("Client Disconnected\n\n\n");
	}
	else if(read_size == -1){
		perror("recv failed");
	}

	free(fd_pointer);
}