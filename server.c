#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

void error (char *msg)
{
	perror(msg);
	exit(1);
}

int main (int argc, char *argv[])
{
	int sockfd, newsockfd, portno, clilen, pid, valid = 2, i = 0, attempt = 3;
	char buffer[256];
	
	struct sockaddr_in serv_addr, cli_addr;
	int n, x;

	char username[10];
	char password[10];
	char inputuser[10];
	char inputpass[10];
	char line[256];
	char * token;

	if(argc < 2){
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
		error("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0)
		error("ERROR on binding");

	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	


	while(1){
	
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
		if(newsockfd < 0)
			error("ERROR on accept");

		pid = fork();
		if(pid < 0)
			error("ERROR creating process");

		if(pid == 0){
			close(sockfd);
			do{
				bzero(buffer, 256);
				n = read(newsockfd, buffer, 255); // reads username from buffer
				if(n < 0) error("ERROR reading from socket");
			


			
				printf("inputuser: %s\n", buffer);
				strcpy(inputuser, buffer);
				inputuser[strlen(inputuser)-1] = '\0';

				n = write(newsockfd, " ", 1);	//writes a space to ack client
				if(n < 0) error("ERROR writing to socket");

				bzero(buffer, 256);
				n = read(newsockfd, buffer, 255); // reads password from buffer
				if(n < 0) error("ERROR reading from socket");

				printf("inputpass: %s\n", buffer);
				strcpy(inputpass, buffer);
				inputpass[strlen(inputpass)-1] = '\0';
			

			
			
				FILE * fp;
				fp = fopen("passwd.txt", "r");
			
				for(i; i < 5; i++){
					valid = 2;
				
					fgets(line, sizeof(line), fp);
			
					token = strtok(line, ",");
				

					strcpy(username, token);
					printf("username: %s\n", username);
			
				

					if( strcmp(inputuser, username) == 0)
						valid= valid -1;
					printf("Valid = %d\n", valid);

					token = strtok(NULL, ",");
					strcpy(password, token);
					printf("Password: %s\n", password);
			
					if( strcmp(inputpass, password) == 0)
						valid = valid -1;
					printf("Valid = %d\n", valid);

				
					if (valid == 0) 
						break;	
				}

				fclose(fp);
				i = 0;

				if(valid == 0){
					n = write(newsockfd, "Welcome to CPSC445-Comp Networking Class\nYou are invited to use Your Own Name Machine", 110);
					attempt = 0;
					break; 
				} else {
					n = write(newsockfd, "Only for ESU CPSC Students taking CPSC445\n You are not invited yet\n\n", 110);
				}//writes pass or fail message

				bzero(buffer, 256);
				n = read(newsockfd, buffer, 255); //reads "attempts"
				if(n < 0) error("ERROR reading from socket");
					

				if(attempt == 0){
					n = write(newsockfd, "1", 5);
					if(n < 0) error("ERROR writing to socket");
				
				} else {
					attempt = attempt - 1;
					n = write(newsockfd, "0", 5);
					if(n < 0) error("ERROR writing to socket");
				}//writes 1 or 0

				bzero(buffer, 256);
				n = read(newsockfd, buffer, 255); //reads "goodbye"
				if(n < 0) error("ERROR reading from socket");
					


			}while(attempt > 0);
			

			close(newsockfd);
			exit(1);

		} else {
			close(newsockfd);
		}

	}

	return 0;
}
