#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>

typedef enum{
    DIRECTORY,
    FILE
}NodeType;

typedef struct Node{
    NodeType type;
    char *name;
    union {
        struct {
            char *hash;
            size_t size;
        } file;

        struct {
            struct Node **children;
            size_t child_count;
            size_t child_capacity;
        } directory;
    };  
} Node;

Node* create_file_node(char* name, const char* hash, size_t size);
Node* create_directory_node(char* name, size_t child_cnt);
void add_node(Node* parent, Node* child);
void free_tree(Node* head);
void print_tree(Node* head,int depth);
#endif