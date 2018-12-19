#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
	printf("[server]-creating well known pipe: well_known_pipe\n");
	mkfifo("well_known_pipe", 0777);
  	int wkp = open("well_known_pipe", O_RDONLY);
	if(wkp == -1){
	  printf("[server]-error opening Well Known Pipe: %s\n", strerror(errno));
	  exit(0);
	}
	char * client = malloc(BUFFER_SIZE);
  	read(wkp, client, BUFFER_SIZE);
  	printf("[server]-received message from client: %s\n", client);
	remove("well_known_pipe");
	printf("[server]-removed well known pipe.\n");

	printf("[server]-sending message [%s] to client: %s\n",ACK,client);

	*to_client = open(client, O_WRONLY);
	if(*to_client == -1){
	  printf("[server]-error opening %s: %s\n", client, strerror(errno));
	  exit(0);
	}
	write(*to_client, ACK, BUFFER_SIZE);

	char * ack_confirm = malloc(BUFFER_SIZE);
	read(wkp,ack_confirm,BUFFER_SIZE);
	printf("[server]-recieved [%s] from client: %s.\n",ack_confirm,client);

	free(client);
	free(ack_confirm);

	return wkp;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
	printf("[client]-opening well_known_pipe\n");
	*to_server = open("well_known_pipe", O_WRONLY);
	if(*to_server == -1){
	  printf("[client]-error opening well_known_pipe: %s\n", strerror(errno));
	  exit(0);
	}
	printf("[client]-opening downstream pipe\n");

	char * pipe_name = malloc(HANDSHAKE_BUFFER_SIZE);
	sprintf(pipe_name,"%d",getpid());
  	mkfifo(pipe_name, 0777);
  	write(*to_server, pipe_name, BUFFER_SIZE);
	printf("[client]-sending downstream pipe name [%s] through well_known_pipe\n",pipe_name);

	int downstream = open(pipe_name, O_RDONLY);
	if(downstream == -1) {
	  printf("[client]-error opening downstream pipe: %s\n", strerror(errno));
	  exit(0);
	}

	char * server_ack = malloc(BUFFER_SIZE);
	read(downstream, server_ack, BUFFER_SIZE);
	printf("[client]-received message from server: %s\n", server_ack);
	free(server_ack);
	remove(pipe_name);

	printf("[client]-sending message to server: %s\n" ,ACK);
	write(*to_server, ACK, BUFFER_SIZE);
	return downstream;
}
