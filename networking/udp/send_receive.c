#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int sockfd, error;
  struct sockaddr_in addr;
  char buf[100];
  socklen_t length = sizeof(addr);

  // Create sock for network communication
  addr.sin_family = AF_INET;
  addr.sin_port = htons(0);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    printf("Error creating socket. Exiting.\n");
    return -1;
  }
  error = bind(sockfd, (struct sockaddr *)&addr, length);
  if (error < 0) {
    printf("Error binding. Exiting.\n");
    return -1;
  }
  error = getsockname(sockfd, (struct sockaddr *)&addr, &length);
  printf("Local UDP port: %d\n", ntohs(addr.sin_port));

  // Configure remote endpoint
  printf("Enter IP of remote endpoint (default: localhost): ");
  fgets(buf, sizeof(buf), stdin);
  if (buf[0] == '\n') // If the user just pressed Enter, set localhost
    strcpy(buf, "127.0.0.1");
  addr.sin_addr.s_addr = inet_addr(buf);
  printf("Set remote IP: %s\n",
         inet_ntoa(*(struct in_addr *)&addr.sin_addr.s_addr));
  printf("Enter port of endpoint: ");
  fgets(buf, sizeof(buf), stdin);
  addr.sin_port = htons(strtol(buf, NULL, 10));

  // Communicate with remote
  printf("Socket ready. Type your message. \n");
  while (1) {
    fgets(buf, sizeof(buf), stdin);
    if (strcmp(buf, "exit\n") == 0)
      break;
    error =
        sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&addr, length);
    if (error < 0)
      return -1;
    printf("Message sent: %s\n", buf);
    error = recvfrom(sockfd, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&addr,
                     &length);
    if (error > 0) {
      buf[error] = '\0'; // Null-terminate the received message
    }
    printf("Received message: %s\n", buf);
    printf("Type new message below. (Type exit to end the program).\n");
  }
  close(sockfd);

  return 0;
}