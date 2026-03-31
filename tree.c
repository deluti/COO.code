#include "tree.h"

#include <string.h>

Node* create_file_node(char* name, const char* hash, size_t size){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->type = FILE;

    newNode->name = malloc(strlen(name)+1);
    strcpy(newNode->name,name);

    newNode->file.hash = malloc(strlen(hash)+1);
    strcpy(newNode->file.hash,hash);
    newNode->file.size = size;

    return newNode;
}


Node* create_directory_node(char* name, size_t child_cnt){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->type = DIRECTORY;

    newNode->name = malloc(strlen(name)+1);
    strcpy(newNode->name,name);

    newNode->directory.child_count = child_cnt;

    newNode->directory.children = (Node**)malloc(newNode->directory.child_count*sizeof(Node*));
    return newNode;
}