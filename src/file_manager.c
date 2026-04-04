#include "headers/file_manager.h"
#include "headers/colors.h"
#include <io.h>
#include <stdlib.h>
#include <direct.h>
#include <string.h>
#include <stdio.h>

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

// Функция для получения списка репозиториев (опционально)
void list_repositories(void) {
    printf(YELLOW "\n> Available repositories:\n" RESET);
    // Здесь можно реализовать поиск всех папок repo_* в директории repositories
}

// Создаёт пустой репозиторий
int init_repo(char *repo_name) {
    char repo_path[512];
    
    // Формируем путь к репозиторию
    snprintf(repo_path, sizeof(repo_path), "./repositories/repo_%s", repo_name);
    
    // Проверяем, не существует ли уже
    if (_access(repo_path, 00) == 0) {
        printf(RED "[ERROR] Repository '%s' already exists!\n" RESET, repo_name);
        return 1;
    }
    
    printf("> Creating repository '%s'...\n", repo_name);
    
    // Создаём основную папку репозитория
    if (_mkdir(repo_path) != 0) {
        perror(RED "[ERROR] Failed to create repository folder\n" RESET);
        return 1;
    }
    
    // Создаём подпапку .coocode для метаданных
    char metadata_path[512];
    snprintf(metadata_path, sizeof(metadata_path), "%s/.coocode", repo_path);
    if (_mkdir(metadata_path) != 0) {
        perror(RED "[ERROR] Failed to create .coocode folder\n" RESET);
        return 1;
    }
    
    // Создаём папку для коммитов
    char commits_path[512];
    snprintf(commits_path, sizeof(commits_path), "%s/.coocode/commits", metadata_path);
    if (_mkdir(commits_path) != 0) {
        perror(RED "[ERROR] Failed to create commits folder\n" RESET);
        return 1;
    }
    
    // Создаём файл с информацией о репозитории
    char info_path[512];
    snprintf(info_path, sizeof(info_path), "%s/.coocode/info.txt", repo_path);
    FILE *info = fopen(info_path, "w");
    if (info) {
        fprintf(info, "Repository: %s\n", repo_name);
        fprintf(info, "Created: %s\n", __DATE__);
        fclose(info);
    }
    
    printf(GREEN "> Repository '%s' created successfully!\n" RESET, repo_name);
    return 0;
}

// Создаёт файл в указанном репозитории
int add_file(char *repo_name, char *file_path) {
    char repo_full_path[512];
    snprintf(repo_full_path, sizeof(repo_full_path), "./repositories/repo_%s", repo_name);
    
    // Проверяем существование репозитория
    if (_access(repo_full_path, 00) != 0) {
        printf(RED "[ERROR] Repository '%s' not found!\n" RESET, repo_name);
        return 1;
    }
    
    // Проверяем существование исходного файла
    if (_access(file_path, 00) != 0) {
        printf(RED "[ERROR] File '%s' not found!\n" RESET, file_path);
        return 1;
    }
    
    // Получаем только имя файла (без пути)
    char *filename = strrchr(file_path, '/');
    if (!filename) {
        filename = strrchr(file_path, '\\');  // Для Windows
    }
    if (filename) {
        filename++;  // Пропускаем символ /
    } else {
        filename = file_path;  // Если нет слешей, используем весь путь
    }
    
    // Формируем путь для копирования файла в репозиторий (только имя файла)
    char dest_path[512];
    snprintf(dest_path, sizeof(dest_path), "%s/%s", repo_full_path, filename);
    
    printf("> Copying file to: %s\n", dest_path);
    
    // Копируем файл
    FILE *src = fopen(file_path, "rb");
    if (!src) {
        printf(RED "[ERROR] Failed to open source file! Error: %d\n" RESET, errno);
        return 1;
    }
    
    FILE *dst = fopen(dest_path, "wb");
    if (!dst) {
        printf(RED "[ERROR] Failed to create destination file! Error: %d\n" RESET, errno);
        fclose(src);
        return 1;
    }
    
    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dst);
    }
    
    fclose(src);
    fclose(dst);
    
    printf(GREEN "> File '%s' added to repository '%s' as '%s'\n" RESET, file_path, repo_name, filename);
    return 0;
}

// Удаляет файл в указанном репозитории
int remove_file(char *repo_name, char *file_path) {
    char full_path[512];
    snprintf(full_path, sizeof(full_path), "./repositories/repo_%s/%s", repo_name, file_path);
    
    // Проверяем существование файла
    if (_access(full_path, 00) != 0) {
        printf(RED "[ERROR] File '%s' not found in repository!\n" RESET, file_path);
        return 1;
    }
    
    // Удаляем файл
    if (remove(full_path) == 0) {
        printf(GREEN "> File '%s' removed from repository '%s'\n" RESET, file_path, repo_name);
        return 0;
    } else {
        perror(RED "[ERROR] Failed to remove file\n" RESET);
        return 1;
    }
}

// Остальные функции-заглушки (временно)
char* get_file_content(char *commit, char *file_path) {
    printf(YELLOW "[WARNING] get_file_content not implemented yet\n" RESET);
    return NULL;
}

int get_file_exists(char *commit, char *file_path) {
    printf(YELLOW "[WARNING] get_file_exists not implemented yet\n" RESET);
    return 0;
}

int print_commit(char *commit) {
    printf(YELLOW "[WARNING] print_commit not implemented yet\n" RESET);
    return 0;
}

int print_history(char *commit) {
    printf(YELLOW "[WARNING] print_history not implemented yet\n" RESET);
    return 0;
}

int print_files(char *commit) {
    printf(YELLOW "[WARNING] print_files not implemented yet\n" RESET);
    return 0;
}