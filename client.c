#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


void error (char *msg)
{
	perror(msg);
	exit(0);
}


int main (int argc, char *argv[])
{

	int sockfd, portno, n, ch, i = 0;
	char buffer[256];
	struct sockaddr_in serv_addr;
	struct hostent *server;

	if(argc < 3){
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}
	
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
		error("ERROR opening socket");

	server = gethostbyname(argv[1]);
	
	if(server == NULL){
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);	
	}



	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR connecting");
	

	
	for(i; i < 3; i++){
	
	while(1){
		printf("Welcome to the Computer Science Department\n\nLogin: ");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);
		
 		if(strlen(buffer) - 1 <= 8)
			break;
		bzero(buffer, 256);
		printf("Username must be 8 characters or less. Try again\n\n");
	}

		n = write (sockfd, buffer, strlen(buffer));  //writes username to buffer
		if(n < 0)
			error("ERROR writing to socket");

		bzero(buffer, 256);
		n = read(sockfd, buffer, 255); // reads ack from server

		if(n < 0)
			error("ERROR reading from soket");

		printf("%s\n", buffer);


		printf("Password: ");
	
		
		bzero(buffer,256);
		fgets(buffer, 255, stdin);
		
		n = write (sockfd, buffer, strlen(buffer)); // writes password to buffer
		if(n < 0)
			error("ERROR writing to socket");

		bzero(buffer, 256);
		n = read(sockfd, buffer, 255); // reads pass/fail message

		if(n < 0)
			error("ERROR reading from soket");
		printf("%s\n", buffer);

		bzero(buffer,256);
		n = write (sockfd, "Attempts", 10); //writes attemts to buffer
		if(n < 0)
			error("ERROR writing to socket");
	

		bzero(buffer, 256);
		n = read(sockfd, buffer, 255); //reads 1 or 0 from server

		if(n < 0)
			error("ERROR reading from soket");

	
		
		bzero(buffer,256);	
		n = write (sockfd, "Goodbye", 10); // writes goodbye
		if(n < 0)
			error("ERROR writing to socket");
	
	
	}	

	return 0;



}
