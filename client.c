#include "pipe_networking.h"

void sighand(int sig){

  if (sig == SIGINT){
    exit(0);
  }


}
int main() {

  signal(SIGINT,sighand);
  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  char * input = malloc(BUFFER_SIZE);
  char * output = malloc(BUFFER_SIZE);
  while(1){
    printf("[client]-User Input: ");
    fgets(input,BUFFER_SIZE,stdin);
    strtok(input,"\n");
    write(to_server, input, BUFFER_SIZE);
    read(from_server, output, BUFFER_SIZE);
    printf("[client]-Server Output: %s\n\n", output);
  }
}
