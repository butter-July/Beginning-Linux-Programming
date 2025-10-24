#include "app_mysql.h"
#include <stdio.h>

int main() {
    if (!database_start("wawa", "qa861234")) {
        printf("Database connection failed!\n");
        return 1;
    }
    printf("Database connected successfully.\n");
    database_end();
    return 0;
}
