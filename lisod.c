#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <fcntl.h>
#include "logging.h"
#include "parse.h"
#define SERVER_ID "liso/1.0\r\n"
const int BUFFER_SIZE = 40960;
int PORT;
char* LOG_PATH;
char* FOLDER_NAME;
int make_socket(unsigned short port)
{
    int sock;
    struct sockaddr_in name;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("failed to establish socket\n");
        log_error("failed to establish socket\n");
        exit(EXIT_FAILURE);
    }

    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock, (struct sockaddr *) &name, sizeof(name)) < 0) {
        perror("failed to bind a name to socket\n");
        log_error("failed to bind a name to socket\n");
        exit(EXIT_FAILURE);
    }

    return sock;
}
char* get_type(char *uri)
{
    if (strstr(uri, ".html"))
    {
        return "text/html";
    }
    else if (strstr(uri, ".css"))
    {
        return "text/css";
    }
    else if (strstr(uri, ".jpg"))
    {
        return "image/jpeg";
    }
    else if (strstr(uri, ".png"))
    {
        return "image/png";
    }
    else if (strstr(uri, ".gif"))
    {
        return "image/gif";
    }
    else
    {
        return "application/octet-stream";
    }
}

void send_header(int client, char *type,  int contentlength) {
    char t[32];
    char header[BUFFER_SIZE];
    struct tm tm;
    time_t now;
    now = time(0);
    tm = *gmtime(&now);
    strftime(t, 32, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    sprintf(header, "HTTP/1.1 200 OK\r\n");
    sprintf(header, "%sDate: %s\r\n", header, t);
    sprintf(header, "%sServer: %s", header, SERVER_ID);
    sprintf(header, "%sContent-Type: %s\r\n", header, type);
    sprintf(header, "%sContent-Length: %d\r\n\r\n", header, contentlength);
    send(client, header, strlen(header), 0);
}

void handle_error(int client, char *error_code, char *msg){
    char t[32];
    char header[BUFFER_SIZE];
    char body[BUFFER_SIZE];
    struct tm tm;
    time_t now;
    now = time(0);
    tm = *gmtime(&now);
    strftime(t, 32, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    sprintf(header, "HTTP/1.1 %s %s\r\n", error_code, msg);
    sprintf(header, "%sDate: %s\r\n", header, t);
    sprintf(header, "%sServer: %s", header, SERVER_ID);
    sprintf(header, "%sContent-Type: text/html\r\n\r\n", header);
    sprintf(body, "<html><title>Error</title>");
    sprintf(body, "%s<body>\r\n", body);
    sprintf(body, "%sError %s -- %s\r\n", body, error_code, msg);
    send(client, header, strlen(header), 0);
    send(client, body, strlen(body), 0);
}

int main(int argc, char* argv[]) {
    int sock;
    fd_set active_fd_set, read_fd_set;
    if(argc < 4){
        printf("Usage ./lisod <HTTP PORT> <LOG_PATH> <WWW FOLDER NAME>\n");
        exit(EXIT_FAILURE);
    }
    PORT = atoi(argv[1]);
    if(PORT == 0 || PORT > 65535){
        printf("invalid port number!\n");
        exit(EXIT_FAILURE);
    }
    LOG_PATH = argv[3];
    FOLDER_NAME = argv[5];
    
    set_file(LOG_PATH);
    log_info("The server listens to port %d\n", PORT);
    log_info("The www folder is %s\n", FOLDER_NAME);
    log_info("The log path is %s\n", LOG_PATH);
    sock = make_socket(PORT);
    if(listen(sock, SOMAXCONN) < 0) {
        perror("failed to listen on the socket\n");
        log_error("failed to listen on the socket\n");
        exit(EXIT_FAILURE);
    }
    FD_ZERO(&active_fd_set);
    FD_SET(sock, &active_fd_set);

    int size, newsock, nbytes;
    struct sockaddr_in clientname;
    while(1) {
        /* Block until input arrives on one or more active sockets. */
        read_fd_set = active_fd_set;
        if(select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
            perror("failed when processing \"select\"\n");
            log_error("failed when processing \"select\"\n");
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < FD_SETSIZE; ++i) {
            if(FD_ISSET(i, &read_fd_set)) {
                if(i == sock) {
                    size = sizeof(clientname);
                    newsock = accept(sock, (struct sockaddr *) &clientname, (socklen_t *) &size);
                    if(newsock < 0) {
                        perror("failed to accept new connection\n");
                        log_error("failed to accept new connection\n");
                    }
                    char *ip = inet_ntoa((&clientname)->sin_addr);
                    log_info("Server: new connection established from %s\n", ip);
                    FD_SET(newsock, &active_fd_set);
                } else {
                    /* Data arriving on an already-connected socket. */
                    char buffer[BUFFER_SIZE];
                    char filepath[255];
                    int readRet = recv(i, buffer, BUFFER_SIZE, 0);
                    if(readRet == -1){
                        perror("something went wrong when reading FD\n");
                        log_error("something went wrong when reading FD\n");
                        exit(EXIT_FAILURE);
                    }
                    //parse request
                    Request *request = parse(buffer,readRet,i);
                    if(request == NULL){//bad request
                        log_info("Bad Request From Client.\n");
                        handle_error(i, "400", "Bad Request");
                        FD_CLR(i, &active_fd_set);
                        close(i);
                        continue;
                    }
                    if(strcasecmp(request->http_version, "HTTP/1.1") != 0){
                        log_info("%s %s %s 505\n", request->http_method, request->http_uri, request->http_version);
                        handle_error(i, "505", "HTTP Version Not Supported.");
                        FD_CLR(i, &active_fd_set);
                        close(i);
                        continue;
                    }
                    if(strcasecmp(request->http_method, "GET") == 0) {
                        //handle GET
                        strcpy(filepath, FOLDER_NAME);
                        if(request->http_uri[1] == '\0') {
                            strcat(filepath, "/index.html");
                        } else {
                            strcat(filepath, request->http_uri);
                        }
                        printf("filepath: %s\n", filepath);
                        // FILE *resource = fopen(filepath, "r");
                        int resource = open(filepath, O_RDONLY);
                        if (resource == -1) {
                            char tmpbuf[1024];
                            handle_error(i, "404", "Not Found");
                            log_info("%s %s %s 404\n", request->http_method, request->http_uri, request->http_version);
                        } else {
                            struct stat st;
                            stat(filepath, &st);
                            send_header(i, get_type(filepath), st.st_size);
                            long ret;
                            while ((ret = read(resource, buffer, sizeof(buffer))) > 0)
                                send(i, buffer, sizeof(buffer), 0); 
                            log_info("%s %s %s 200\n", request->http_method, request->http_uri, request->http_version);
                        }
                        FD_CLR(i, &active_fd_set);
                        close(i);
                    } else if(strcasecmp(request->http_method, "HEAD") == 0) {
                        //handle HEAD
                        strcpy(filepath, FOLDER_NAME);
                        if(request->http_uri[1] == '\0') {
                            strcat(filepath, "/index.html");
                        } else {
                            strcat(filepath, request->http_uri);
                        }
                        FILE *resource = fopen(filepath, "r");
                        if (resource == NULL) {
                            char tmpbuf[1024];
                            handle_error(i, "404", "Not Found");
                            log_info("%s %s %s 404\n", request->http_method, request->http_uri, request->http_version);
                        } else {
                            //just send the header
                            struct stat st;
                            stat(filepath, &st);
                            send_header(i, get_type(request->http_uri), st.st_size);
                        }
                        FD_CLR(i, &active_fd_set);
                        close(i);
                    } else if(strcasecmp(request->http_method, "POST") == 0) {
                        //handle POST
                        char header[BUFFER_SIZE];
                        char t[32];
                        struct tm tm;
                        time_t now;
                        now = time(0);
                        tm = *gmtime(&now);
                        strftime(t, 32, "%a, %d %b %Y %H:%M:%S %Z", &tm);
                        sprintf(header, "HTTP/1.1 200 OK\r\n");
                        sprintf(header, "%sDate: %s\r\n", header, t);
                        sprintf(header, "%sServer: %s", header, SERVER_ID);
                        send(i, header, strlen(header), 0);
                        log_info("%s %s %s 200\n", request->http_method, request->http_uri, request->http_version);
                        FD_CLR(i, &active_fd_set);
                        close(i);
                    }else{
                        //handle Unsupported Method
                        handle_error(i, "501", "HTTP Method Not Implemented");
                        log_info("%s %s %s 501\n", request->http_method, request->http_uri, request->http_version);
                        FD_CLR(i, &active_fd_set);
                        close(i);
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

