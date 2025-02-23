#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
size_t write_data(void *ptr, size_t size, size_t nmemb, char *data) {
    size_t realsize = size * nmemb;
    strncat(data, ptr, realsize);
    return realsize;
}
int is_valid_url(const char *url) {
    return (strncmp(url, "http://", 7) == 0 || strncmp(url, "https://", 8) == 0);
}
int read_links_from_file(const char *filename, char links[][1024], int *link_count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return -1;
    }

    char line[1024];
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (is_valid_url(line)) {
            strcpy(links[count], line);
            count++;
        }
    }

    fclose(file);
    *link_count = count;
    return 0;
}
int is_line_exists(const char *filename, const char *line) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    char file_line[1024];
    while (fgets(file_line, sizeof(file_line), file)) {
        file_line[strcspn(file_line, "\n")] = 0; 
        char *start = file_line;
        while (*start == ' ' || *start == '\t') start++;
        char *end = file_line + strlen(file_line) - 1;
        while (end > start && (*end == ' ' || *end == '\t')) end--;
        *(end + 1) = '\0';

        if (strcmp(start, line) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int main() {
    char links[100][1024];
    int link_count = 0;
    if (read_links_from_file("links.txt", links, &link_count) != 0) {
        return 1;
    }
    char data[1000000] = ""; 
    CURL *curl;
    CURLcode res;
    int added_count = 0;
    int repeat_count = 0;
    int total_ips = 0;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        FILE *file = fopen("conf/data.txt", "w");
        if (file == NULL) {
            fprintf(stderr, "Failed to open data.txt file.\n");
            return 1;
        }
        for (int i = 0; i < link_count; i++) {
            printf("download > : %s\n", links[i]);
            curl_easy_setopt(curl, CURLOPT_URL, links[i]);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "Failed to download data from: %s\n", links[i]);
                fprintf(stderr, "Error: %s\n", curl_easy_strerror(res)); 
                continue;
            } else {
                char *line = strtok(data, "\n");
                while (line != NULL) {
                    if (strchr(line, ':')) {
                        total_ips++;
                        if (!is_line_exists("conf/data.txt", line)) {
                            fprintf(file, "%s\n", line);
                            added_count++;
                        } else {
                            repeat_count++;
                        }
                    }
                    line = strtok(NULL, "\n");
                }
            }
            memset(data, 0, sizeof(data));
            printf("\r\033[1;32mLink %d/%d\033[0m  |  \033[1;34mTotal IPs: %d\033[0m  |  "
                   "\033[1;33mRepetitions: %d\033[0m  |  \033[1;31mAdded: %d\033[0m   ",
                   i + 1, link_count, total_ips, repeat_count, added_count);
            fflush(stdout);
        }
        fclose(file);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    printf("\nFinal statistics:\n");
    printf("\033[1;32mTotal links: %d\033[0m  |  \033[1;34mTotal IPs: %d\033[0m  |  "
           "\033[1;33mRepetitions: %d\033[0m  |  \033[1;31mAdded: %d\033[0m\n",
           link_count, total_ips, repeat_count, added_count);
    printf("Data has been saved to data.txt\n");
    printf("Running ./home...\n");
    system("./main");

    return 0;
}
