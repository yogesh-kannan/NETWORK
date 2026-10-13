#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#define PORT 5063

int main() {
int s;
struct sockaddr_in server;

s = socket(AF_INET, SOCK_STREAM, 0);

server.sin_family = AF_INET;
server.sin_port = htons(PORT);
server.sin_addr.s_addr = inet_addr("127.0.0.1");

if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("Connection failed");
    close(s);
    return 1;
}

printf("Connected to server!\n");

if (fork() == 0) {
    char msg[1024];
    while (1){
        printf("Enter Message :");
        fgets(msg, sizeof(msg), stdin);
        if (send(s, msg, strlen(msg), 0) <= 0)
            break;
        if (strncmp(msg, "exit", 4) == 0)
            break;
    }
    shutdown(s, SHUT_WR);
    exit(0);
} else {
    char msg[1024];
    int n;
    while ((n = recv(s, msg, sizeof(msg) - 1, 0)) > 0) {
        msg[n] = '\0';
        printf("Server: %s", msg);
        if (strncmp(msg, "exit", 4) == 0)
            break;
    }
}

close(s);
return 0;

}
