#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "logging.h"

const unsigned short PORT = 8081;
const int BUFFER_SIZE = 40960;

int make_socket(unsigned short port)
{
    int sock;
    struct sockaddr_in name;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        set_file();
        perror("failed to establish socket\n");
        log_error("failed to establish socket\n");
        close_file();
        exit(EXIT_FAILURE);
    }

    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock, (struct sockaddr *) &name, sizeof(name)) < 0) {
        set_file();
        perror("failed to bind a name to socket\n");
        log_error("failed to bind a name to socket\n");
        close_file();
        exit(EXIT_FAILURE);
    }

    return sock;
}

int read_from_client(int filedes, char *buffer) {
    int nbytes;

    nbytes = (int) read(filedes, buffer, BUFFER_SIZE);
    if(nbytes < 0) {
        set_file();
        perror("something went wrong when reading FD\n");
        log_error("something went wrong when reading FD\n");
        close_file();
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
        set_file();
        perror("failed to listen on the socket\n");
        log_error("failed to listen on the socket\n");
        close_file();
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
            set_file();
            perror("failed when processing \"select\"\n");
            log_error("failed when processing \"select\"\n");
            close_file();
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < FD_SETSIZE; ++i) {
            if(FD_ISSET(i, &read_fd_set)) {
                if(i == sock) {
                    size = sizeof(clientname);
                    new = accept(sock, (struct sockaddr *) &clientname, (socklen_t *) &size);
                    if(new < 0) {
                        set_file();
                        perror("failed to accept new connection\n");
                        log_error("failed to accept new connection\n");
                        close_file();
                        exit(EXIT_FAILURE);
                    }
                    char *ip = inet_ntoa((&clientname)->sin_addr);
                    set_file();
                    log_info("Server: new connection established from client, client ip is %s.\n", ip);
                    close_file();
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
                            char *ip = inet_ntoa((&clientname)->sin_addr);
                            set_file();
                            perror("failed to send message back to client\n");
                            log_error("failed to send message back to client, client ip is %s\n", ip);
                            close_file();
                            exit(EXIT_FAILURE);
                        }
                        char *ip = inet_ntoa((&clientname)->sin_addr);
                        set_file();
                        log_info("New message from client ip: %s, message is: %s", ip, buffer);
                        close_file();
                    }
                }
            }
        }
    }
    return 0;
}

