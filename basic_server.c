#include "pipe_networking.h"

void sighand(int sig){

  if (sig == SIGINT){
    remove("well_known_pipe");
    exit(0);
  }

}
char *  rot13(char * str){
  for(int i = 0; str[i]; i++){
    if( str[i] >= 'a' && str[i] < 'n')
      str[i] += 13;       
    else if( str[i] >= 'n' && str[i] <= 'z')
      str[i] -= 13;
  }
  return str;
}

int main() {
  signal(SIGINT,sighand);

  int to_client;
  int from_client;

  char * input = malloc(BUFFER_SIZE);
  char * output = malloc(BUFFER_SIZE);

  while(1){
    from_client = server_handshake( &to_client );
    printf("This server adds 13 to char (not rot13)\n");
    while(read(from_client,input,BUFFER_SIZE)){
      input[strlen(input)] = '\0';
      printf("[server]-Client Input: %s\n",input);
      output = rot13(input);
      printf("[server]-Sending [%s] to client\n",output);
      write(to_client,output,BUFFER_SIZE);

    }

  }
  return 0;
}

