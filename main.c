// Главный файл проекта
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/file_manager.c"
#include "src/headers/colors.h"

// Глобальная переменная для текущего репозитория
char current_repo[256] = "";

// Функция для отображения меню репозитория
void repository_menu(void) {
    short repo_op = -1;
    short repo_exit = 0;
    
    while (!repo_exit) {
        printf(BLUE "\n|--------| REPOSITORY: %s |--------|\n\n" RESET, current_repo);
        printf("* 1 - add file\n");
        printf("* 2 - remove file\n");
        printf("* 3 - show files\n");
        printf("* 4 - show history\n");
        printf("* 0 - back to main menu\n");
        printf("\nSelect an option:");
        
        short input;
        if (scanf("%hd", &input) != 1) {
            while(getchar() != '\n');
            continue;
        }
        
        switch (input) {
            case 0:
                repo_exit = 1;
                break;
            case 1: {
                char *file_path = malloc(256);
                printf("> Enter file path to add: ");
                scanf("%255s", file_path);
                if (add_file(current_repo, file_path) == 0) {
                    printf(GREEN "> File added successfully!\n" RESET);
                } else {
                    printf(RED "> Failed to add file\n" RESET);
                }
                free(file_path);
                system("pause");
                break;
            }
            case 2: {
                char *file_path = malloc(256);
                printf("> Enter file path to remove: ");
                scanf("%255s", file_path);
                if (remove_file(current_repo, file_path) == 0) {
                    printf(GREEN "> File removed successfully!\n" RESET);
                } else {
                    printf(RED "> Failed to remove file\n" RESET);
                }
                free(file_path);
                system("pause");
                break;
            }
            case 3: {
                printf(YELLOW "\n> Files in repository:\n" RESET);
                // Временная заглушка для показа файлов
                printf("(Function print_files will be implemented)\n");
                system("pause");
                break;
            }
            case 4: {
                printf(YELLOW "\n> Commit history:\n" RESET);
                // Временная заглушка для истории
                printf("(Function print_history will be implemented)\n");
                system("pause");
                break;
            }
            default:
                printf(RED "Invalid option!\n" RESET);
                system("pause");
                break;
        }
        
        if (!repo_exit) {
            system("cls");
            printf(BLUE "COO.code v1.0.0 - Repository: %s\n\n" RESET, current_repo);
        }
    }
}

int main(void){
    printf(BLUE "COO.code v1.0.0 started\n\n" RESET);
    
    // Проверка существования папки с репозиториями
    if (check_directory("./repositories") != 0) {
        printf(RED "Failed to initialize repositories folder!\n" RESET);
        printf("\n> Enter to close:");
        scanf("1");
        return 1;
    }

    short operation_id = -1;
    short exit_flag = 0;

    // Цикл работы
    while (!exit_flag) {
        printf(BLUE "\n|------------| OPERATIONS |------------|\n\n" RESET);
        printf("* 1 - create repository\n");
        printf("* 2 - choose repository\n");
        printf("* 0 - exit\n");
        printf("\nSelect an option: ");
        
        // Ввод
        if (scanf("%hd", &operation_id) != 1) {
            while(getchar() != '\n');
            operation_id = -1;
            continue;
        }
        
        // Обработка операции
        switch (operation_id) {
            case 0:
                exit_flag = 1;
                break;
                
            case 1: {  // Создать репозиторий
                char *repo_name = malloc(21);
                printf("> New repository name (max 20 chars): ");
                scanf("%20s", repo_name);
                
                if (init_repo(repo_name) == 0) {
                    printf(GREEN "> Repository '%s' created successfully!\n" RESET, repo_name);
                    strcpy(current_repo, repo_name);
                    printf(YELLOW "> Now working in repository: %s\n" RESET, current_repo);
                    
                    // Сразу переходим в меню репозитория
                    system("pause");
                    system("cls");
                    repository_menu();
                } else {
                    printf(RED "> Failed to create repository '%s'\n" RESET, repo_name);
                    system("pause");
                }
                
                free(repo_name);
                break;
            }
            
            case 2: {  // Выбрать репозиторий
                char *repo_name = malloc(21);
                printf("> Enter repository name: ");
                scanf("%20s", repo_name);
                
                char repo_path[512];
                snprintf(repo_path, sizeof(repo_path), "./repositories/repo_%s", repo_name);
                
                // Проверяем существование репозитория
                #ifdef _WIN32
                if (_access(repo_path, 00) == 0) {
                #else
                if (access(repo_path, 00) == 0) {
                #endif
                    strcpy(current_repo, repo_name);
                    printf(GREEN "> Now working in repository: %s\n" RESET, current_repo);
                    system("pause");
                    system("cls");
                    repository_menu();
                } else {
                    printf(RED "> Repository '%s' not found!\n" RESET, repo_name);
                    system("pause");
                }
                
                free(repo_name);
                break;
            }
            
            default:
                printf(RED "Invalid option! Please try again.\n" RESET);
                system("pause");
                break;
        }
        
        if (!exit_flag) {
            system("cls");
            printf(BLUE "COO.code v1.0.0\n\n" RESET);
        }
    }

    // Выход
    printf(GREEN "\n> Thank you for using COO.code!\n" RESET);
    printf("\n> Press Enter to close:");
    while(getchar() != '\n');
    getchar();
    return 0;
}