#include "tree.h"

#include <string.h>

Node* create_file_node(char* name, const char* hash, size_t size){
    Node* newNode = (Node*)malloc(sizeof(Node*));
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
    
    newNode->directory.child_capacity = child_cnt;
    newNode->directory.child_count = 0;
    
    newNode->directory.children = (Node**)malloc(newNode->directory.child_capacity*sizeof(Node));
    return newNode;
}

void update_capacity(Node* node){
    realloc(node->directory.children ,node->directory.child_capacity*sizeof(Node));
}
void add_node(Node* parent, Node* child){
    if (parent->directory.child_capacity > parent->directory.child_count) {
        int child_n = parent->directory.child_count;
        parent->directory.children[child_n] = child;

        parent->directory.child_count++;
    } else {
        parent->directory.child_capacity*=2;
        update_capacity(parent);
    }
}