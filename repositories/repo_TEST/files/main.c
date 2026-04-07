// Главный файл проекта
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include "src/file_manager.c"
#include "src/headers/colors.h"

#ifdef _WIN32
#include <windows.h>
#endif

#define VERSION "1.0.0"

// Глобальная переменная для текущего репозитория
char current_repo[256] = "";

// Функция для отображения версии программы
void show_version(void) {
    printf(BLUE "CCD v%s\n" RESET, VERSION);
    printf("A simple version control system\n");
    printf("Created with C programming language\n");
    printf("\nUsage:\n");
    printf("  %s interact - start interactive mode\n", "ccd");
    printf("  %s repo create \"name\" - create repository\n", "ccd");
    printf("  %s repo \"name\" - select repository\n", "ccd");
    printf("  %s add \"file path\" - add file to repository\n", "ccd");
    printf("  %s remove \"filename\" - remove file from repository\n", "ccd");
}

// Функция для проверки валидности имени репозитория
int is_valid_repo_name(const char *name) {
    if (name == NULL || strlen(name) == 0) return 0;
    
    // Проверяем, что имя не состоит только из пробелов
    int has_content = 0;
    for (int i = 0; name[i]; i++) {
        if (!isspace((unsigned char)name[i])) {
            has_content = 1;
            break;
        }
    }
    return has_content;
}

// Функция для загрузки сохраненного репозитория
int load_current_repo(void) {
    FILE *config = fopen("./repositories/.current_repo", "r");
    if (config) {
        fgets(current_repo, sizeof(current_repo), config);
        // Убираем символ новой строки
        char *newline = strchr(current_repo, '\n');
        if (newline) *newline = '\0';
        fclose(config);
        return 1;
    }
    return 0;
}

// Функция для сохранения текущего репозитория
void save_current_repo(void) {
    FILE *config = fopen("./repositories/.current_repo", "w");
    if (config) {
        fprintf(config, "%s", current_repo);
        fclose(config);
    }
}

// Функция для отображения меню репозитория
void repository_menu(void) {
    short repo_exit = 0;
    
    while (!repo_exit) {
        printf(BLUE "\n|--------| REPOSITORY: %s |--------|\n\n" RESET, current_repo);
        printf("* 1 - add file\n");
        printf("* 2 - remove file\n");
        printf("* 3 - show files\n");
        printf("* 4 - show history\n");
        printf("* 0 - back to main menu\n");
        printf("\nSelect option: ");
        
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
                char file_path[256];
                printf("> Enter file path to add: ");
                scanf("%255s", file_path);
                if (add_file(current_repo, file_path) == 0) {
                    printf(GREEN "> File added successfully!\n" RESET);
                } else {
                    printf(RED "> Failed to add file\n" RESET);
                }
                system("pause");
                break;
            }
            case 2: {
                char file_path[256];
                printf("> Enter filename to remove: ");
                scanf("%255s", file_path);
                if (remove_file(current_repo, file_path) == 0) {
                    printf(GREEN "> File removed successfully!\n" RESET);
                } else {
                    printf(RED "> Failed to remove file\n" RESET);
                }
                system("pause");
                break;
            }
            case 3: {
                printf(YELLOW "\n> Files in repository:\n" RESET);
                print_files(NULL); // Временно передаем NULL
                system("pause");
                break;
            }
            case 4: {
                printf(YELLOW "\n> Commit history:\n" RESET);
                print_history(NULL); // Временно передаем NULL
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
            printf(BLUE "CCD v%s - Repository: %s\n\n" RESET, VERSION, current_repo);
        }
    }
}

// Функция для интерактивного режима
void interactive_mode(void) {
    system("cls");
    printf(BLUE "CCD v%s - Interactive Mode\n\n" RESET, VERSION);
    
    // Проверка существования папки с репозиториями
    if (check_directory("./repositories") != 0) {
        printf(RED "Failed to initialize repositories folder!\n" RESET);
        printf("\n> Press Enter to exit:");
        getchar();
        return;
    }

    short operation_id = -1;
    short exit_flag = 0;

    // Цикл работы
    while (!exit_flag) {
        printf(BLUE "\n|------------| OPERATIONS |------------|\n\n" RESET);
        printf("* 1 - create repository\n");
        printf("* 2 - select repository\n");
        printf("* 0 - exit\n");
        printf("\nSelect option: ");
        
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
                char repo_name[256];
                printf("> New repository name (max 255 chars): ");
                scanf("%255s", repo_name);
                
                // Проверяем, что имя не пустое
                if (!is_valid_repo_name(repo_name)) {
                    printf(RED "Error: repository name cannot be empty!\n" RESET);
                    system("pause");
                    break;
                }
                
                if (init_repo(repo_name) == 0) {
                    printf(GREEN "> Repository '%s' created successfully!\n" RESET, repo_name);
                    strcpy(current_repo, repo_name);
                    save_current_repo();
                    printf(YELLOW "> Now working in repository: %s\n" RESET, current_repo);
                    
                    // Сразу переходим в меню репозитория
                    system("pause");
                    system("cls");
                    repository_menu();
                } else {
                    printf(RED "> Failed to create repository '%s'\n" RESET, repo_name);
                    system("pause");
                }
                break;
            }
            
            case 2: {  // Выбрать репозиторий
                char repo_name[256];
                printf("> Enter repository name: ");
                scanf("%255s", repo_name);
                
                // Проверяем, что имя не пустое
                if (!is_valid_repo_name(repo_name)) {
                    printf(RED "Error: repository name cannot be empty!\n" RESET);
                    system("pause");
                    break;
                }
                
                char repo_path[512];
                snprintf(repo_path, sizeof(repo_path), "./repositories/repo_%s", repo_name);
                
                // Проверяем существование репозитория
                #ifdef _WIN32
                if (_access(repo_path, 00) == 0) {
                #else
                if (access(repo_path, 00) == 0) {
                #endif
                    strcpy(current_repo, repo_name);
                    save_current_repo();
                    printf(GREEN "> Now working in repository: %s\n" RESET, current_repo);
                    system("pause");
                    system("cls");
                    repository_menu();
                } else {
                    printf(RED "> Repository '%s' not found!\n" RESET, repo_name);
                    system("pause");
                }
                break;
            }
            
            default:
                printf(RED "Invalid option! Please try again.\n" RESET);
                system("pause");
                break;
        }
        
        if (!exit_flag) {
            system("cls");
            printf(BLUE "CCD v%s - Interactive Mode\n\n" RESET, VERSION);
        }
    }
    
    // Выход из интерактивного режима
    printf(GREEN "\n> Exiting interactive mode...\n" RESET);
}

