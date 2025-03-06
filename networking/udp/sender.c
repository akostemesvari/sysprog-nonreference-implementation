#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  // Declare stuff
  int sock_fd;
  struct sockaddr_in addr;
  int error;
  char message[] = "Hallo Server";
  addr.sin_family = AF_INET;
  addr.sin_port = htons(57105);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // Create socket
  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (error < 0) {
    printf("Error creating socket! Exiting.");
    return -1;
  }

  // Send message
  error = sendto(sock_fd, message, sizeof(message), 0, (struct sockaddr *)&addr,
                 sizeof(addr));

  // Close socket
  close(sock_fd);
  return 0;
}