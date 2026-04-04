#include "headers/commit_manager.h"
#include "headers/colors.h"
#include <string.h>
#include <stdio.h>

// Генерация простого хеша для коммита
void generate_commit_hash(Commit *commit) {
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%s%s%s%s", 
             commit->parent_hash, commit->message, commit->date, commit->tree_hash);
    
    unsigned long hash = 5381;
    int c;
    char *ptr = buffer;
    while ((c = *ptr++)) {
        hash = ((hash << 5) + hash) + c;
    }
    snprintf(commit->hash, 41, "%lx", hash);
}

// Создание нового коммита
Commit* create_commit(const char *parent_hash, const char *message, const char *tree_hash) {
    Commit *commit = (Commit*)malloc(sizeof(Commit));
    if (!commit) return NULL;
    
    // Получаем текущую дату
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(commit->date, sizeof(commit->date), "%Y-%m-%d %H:%M:%S", t);
    
    // Копируем данные
    strncpy(commit->parent_hash, parent_hash ? parent_hash : "", 40);
    strncpy(commit->message, message, 255);
    strncpy(commit->tree_hash, tree_hash ? tree_hash : "", 40);
    commit->next = NULL;
    
    // Генерируем хеш
    generate_commit_hash(commit);
    
    return commit;
}

// Освобождение памяти
void free_commit(Commit *commit) {
    if (commit) free(commit);
}