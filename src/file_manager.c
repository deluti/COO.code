#include "headers/file_manager.h"
#include "headers/colors.h"
#include <io.h>
#include <stdlib.h>
#include <direct.h>

// Проверяет существование папки под репозитории
int check_directory(char *directory_path){
    printf("> Checking folder preservation for repositories...\n");
    // Проверка существования папки
    if (_access(directory_path, 00) == 0){
        printf("> Directory exist.\n> Welcome to COO.code!\n");
    } else {
        printf("> Desired directory not found. Attempt to create...\n");
        if (_mkdir("repositories") == 0){
            printf(GREEN "> Directory created. Welcome to COO.code!\n" RESET);
            return 0;
        } else {
            perror(RED "[ERROR] Failed to create folder.\n" RESET);
            return 1;
        }
    }
}
