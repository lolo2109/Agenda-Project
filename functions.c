#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include <stdbool.h>

cell* createcell(int val,int taille){
    cell* newcell = malloc(sizeof(cell));
    (*newcell).value=val;
    (*newcell).lvl = taille;
    (*newcell).next= (cell**) malloc((taille+1) * sizeof(cell*));
    for(int i = 0; i<=taille; i+=1){
        (*newcell).next[i] = NULL;
    }
    return newcell;
}

void free_cell(cell* cell){
    free((*cell).next);
    free(cell);
}

cell_list* create_cell_list(int lvlmax){
    cell** tab = malloc((lvlmax+1)*sizeof(cell*));
    for(int i = 0; i<=lvlmax; i+=1){
        tab[i] = NULL;
    }
    cell_list* list = (cell_list*) malloc(sizeof(cell_list));
    list->level = lvlmax;
    list->head = tab;
    return list;
}

void cell_listsort_add(cell_list* l, int val, int taille){
    if(taille <= (*l).level){

        if((*l).head[0]->value >= val){
           cell_listhd_add(l,val,taille);
        }
        else {
            cell* c = createcell(val, taille);
            place_before(l,c);
        }

    }
    else printf("cellule trop grande\n");
}

void place_before(cell_list* l, cell* c){
    cell *current_cell = (*l).head[0];
    while (current_cell->value < c->value && current_cell->next[0] != NULL) {
        current_cell = current_cell->next[0];

        }
    if(current_cell->next[0] == NULL && c->value>current_cell->value) {
        current_cell->next[0] = c;
        for(int i= 0; i<=(*c).lvl; i+=1){
            (*c).next[i] = NULL;
    }}
    else{
    if (c->value <= current_cell->value) {
        for(int i= 0; i<=(*c).lvl; i+=1){
            while(i<=current_cell->lvl){
                (*c).next[i] = current_cell;
                break;
            }
            while(current_cell->next[i]==NULL && current_cell->next[0]!=NULL){
                current_cell = current_cell->next[0];
            }
            if(current_cell->next[0]==NULL){
                (*c).next[i] = NULL;
            }
            else{
                (*c).next[i] = current_cell;}
        }
        cell** alloue = malloc(((*l).level+1)*sizeof(cell*));
        for(int i= 0; i<=(*c).lvl; i+=1){
            alloue[i] = c;
        }
        for(int i =(*c).lvl+1; i<= (*l).level; i+=1){
            free(alloue[i]);
        }

    }}
}


void cell_listhd_add(cell_list* l, int val, int taille){
    if(taille <= (*l).level){
        cell* c = createcell(val, taille);
        for(int i= 0; i<=(*c).lvl; i+=1){
            (*c).next[i] = (*l).head[i];
        }
        cell** alloue = malloc(((*l).level+1)*sizeof(cell*));
        for(int i= 0; i<=(*c).lvl; i+=1){
            alloue[i] = c;
        }
        for(int i =(*c).lvl+1; i<= (*l).level; i+=1){
            alloue[i]=(*l).head[i];
        }
        free((*l).head);
        (*l).head = alloue;
    }
    else printf("cellule trop grande\n");
}

void free_cell_profond(cell* c){
    if((*c).next[0] == NULL){
        free_cell(c);
    }
    else{
        free_cell_profond((*c).next[0]);
        free_cell(c);
    }
}

void free_cell_list(cell_list* l){
    if((*l).head[0] == NULL){
        free(l->head);
        free(l);
    }
    else{
        free_cell_profond(l->head[0]);
        free(l->head);
        free(l);
    }
}

void print_list_level(cell_list* l, int lvl){
    if(lvl<=(*l).level){
        printf("[list head_%d @-]", lvl);
        cell* stock_p = (*l).head[lvl];
        while(stock_p != NULL){
            printf("-->[ %d|@-]", (*stock_p).value);
            stock_p = (*stock_p).next[lvl];
        }
        printf("-->NULL\n");
    }
    else printf("pas de level: %d pour la liste donnée\n", lvl);
}

void print_list(cell_list* l){
    for(int i=0; i< (*l).level; i+=1){
        print_list_level(l, i);
    }
}

int cell_list_length(cell_list* l, int i){
    int k = 0;
    cell* stock_p = (*l).head[i];
    while(stock_p != NULL){
        stock_p = (*stock_p).next[i];
        k+=1;
    }
    return k;
}

void print_list_alligne(cell_list* l){
    int len = cell_list_length(l, 0);
    int k = 0;
    int log = 0;
    int count_log= 0;
    int** M = (int**) malloc(((*l).level+1)*sizeof(int*));
    for(int i=0; i<= (*l).level; i+=1){
        M[i] = (int*) malloc(len*sizeof(int));
    }
    cell* stock_p = (*l).head[0];
    printf("[list head_0 @-]");
    while(stock_p != NULL){
        for(int i = 0; i<= (*l).level; i+=1){
            if(i<(*stock_p).lvl+1){
                count_log = 0;
                log = (*stock_p).value;
                while(log > 9){
                    log = log/10;
                    count_log +=1;
                }
                M[i][k]=count_log;
            }
            else{
                M[i][k] = -1;
            }
        }
        printf("-->[ %d|@-]", (*stock_p).value);
        stock_p = (*stock_p).next[0];
        k+=1;
    }
    printf("-->NULL\n");
    for(int i = 1; i<= (*l).level; i+=1){
        printf("[list head_%d @-]", i);
        stock_p = (*l).head[i];
        for(int j = 0; j<len ; j+=1){
            if(M[i][j] == -1){
                printf("----------");
                for(int u = 0; u<M[0][j]; u+=1){
                    printf("-");
                }
            }
            else {
                printf("-->[ %d|@-]", (*stock_p).value);
                stock_p = (*stock_p).next[i];
            }
        }
        printf("-->NULL\n");
    }
    for(int i=0; i<= (*l).level; i+=1){
        free(M[i]);
    }
    free(M);
}
