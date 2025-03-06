#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>

int main() {

  // Declare stuff
  int socket_fd, error;
  struct sockaddr_in addr;
  socklen_t socket_length = sizeof(addr);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(0);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  char buf[100];

  // Create socket
  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  error = bind(socket_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
  if (error < 0) {
    printf("Error! Exiting.");
    return -1;
  }
  error = getsockname(socket_fd, (struct sockaddr *)&addr, &socket_length);
  if (error < 0) {
    printf("Error! Exiting.");
    return -1;
  }
  printf("\nReceiver is using port %d.\n", ntohs(addr.sin_port));

  // Wait for incoming message
  recvfrom(socket_fd, &buf, sizeof(buf), 0, (struct sockaddr *)&addr,
           &socket_length);
  printf("Message received: %s\n", buf);
  close(socket_fd);
  return 0;
}