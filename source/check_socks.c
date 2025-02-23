#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define MAX_THREADS 1000 
typedef struct {
    char proxy_ip[50];
    int proxy_port;
    char protocol[20];
} ProxyInfo;

int successful_proxies = 0;
int failed_proxies = 0;
pthread_mutex_t stats_mutex = PTHREAD_MUTEX_INITIALIZER;
void save_to_file(const char *proxy_ip, int proxy_port, const char *protocol) {
    FILE *file = fopen("conf/list_socks.txt", "a");
    if (file == NULL) {
        perror("Unable to open file for writing");
        return;
    }
    fprintf(file, "%s:%d - %s\n", proxy_ip, proxy_port, protocol);
    fclose(file);
}
void *ping_ip_thread(void *arg) {
    char *ip_address = (char *)arg;
    char command[100];
    snprintf(command, sizeof(command), "ping -c 1 -W 1 %s > /dev/null 2>&1", ip_address);
    int result = system(command);

    return (void *)(intptr_t)result;
}
int check_socks4_proxy(const char *proxy_ip, int proxy_port) {
    int sock;
    struct sockaddr_in server;
    unsigned char request[7] = {0x04, 0x01, 0x00, 0x50, 0x00, 0x00, 0x00};
    unsigned char response[8];
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return 0;
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(proxy_port);
    if (inet_pton(AF_INET, proxy_ip, &server.sin_addr) <= 0) {
        close(sock);
        return 0;
    }
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        close(sock);
        return 0;
    }
    if (send(sock, request, sizeof(request), 0) != sizeof(request)) {
        close(sock);
        return 0;
    }
    if (recv(sock, response, sizeof(response), 0) != sizeof(response)) {
        close(sock);
        return 0;
    }
    if (response[1] == 0x5A) { 
        close(sock);
        return 1;
    } else {
        close(sock);
        return 0;
    }
}
int check_socks5_proxy(const char *proxy_ip, int proxy_port) {
    int sock;
    struct sockaddr_in server;
    unsigned char request[3] = {0x05, 0x01, 0x00};
    unsigned char response[2];
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return 0;
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(proxy_port);
    if (inet_pton(AF_INET, proxy_ip, &server.sin_addr) <= 0) {
        close(sock);
        return 0;
    }
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        close(sock);
        return 0;
    }
    if (send(sock, request, sizeof(request), 0) != sizeof(request)) {
        close(sock);
        return 0;
    }
    if (recv(sock, response, sizeof(response), 0) != sizeof(response)) {
        close(sock);
        return 0;
    }
    if (response[0] == 0x05 && response[1] == 0x00) { 
        close(sock);
        return 1;
    } else {
        close(sock);
        return 0;
    }
}

void *check_proxy(void *arg) {
    ProxyInfo *proxy_info = (ProxyInfo *)arg;
    char *proxy_ip = proxy_info->proxy_ip;
    int proxy_port = proxy_info->proxy_port
    pthread_t ping_thread;
    pthread_create(&ping_thread, NULL, ping_ip_thread, (void *)proxy_ip);
    void *ping_result;
    pthread_join(ping_thread, &ping_result);
    if ((intptr_t)ping_result != 0) {
        pthread_mutex_lock(&stats_mutex);
        failed_proxies++;  
        pthread_mutex_unlock(&stats_mutex);
        free(proxy_info); 
        return NULL;
    }

    int socks4_valid = check_socks4_proxy(proxy_ip, proxy_port);
    int socks5_valid = check_socks5_proxy(proxy_ip, proxy_port);
    char protocol[30] = "";

    if (socks4_valid && socks5_valid) {
        snprintf(protocol, sizeof(protocol), "SOCKS4 - SOCKS5");
        pthread_mutex_lock(&stats_mutex);
        successful_proxies++;
        pthread_mutex_unlock(&stats_mutex);
        save_to_file(proxy_ip, proxy_port, protocol);
    } else if (socks4_valid) {
        snprintf(protocol, sizeof(protocol), "SOCKS4");
        pthread_mutex_lock(&stats_mutex);
        successful_proxies++;
        pthread_mutex_unlock(&stats_mutex);
        save_to_file(proxy_ip, proxy_port, protocol);
    } else if (socks5_valid) {
        snprintf(protocol, sizeof(protocol), "SOCKS5");
        pthread_mutex_lock(&stats_mutex);
        successful_proxies++;
        pthread_mutex_unlock(&stats_mutex);
        save_to_file(proxy_ip, proxy_port, protocol);
    } else {
        pthread_mutex_lock(&stats_mutex);
        failed_proxies++;
        pthread_mutex_unlock(&stats_mutex);
    }

    free(proxy_info);

    return NULL;
}

void print_statistics_continuous() {
    while (1) {
        printf("\033[H\033[J");
        printf("--- Current Statistics ---\n");
        printf("Successful Proxies: %d\n", successful_proxies);
        printf("Failed Proxies: %d\n", failed_proxies);
        printf("--------------------------\n");
        usleep(1000000);
    }
}
int main() {
    FILE *file = fopen("conf/data.txt", "r");
    if (file == NULL) {
        perror("Unable to open file");
        return 1;
    }

    pthread_t threads[MAX_THREADS];
    pthread_t stats_thread; 
    int thread_count = 0;
    pthread_create(&stats_thread, NULL, (void *)print_statistics_continuous, NULL);

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        ProxyInfo *proxy_info = (ProxyInfo *)malloc(sizeof(ProxyInfo));
        if (sscanf(line, "%49[^:]:%d", proxy_info->proxy_ip, &proxy_info->proxy_port) == 2) {
            pthread_create(&threads[thread_count], NULL, check_proxy, (void *)proxy_info);
            thread_count++;
            if (thread_count >= MAX_THREADS) break;
        }
    }
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_cancel(stats_thread);
    pthread_join(stats_thread, NULL);

    fclose(file);
    return 0;
}
