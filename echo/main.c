#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

const unsigned short PORT = 8089;
const int BUFFER_SIZE = 40960;

int make_socket(unsigned short port)
{
    int sock;
    struct sockaddr_in name;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("failed to establish socket");
        exit(EXIT_FAILURE);
    }

    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock, (struct sockaddr *) &name, sizeof(name)) < 0) {
        perror("failed to bind a name to socket");
        exit(EXIT_FAILURE);
    }

    return sock;
}

int read_from_client(int filedes, char *buffer) {
    int nbytes;

    nbytes = (int) read(filedes, buffer, BUFFER_SIZE);
    if(nbytes < 0) {
        perror("something went wrong when reading FD");
        exit(EXIT_FAILURE);
    }
    else if(nbytes == 0) {
        /* End-of-file. */
        return -1;
    }
    else {
        return nbytes;
    }
}

int main() {
    int sock;
    fd_set active_fd_set, read_fd_set;
    sock = make_socket(PORT);
    if(listen(sock, SOMAXCONN) < 0) {
        perror("failed to listen on the socket");
        exit(EXIT_FAILURE);
    }
    FD_ZERO(&active_fd_set);
    FD_SET(sock, &active_fd_set);

    int size, new, nbytes;
    struct sockaddr_in clientname;
    while(1) {
        /* Block until input arrives on one or more active sockets. */
        read_fd_set = active_fd_set;
        if(select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
            perror("failed when processing \"select\"");
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < FD_SETSIZE; ++i) {
            if(FD_ISSET(i, &read_fd_set)) {
                if(i == sock) {
                    size = sizeof(clientname);
                    new = accept(sock, (struct sockaddr *) &clientname, (socklen_t *) &size);
                    if(new < 0) {
                        perror("failed to accept new connection");
                        exit(EXIT_FAILURE);
                    }
                    printf("Server: new connection established from a host.\n");
                    FD_SET(new, &active_fd_set);
                } else {
                    /* Data arriving on an already-connected socket. */
                    char buffer[BUFFER_SIZE];
                    nbytes = read_from_client(i, buffer);
                    if(nbytes < 0) {
                        close(i);
                        FD_CLR(i, &active_fd_set);
                    } else {
                        if(send(i, buffer, (size_t) nbytes, 0) != nbytes) {
                            perror("failed to send message back");
                            exit(EXIT_FAILURE);
                        }
                        printf("%s", buffer);
                    }
                }
            }
        }
    }
    return 0;
}

