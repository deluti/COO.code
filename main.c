// Главный файл проекта
#include <stdio.h>
#include "src/file_manager.c"
#include "src/headers/colors.h"

int main(void){
    printf(BLUE "COO.code v1.0.0 started\n\n" RESET);
    // Проверка существования папки с репозиториями
    check_directory("./repositories");

    // Выход
    printf("\n> Enter to close:");
    scanf("1");
    return 0;
}