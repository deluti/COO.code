#include "headers/file_manager.h"
#include "headers/colors.h"
#include <io.h>
#include <stdlib.h>
#include <direct.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

// Функция для рекурсивного создания директорий
int mkdir_recursive(const char *path) {
    char temp[512];
    char *p = NULL;
    size_t len;
    
    snprintf(temp, sizeof(temp), "%s", path);
    len = strlen(temp);
    
    // Убираем trailing slash
    if (temp[len - 1] == '/' || temp[len - 1] == '\\') {
        temp[len - 1] = 0;
    }
    
    // Создаем папки рекурсивно
    for (p = temp + 1; *p; p++) {
        if (*p == '/' || *p == '\\') {
            *p = 0;
            if (_access(temp, 00) != 0) {
                if (_mkdir(temp) != 0 && errno != EEXIST) {
                    return 1;
                }
            }
            *p = '/';
        }
    }
    
    // Создаем финальную папку
    if (_access(temp, 00) != 0) {
        if (_mkdir(temp) != 0 && errno != EEXIST) {
            return 1;
        }
    }
    
    return 0;
}

// Проверяет существование папки под репозитории
int check_directory(char *directory_path){
    printf("> Checking folder preservation for repositories...\n");
    // Проверка существования папки
    if (_access(directory_path, 00) == 0){
        printf("> Directory exist.\n> Welcome to COO.code!\n");
        return 0;
    } else {
        printf("> Desired directory not found. Attempt to create...\n");
        if (mkdir_recursive("repositories") == 0){
            printf(GREEN "> Directory created. Welcome to COO.code!\n" RESET);
            return 0;
        } else {
            perror(RED "[ERROR] Failed to create folder.\n" RESET);
            return 1;
        }
    }
    return 0;
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
    if (mkdir_recursive(repo_path) != 0) {
        perror(RED "[ERROR] Failed to create repository folder\n" RESET);
        return 1;
    }
    
    // Создаём подпапку .coocode для метаданных
    char metadata_path[512];
    snprintf(metadata_path, sizeof(metadata_path), "%s/.coocode", repo_path);
    if (mkdir_recursive(metadata_path) != 0) {
        perror(RED "[ERROR] Failed to create .coocode folder\n" RESET);
        return 1;
    }
    
    // Создаём папку для коммитов
    char commits_path[512];
    snprintf(commits_path, sizeof(commits_path), "%s/.coocode/commits", metadata_path);
    if (mkdir_recursive(commits_path) != 0) {
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
    
    // Создаём папку для файлов (опционально)
    char files_path[512];
    snprintf(files_path, sizeof(files_path), "%s/files", repo_path);
    mkdir_recursive(files_path);
    
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
    
    // Создаём папку для файлов, если её нет
    char files_path[512];
    snprintf(files_path, sizeof(files_path), "%s/files", repo_full_path);
    mkdir_recursive(files_path);
    
    // Формируем путь для копирования файла в репозиторий
    char dest_path[512];
    snprintf(dest_path, sizeof(dest_path), "%s/files/%s", repo_full_path, filename);
    
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
    
    return 0;
}

// Удаляет файл в указанном репозитории
int remove_file(char *repo_name, char *file_path) {
    char full_path[512];
    snprintf(full_path, sizeof(full_path), "./repositories/repo_%s/files/%s", repo_name, file_path);
    
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

// Функции-заглушки с правильными типами возврата
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