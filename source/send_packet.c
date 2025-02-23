#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

void usage() {
system("clear");
  printf("\033[1;32m");
    printf("         /^\\/^\\\n");
    printf("        /       \\\n");
    printf("\033[1;33m");
    printf("       |  O   O  | Version: 1.2\n");
    printf("       |    ^    | Dev: BayLak\n");
    printf("\033[1;34m");
    printf("        \\_______/ Info: Send packet by Socks!\n");
    printf("\033[0m");
    printf("\033[1;32m");
    printf("         \\_____/\n");
    printf("          |   |\n");
    printf("          |   |\n");
    printf("          |   |\n");
    printf("\033[1;31m");
    printf("         /     \\\n");
    printf("        /       \\\n");
    printf("\033[0m");
    printf("\nuse > ./send_packet -ip <proxy_ip:port> -p <url> -m <message>\n");
    printf("like > ./send_packet -ip socks5://192.168.1.2:8080 -p http://localhost -m \"Hello world!\"\n");
}

int main(int argc, char *argv[]) {
    CURL *curl;
    CURLcode res;
    const char *proxy = NULL;
    const char *url = NULL;
    const char *message = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-ip") == 0 && i + 1 < argc) {
            proxy = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            url = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            message = argv[i + 1];
            i++;
} else {
            usage();
            return 1;
        }
    }
    if (!proxy || !url || !message) {
        usage();
        return 1;
    }
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_PROXY, proxy);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "SEND ERROR", curl_easy_strerror(res));
        } else {
            printf("SEND ONE PACKET TRUE.\n");
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return 0;
}
