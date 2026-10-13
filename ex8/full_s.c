#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 5063
#define SIZE 1024
#define MAX_QUEUE 10

typedef struct {
    struct sockaddr_in addr;
    socklen_t len;
} ClientNode;

int main(void) {
    int s, n,i;
    char b[SIZE];
    struct sockaddr_in addr = {0};
    fd_set readfds;

    ClientNode queue[MAX_QUEUE];
    int q_front = 0, q_rear = 0, q_count = 0;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("socket");
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(s);
        return 1;
    }

    printf("UDP Server running on port %d. Type EXIT in server console to quit.\n", PORT);
    fflush(stdout);

    int server_running = 1;
    int chat_active = 0;
    struct sockaddr_in client = {0};
    socklen_t client_len = sizeof(client);

    while (server_running) {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(s, &readfds);

        if (select(s + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("select");
            break;
        }

        // 1. Check if server operator typed EXIT on stdin
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            if (fgets(b, sizeof(b), stdin)) {
                b[strcspn(b, "\n")] = '\0';
                if (strcmp(b, "EXIT") == 0) {
                    server_running = 0;
                    break;
                }
                // If chat is active, server can send messages to the current client
                if (chat_active) {
                    if (strcmp(b, "EXIT") == 0) {
                        sendto(s, "EXIT", 4, 0, (struct sockaddr *)&client, client_len);
                        printf("You disconnected from the client.\n");
                        chat_active = 0;
                    } else if (b[0] != '\0') {
                        sendto(s, b, strlen(b), 0, (struct sockaddr *)&client, client_len);
                        printf("Server> ");
                        fflush(stdout);
                        continue;
                    }
                }
            }
        }

        // 2. Check for incoming datagrams on socket
        if (FD_ISSET(s, &readfds)) {
            struct sockaddr_in temp_addr = {0};
            socklen_t temp_len = sizeof(temp_addr);
            n = recvfrom(s, b, SIZE - 1, 0, (struct sockaddr *)&temp_addr, &temp_len);
            if (n > 0) {
                b[n] = '\0';

                // Handle HELLO request
                if (strcmp(b, "HELLO") == 0) {
                    if (!chat_active) {
                        // Connect immediately
                        client = temp_addr;
                        client_len = temp_len;
                        chat_active = 1;
                        sendto(s, "CONNECTED", 9, 0, (struct sockaddr *)&client, client_len);
                        printf("Client connected. Type EXIT to disconnect client.\n\nServer> ");
                        fflush(stdout);
                    } else {
                        // Check if this client is already the active one or already queued
                        int already_in_queue = 0;
                        for (i = 0; i < q_count; i++) {
                            int idx = (q_front + i) % MAX_QUEUE;
                            if (queue[idx].addr.sin_addr.s_addr == temp_addr.sin_addr.s_addr &&
                                queue[idx].addr.sin_port == temp_addr.sin_port) {
                                already_in_queue = 1;
                                break;
                            }
                        }

                        if ((temp_addr.sin_addr.s_addr == client.sin_addr.s_addr &&
                             temp_addr.sin_port == client.sin_port) || already_in_queue) {
                            // Ignore duplicate HELLO
                            continue;
                        }

                        if (q_count < MAX_QUEUE) {
                            queue[q_rear].addr = temp_addr;
                            queue[q_rear].len = temp_len;
                            q_rear = (q_rear + 1) % MAX_QUEUE;
                            q_count++;
                            sendto(s, "WAIT", 4, 0, (struct sockaddr *)&temp_addr, temp_len);
                            printf("Client added to queue. Position: %d\n", q_count);
                            fflush(stdout);
                        } else {
                            sendto(s, "BUSY", 4, 0, (struct sockaddr *)&temp_addr, temp_len);
                        }
                    }
                } 
                // Handle messages or EXIT from active client
                else if (chat_active &&
                         temp_addr.sin_addr.s_addr == client.sin_addr.s_addr &&
                         temp_addr.sin_port == client.sin_port) {
                    printf("\r\033[K");
                    if (strcmp(b, "EXIT") == 0) {
                        printf("Client left the chat.\n");
                        chat_active = 0;
                    } else {
                        printf("Client: %s\nServer> ", b);
                        fflush(stdout);
                    }

                    // If active client left, promote next client from queue
                    if (!chat_active && q_count > 0) {
                        client = queue[q_front].addr;
                        client_len = queue[q_front].len;
                        q_front = (q_front + 1) % MAX_QUEUE;
                        q_count--;
                        chat_active = 1;
                        sendto(s, "CONNECTED", 9, 0, (struct sockaddr *)&client, client_len);
                        printf("Queued client promoted and connected!\n\nServer> ");
                        fflush(stdout);
                    } else if (!chat_active) {
                        printf("Waiting for a client...\n");
                        fflush(stdout);
                    }
                }
            }
        }
    }

    close(s);
    printf("Server shut down.\n");
    return 0;
}
