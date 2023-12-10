#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structures

struct cell_s{
    int value;
    int lvl;
    struct cell_s** next;
};

typedef struct cell_s cell;



struct cell_list_s{
    int level;
    cell** head;
};

typedef struct cell_list_s cell_list;


// Prototypes


cell* createcell(int val, int taille);

void free_cell(cell* cell);

cell_list* create_cell_list(int lvlmax);

void cell_listhd_add(cell_list* l, int val, int talle);

void print_list_level(cell_list* l, int lvl);

void free_cell_list(cell_list* l);

void print_list(cell_list* l);

void print_list_alligne(cell_list* l);

void place_before(cell_list* l, cell* c);

void cell_listsort_add(cell_list* l, int val, int taille);

int* tableau_niveaux (int n);

void list_balanced(cell_list* l, int val);

void search_classic(cell_list* l, int val);
