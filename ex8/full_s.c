#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#define PORT 5063

int main() {
int s, c;
struct sockaddr_in server, client;
socklen_t len = sizeof(client);

s = socket(AF_INET, SOCK_STREAM, 0);

server.sin_family = AF_INET;
server.sin_port = htons(PORT);
server.sin_addr.s_addr = INADDR_ANY;

if(bind(s, (struct sockaddr *)&server, sizeof(server))<0){
   perror("Binding failed\n");
   close(s);
   return 1;
}
listen(s, 5);

printf("Server waiting for client...\n");

c = accept(s, (struct sockaddr *)&client, &len);
printf("Client connected!\n");

if (fork() == 0) {
    char msg[1024];
    while (1){
        printf("Enter Message :");
        fgets(msg, sizeof(msg), stdin);
        if (send(c, msg, strlen(msg), 0) <= 0)
            break;
        if (strncmp(msg, "exit", 4) == 0)
            break;
    }
    shutdown(c, SHUT_WR);
    exit(0);
} else {
    char msg[1024];
    int n;
    while ((n = recv(c, msg, sizeof(msg) - 1, 0)) > 0) {
        msg[n] = '\0';
        printf("Client: %s", msg);
        if (strncmp(msg, "exit", 4) == 0)
            break;
    }
}

close(c);
close(s);
return 0;

}
