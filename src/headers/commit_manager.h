// Тут так называемые коммиты и все что с этим связано
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Структура для коммита
typedef struct Commit {
    char hash[41];          // SHA-1 хеш (40 символов + \0)
    char parent_hash[41];   // Хеш родительского коммита
    char message[256];      // Сообщение коммита
    char date[64];          // Дата создания
    char tree_hash[41];     // Хеш дерева файлов
    struct Commit *next;    // Для списка
} Commit;

// Функции для работы с коммитами
Commit* create_commit(const char *parent_hash, const char *message, const char *tree_hash);
void free_commit(Commit *commit);
void save_commit(Commit *commit, const char *repo_path);
Commit* load_commit(const char *repo_path, const char *hash);
void generate_commit_hash(Commit *commit);