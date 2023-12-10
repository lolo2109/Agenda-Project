#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include <stdbool.h>


// Creation d'une cellule et initialisation de ses pointeurs
cell* createcell(int val,int taille){
    cell* newcell = malloc(sizeof(cell));
    (*newcell).value=val;
    (*newcell).lvl = taille;
    (*newcell).next= (cell**) malloc((taille+1) * sizeof(cell*)); // tableau dynamique de la taille du niveau ùax de la cellule
    for(int i = 0; i<=taille; i+=1){
        (*newcell).next[i] = NULL;
    }
    return newcell;
}


void free_cell(cell* cell){
    free((*cell).next);
    free(cell);
}


// Creation d'une liste en fonction de son niveau max, tableau dynamique tab assigné au head
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


// Ajoute une cellule à sa position correcte en fonction de sa valeur (ordre croissant)
void cell_listsort_add(cell_list* l, int val, int taille){
    if(taille <= (*l).level){

        if((*l).head[0]->value >= val){
           cell_listhd_add(l,val,taille); // appel à cell_listhd_add pour ajouter en tête
        }
        else {
            cell* c = createcell(val, taille);
            place_before(l,c); // appel à place_before pour ajouter avant la première cellule de valeur supérieure ou égale à val
        }
    }
    else printf("cellule trop grande\n");
}


// Place la cellule avant la première cellule de valeur supérieure ou égale à val
void place_before(cell_list* l, cell* c){
    cell *current_cell = (*l).head[0]; // Cellule courante initialisée au head

    // Tant que la valeur courante est inférieure à la valeur de la cellule
    // à insérer et que ce n'est pas la dernière cellule, la cellule courante avance
    while (current_cell->value < c->value && current_cell->next[0] != NULL) { 
        current_cell = current_cell->next[0];
        }

    
    // Cas où la cellule est à insérer à la fin de la liste
    if(current_cell->next[0] == NULL && c->value>current_cell->value) {
        current_cell->next[0] = c;
        for(int i= 0; i<=(*c).lvl; i+=1){
            (*c).next[i] = NULL;
    }}

    // Cas classique ( cellule dans le corps de la liste )
    else{
        for(int i= 0; i<=(*c).lvl; i+=1){ // Parcours des niveaux de la cellule
            while(i<=current_cell->lvl){ // Cas où les niveaux sont inf ou égaux à ceux de la cellule devant la cellule a placer
                (*c).next[i] = current_cell;
                break;
            }
            while(current_cell->lvl<=i && current_cell->next[0]!=NULL){ // Recherche des prochaines cellules sur le meme niveau actuel
                current_cell = current_cell->next[0];
            }
            if(current_cell->next[0]==NULL){ // Si aucune autre cellule sur le meme niveau -> pointeur sur NULL
                (*c).next[i] = NULL;
            }
            else{
                (*c).next[i] = current_cell;}
        }

        // Allocation mémoire des niveaux de la cellule
        cell** alloue = malloc(((*l).level+1)*sizeof(cell*)); 
        for(int i= 0; i<=(*c).lvl; i+=1){ 
            alloue[i] = c;
        }
        for(int i =(*c).lvl+1; i<= (*l).level; i+=1){
            free(alloue[i]);
            (*c).next = alloue;
        }
    }
}


// Ajout d'une cellule ne tête de liste
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


// Affichage d'un niveau de la liste
void print_list_level(cell_list* l, int lvl){
    if(lvl<=(*l).level){
        printf("[list head_%d @-]", lvl);
        cell* stock_p = (*l).head[lvl];
        while(stock_p != NULL){ // Parcours de la liste jusqu'à la fin du niveau
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

// Donne le nombre de cellules sur un niveau i donné
int cell_list_length(cell_list* l, int i){
    int k = 0;
    cell* stock_p = (*l).head[i];
    while(stock_p != NULL){ // Parcours de la liste jusqu'à la fin du niveau
        stock_p = (*stock_p).next[i];
        k+=1;
    }
    return k;
}

// Affiche la liste en alignant les cellules verticalement
void print_list_alligne(cell_list* l){
    int len = cell_list_length(l, 0); // On récupère la longueur de la liste

    // Variables initialisées
    int k = 0;
    int log = 0;
    int count_log= 0;

    // Allocation mémoire 
    int** M = (int**) malloc(((*l).level+1)*sizeof(int*));
    for(int i=0; i<= (*l).level; i+=1){
        M[i] = (int*) malloc(len*sizeof(int));
    }
    cell* stock_p = (*l).head[0];
    printf("[list head_0 @-]");
    while(stock_p != NULL){ // Parcours de la liste
        // Matrice M remplie avec les count_log
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
        // Affichage du niveau 0
        printf("-->[ %d|@-]", (*stock_p).value); 
        stock_p = (*stock_p).next[0];
        k+=1;
    }
    printf("-->NULL\n");

    // Affichage des autres niveaux
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

//fonction qui crée le tableau qui définiera la profondeur des cellules
int* tableau_niveaux (int n) {
    int compte = 1;
    int taille = (pow(2,n) - 1); //taille de la liste
    int* tab = (int *) calloc(taille , sizeof(int)); //définition du tableau initialisé avec des zéros
    for (int i = 1; i < n; i++) {
        while (compte < (taille)) {
            int temp = pow(2,i);
            if ((compte+1) % (temp) == 0) { 
                tab[compte]++;  //si le numéro de la case du tableau et divisible par 2^i, alors cette case prend +1
            }
            compte++;
        }
        compte = 0;
    }
    tab[taille]=-1;
    return tab;
}

//fonction ajoute les cellules avec leur profondeur définies par la fonction précédente à une liste
void list_balanced(cell_list* l, int val){
    int* niveaux = tableau_niveaux(val); //définition du tableau
    int taille = pow(2,val)-1;
    for (int i = 0; i < taille ; i++){
        cell_listhd_add(l, taille-i, niveaux[i]); //ajout des cellules
    }
}

//fonction de recherche classique qui parcours le niveau 0
void search_classic(cell_list* l, int val){
    cell* temp=(*l).head[0]; //pointeur initialisé au niveau 0
    int stop = pow(2,(*l).level)-1; 
    if (val > stop){
        printf("the value %d is not found in the list\n", val); //vérification de la présence de la valeur recherchée
    }
    else{
        for (int i = 0; i < stop; i++) {
            if ((*temp).value == val) {
                printf("the value %d is at the index %d\n", val, i);
                break;
            }
            temp = (*temp).next[0]; //incrémentation du pointeur qui parcours les cellules
        }
    }
}



