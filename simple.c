#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>


#define PORT 8080
int main(int argc, const char *argv[])
{
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char *hello = "HTTP/1.1 200 OK\n Content-Type: text/plain\n Content-Length: 12\n\nHello world!";

    //create the socket file descriptor
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        perror("in socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("in bind");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 10) < 0) 
    {
        perror("in listen");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        printf("\n Waiting for connection\n\n");
        if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("in accept");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        valread = read( new_socket, buffer, 30000);
        printf("%s\n", buffer);
        write(new_socket, hello, strlen(hello));
        printf(" ====== Hello Message Sent =====");
        close(new_socket);
    }
    return 0;
}