// Функция для обработки командной строки
int process_command_line(int argc, char *argv[]) {
    if (argc < 2) {
        show_version();
        return 0;
    }
    
    // Интерактивный режим
    if (strcmp(argv[1], "interact") == 0) {
        interactive_mode();
        return 0;
    }
    
    if (strcmp(argv[1], "repo") == 0) {
        if (argc < 3) {
            printf(RED "Error: insufficient arguments for repo command\n" RESET);
            printf("Usage:\n");
            printf("  %s repo create \"name\" - create repository\n", "ccd");
            printf("  %s repo \"name\" - select repository\n", "ccd");
            return 1;
        }
        
        if (strcmp(argv[2], "create") == 0) {
            if (argc < 4) {
                printf(RED "Error: specify repository name\n" RESET);
                return 1;
            }
            
            // Проверяем, что название не пустое
            if (!is_valid_repo_name(argv[3])) {
                printf(RED "Error: repository name cannot be empty!\n" RESET);
                return 1;
            }
            
            // Создаем репозиторий
            if (init_repo(argv[3]) == 0) {
                printf(GREEN "Repository '%s' created successfully!\n" RESET, argv[3]);
                strcpy(current_repo, argv[3]);
                save_current_repo();
            } else {
                printf(RED "Failed to create repository '%s'\n" RESET, argv[3]);
                return 1;
            }
        } else {
            // Выбор репозитория
            if (!is_valid_repo_name(argv[2])) {
                printf(RED "Error: repository name cannot be empty!\n" RESET);
                return 1;
            }
            
            char repo_path[512];
            snprintf(repo_path, sizeof(repo_path), "./repositories/repo_%s", argv[2]);
            
            #ifdef _WIN32
            if (_access(repo_path, 00) == 0) {
            #else
            if (access(repo_path, 00) == 0) {
            #endif
                strcpy(current_repo, argv[2]);
                save_current_repo();
                printf(GREEN "Now working in repository: %s\n" RESET, current_repo);
            } else {
                printf(RED "Repository '%s' not found!\n" RESET, argv[2]);
                return 1;
            }
        }
    }
    else if (strcmp(argv[1], "add") == 0) {
        if (argc < 3) {
            printf(RED "Error: specify file path\n" RESET);
            printf("Usage: %s add \"file path\"\n", "ccd");
            return 1;
        }
        
        // Загружаем текущий репозиторий
        if (!load_current_repo() || strlen(current_repo) == 0) {
            printf(RED "Error: no active repository. Please select or create a repository first.\n" RESET);
            return 1;
        }
        
        if (add_file(current_repo, argv[2]) == 0) {
            printf(GREEN "File added successfully to repository '%s'!\n" RESET, current_repo);
        } else {
            printf(RED "Failed to add file\n" RESET);
            return 1;
        }
    }
    else if (strcmp(argv[1], "remove") == 0) {
        if (argc < 3) {
            printf(RED "Error: specify filename\n" RESET);
            printf("Usage: %s remove \"filename\"\n", "ccd");
            return 1;
        }
        
        // Загружаем текущий репозиторий
        if (!load_current_repo() || strlen(current_repo) == 0) {
            printf(RED "Error: no active repository. Please select or create a repository first.\n" RESET);
            return 1;
        }
        
        if (remove_file(current_repo, argv[2]) == 0) {
            printf(GREEN "File removed successfully from repository '%s'!\n" RESET, current_repo);
        } else {
            printf(RED "Failed to remove file\n" RESET);
            return 1;
        }
    }
    else {
        printf(RED "Unknown command: %s\n" RESET, argv[1]);
        printf("\nAvailable commands:\n");
        printf("  %s interact - start interactive mode\n", "ccd");
        printf("  %s repo create \"name\" - create repository\n", "ccd");
        printf("  %s repo \"name\" - select repository\n", "ccd");
        printf("  %s add \"file path\" - add file to repository\n", "ccd");
        printf("  %s remove \"filename\" - remove file from repository\n", "ccd");
        return 1;
    }
    
    return 0;
}

int main(int argc, char *argv[]){
    // Устанавливаем русскую локаль для поддержки кириллицы в комментариях
    setlocale(LC_ALL, "ru_RU.UTF-8");
    #ifdef _WIN32
    SetConsoleOutputCP(65001); // UTF-8 для Windows
    SetConsoleCP(65001);
    #endif
    
    // Проверяем аргументы командной строки
    if (argc > 1) {
        return process_command_line(argc, argv);
    }
    
    // Если аргументов нет - показываем версию
    show_version();
    return 0;
}