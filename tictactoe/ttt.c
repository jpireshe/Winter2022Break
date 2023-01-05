#include "libtt.h"

int main(int argc, char* argv[]) {
    // SERVER SET UP
    signal(SIGCHLD, &server_handle_sigchild);
    if(argc < 2) return 1;
    int server_socket, server_port;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_port = atoi(argv[1]);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(server_port);
    // bind
    int server_t = bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));      
    // listen
    listen(server_socket, 5);
    // accept
    int server_cl, server_cpid;

    // SERVER LOOP
    while(1) {
        server_cl = accept(server_socket, NULL, NULL);

        server_cpid = fork();

        if(server_cpid == 0) {
            server_handle_incomming_connection(server_cl);
        }

        close(server_cl);
    }

}