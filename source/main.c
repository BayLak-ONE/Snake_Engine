#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// تعريف أكواد الألوان
#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define BOLD        "\033[1m"

// دالة لمسح الشاشة
void clear_screen() {
    #ifdef _WIN32
        system("cls"); // ويندوز
    #else
        system("clear"); // لينكس/ماك
    #endif
}
void fetch_ip_info() {
    char buffer[1024];
    FILE *fp;

    // استدعاء curl لجلب بيانات IP
    fp = popen("curl -s https://ipinfo.io/json", "r");
    if (fp == NULL) {
        perror("Failed to run curl");
        exit(1);
    }

    // قراءة البيانات من curl
    char ip[64] = "", city[64] = "", country[64] = "", timezone[64] = "";
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // البحث عن البيانات المطلوبة
        if (strstr(buffer, "\"ip\"")) {
            sscanf(buffer, " \"ip\": \"%[^\"]\"", ip);
        } else if (strstr(buffer, "\"city\"")) {
            sscanf(buffer, " \"city\": \"%[^\"]\"", city);
        } else if (strstr(buffer, "\"country\"")) {
            sscanf(buffer, " \"country\": \"%[^\"]\"", country);
        } else if (strstr(buffer, "\"timezone\"")) {
            sscanf(buffer, " \"timezone\": \"%[^\"]\"", timezone);
        }
    }
    clear_screen();
    printf(CYAN "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀ \n");
    printf(CYAN "⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⡉⠙⣻⣷⣶⣤⣀⠀⠀⠀\n");
    printf(CYAN "⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⡿⠋⠀⠀⠀⠀⢹⣿⣿⡟⠉⠉⠉⢻⡿⠀⠀⠀\n");
    printf(CYAN "⠀⠀⠀⠀⠀⠀⠀⠰⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⣿⣿⣇⠀⠀⠀⠈⠇⠀⠀⠀\n");
    printf(CYAN "⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠉⠛⠿⣷⣤⡤⠀⠀⠀⠀⠀\n");
    printf(CYAN "⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣿⣿⣿⣿⣿⣶⣦⣤⣤⣀⣀⣀⡀⠉⠀⠀⠀⠀- Snake Engine⠀V1.0⠀\n");
    printf(CYAN "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀⠀⠀\n");
    printf(CYAN "⠀⠀⠀⢀⣀⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠙⠛⠿⣿⣿⣿⣿⣿⣦⠀⠀⠀\n");
    printf(CYAN "⠀⠀⣰⣿⣿⣿⣿⣿⣷⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣧⠀⠀\n");
    printf(CYAN "⠀⠀⣿⣿⣿⠁⠀⠈⠙⢿⣿⣦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⠀⠀\n");
    printf(CYAN "⠀⠀⢿⣿⣿⣆⠀⠀⠀⠀⠈⠛⠿⣿⣶⣦⡤⠴⠀⠀⠀⠀⠀⣸⣿⣿⣿⡿⠀⠀\n");
    printf(CYAN "⠀⠀⠈⢿⣿⣿⣷⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣿⠃⠀⠀\n");
    printf(CYAN "⠀⠀⠀⠀⠙⢿⣿⣿⣿⣶⣦⣤⣀⣀⡀⠀⠀⠀⣀⣠⣴⣾⣿⣿⣿⡿⠃⠀⠀⠀\n");
    printf(CYAN "⠀⠀⠀⠀⠀⠀⠈⠙⠻⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠋⠀⠀⠀⠀⠀\n");
    printf(CYAN "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠙⠛⠛⠛⠛⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀\n");

       printf("- %s | %s | %s | %s\n", ip, country, city, timezone);

    // إغلاق العملية
    pclose(fp);
}
int main() {
    int choice;

    // عرض الرسم ASCII مع ألوان جميلة

    do {
fetch_ip_info();
    printf(BLUE "\n1. Build Socks http-4-5\n");
    printf("2. Scan Socks 4-5\n");
    printf("3. Browser Snake Multiple Socks\n");
    printf("4. DDOS Attack By Socks\n");
    printf("5. Update\n");
    printf("6. Uninstall\n");
    printf(RED "7. Exit\n");

        // قراءة اختيار المستخدم
        printf(MAGENTA "> ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                system("./build_socks");
                break;
            case 2:
                system("./check_socks");
                break;
            case 3:
                system("./browser_socks");
                break;
            case 4:
                system("./ddos_socks");
             break;
            case 5:
              system("chmod +x ./update.sh && ./update.sh");
                break;
             case 6:
              system("chmod +x ./uninstall.sh && ./uninstall.sh");
                break;
                case 7:
                break;
            default:
                printf(RED "error select ..!\n");
                break;
        }
    } while (choice != 7);

    return 7;
}
