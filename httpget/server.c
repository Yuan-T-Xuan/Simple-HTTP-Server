#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "logging.h"

#define SERVER_ID "simplehttpserver\r\n"

const unsigned short PORT = 8080;
const int BUFFER_SIZE = 40960;

int get_line(int sock, char *buf, int size) {
    int i = 0;
    char c = '\0';
    int n;
    while ((i < size - 1) && (c != '\n')) {
        n = recv(sock, &c, 1, 0);
        /* DEBUG printf("%02X\n", c); */
        if (n > 0) {
            if (c == '\r') {
                n = recv(sock, &c, 1, MSG_PEEK);
                /* DEBUG printf("%02X\n", c); */
                if ((n > 0) && (c == '\n'))
                    recv(sock, &c, 1, 0);
                else
                    c = '\n';
            }
            buf[i] = c;
            i++;
        }
        else
            c = '\n';
    }
    buf[i] = '\0';
    return(i);
}

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

void send_header(int client) {
    char buf[1024];
    strcpy(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, SERVER_ID);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
}

void print_then_log(char* message) {
    set_file();
    perror(message);
    log_error(message);
    close_file();
}

int main() {
    int sock;
    fd_set active_fd_set, read_fd_set;
    sock = make_socket(PORT);
    if(listen(sock, SOMAXCONN) < 0) {
        print_then_log("failed to listen on the socket\n");
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
            print_then_log("failed when processing \"select\"\n");
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < FD_SETSIZE; ++i) {
            if(FD_ISSET(i, &read_fd_set)) {
                if(i == sock) {
                    size = sizeof(clientname);
                    new = accept(sock, (struct sockaddr *) &clientname, (socklen_t *) &size);
                    if(new < 0) {
                        print_then_log("failed to accept new connection\n");
                        exit(EXIT_FAILURE);
                    }
                    char *ip = inet_ntoa((&clientname)->sin_addr);
                    print_then_log("Server: new connection established from client.");
                    FD_SET(new, &active_fd_set);
                } else {
                    /* Data arriving on an already-connected socket. */
                    char buffer[BUFFER_SIZE];
                    char firstline[255];
                    char methodname[255];
                    get_line(i, firstline, sizeof(firstline));
                    printf(firstline);
                    nbytes = read_from_client(i, buffer);
                    printf(buffer);
                    printf("- end -\n");
                    // check the method name
                    int k = 0, j = 0;
                    while(!isspace((int)firstline[j]) && (k < sizeof(methodname) - 1)) {
                        methodname[k] = firstline[j];
                        k++; j++;
                    }
                    methodname[k] = '\0';
                    if(strcasecmp(methodname, "GET") == 0) {
                        char *sample = "<!DOCTYPE html>\n<html>\n<title>HTML Tutorial</title>\n<body>\n<h1>heading</h1>\n</body>\n</html>";
                        send_header(i);
                        send(i, sample, strlen(sample), 0);
                        send(i, "\r\n", strlen("\r\n"), 0);
                        FD_CLR(i, &active_fd_set);
                        close(i);
                    } else {
                        // ...
                    }
                    // do forget to:
                    // FD_CLR(i, &active_fd_set);
                    // ...
                    /*
                    char *sample = "<!DOCTYPE html>\n<html>\n<title>HTML Tutorial</title>\n<body>\n<h1>heading</h1>\n</body>\n</html>";
                    send_header(i);
                    send(i, sample, strlen(sample), 0);
                    send(i, "\r\n", strlen("\r\n"), 0);
                    */

                }
            }
        }
    }
    return 0;
}

