#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>

// GLOBAL DECLARATIONS

#define A 64
#define B 65
#define E 10
int last_el=0;
int rep;
unsigned int glob = 0;

//END OF GLOBAL DECLARATIONS

typedef struct inbound_rels {
    char *o_entity;
    struct inbound_rels *next;
} inbound_rels;

typedef struct outbound_rels {
    struct entity *d_entity;
    struct outbound_rels *next;
} outbound_rels;

typedef struct relations {
    char *name;
    struct relations *next;
    struct inbound_rels *i_rel;
    struct outbound_rels * o_rel;
    int counter;
} relations;

typedef relations *r;

typedef struct entity {
    struct entity *next;
    r rels[E];
    char *name;
} entity;

typedef struct keys {
    char *ID_rel;
    int key;
    int counter;
} keys;

typedef struct report_structure {
    char *name;
    int counter;
    struct inbound_rels * rels;
} report_structure;

typedef entity *e;
typedef inbound_rels * ir;
typedef outbound_rels * or;
typedef keys * k;
typedef report_structure * rs;

int search_letters (char letter, int cases) {

    if (cases == 1) {
        int letter_id;
        if ((int)letter==45) {
        letter_id = 0;
        }

        else if ((int)letter>=48 && (int)letter<=57)
        letter_id = (int) letter - 47;

        else if ((int)letter >= 65 && (int)letter <= 90) {
            letter_id = (int)letter - 54;
        }

        else if ((int)letter==95) {
            letter_id = 37;
        }

        else if ((int)letter>=97 && (int)letter <= 122) {
            letter_id = (int) letter - 59;
        }
        return letter_id;
    }

    if (cases == 2) {
        int letter_id;

        if(letter== '\0') {
            letter_id = 0;
        }

        else if ((int)letter==45) {
        letter_id = 1;
        }

        else if ((int)letter>=48 && (int)letter<=57) {
            letter_id = (int) letter - 46;
        }

        else if ((int)letter >= 65 && (int)letter <= 90) {
            letter_id = (int)letter - 53;
        }

        else if ((int)letter==95) {
            letter_id = 38;
        }

        else if ((int)letter>=97 && (int)letter <= 122) {
            letter_id = (int) letter - 58;
        }
        return letter_id;
    }
}

e add_ent(char * ID, e list) {

    if (list==NULL) {
        list = calloc(1, sizeof(entity));
        list->name = malloc((strlen(ID) +1)*sizeof(char));
        strcpy(list->name, ID);
        return list;
    }

    else if (strcmp(ID, list->name) < 0) {
        e head = calloc(1, sizeof(entity));
        head->name = malloc((strlen(ID) +1) * sizeof(char) + 1);
        strcpy(head->name, ID);
        head->next = list;
        list = head;
        return list;
    }

    e count=list;
    while (count->next!=NULL) {
        if (strcmp(ID, count->name)==0) return list;
        else if (strcmp(ID, count->next->name) < 0) {
            e temp = calloc(1, sizeof(entity));
            temp->name = malloc((strlen(ID) +1) * sizeof(char *));
            strcpy(temp->name, ID);
            temp->next = count->next;
            count->next = temp;
            return list;
        }
        count = count->next;
    }
    if (strcmp(ID, count->name)==0) return list;
    count->next = calloc (1, sizeof(entity));
    count = count->next;
    count->name = malloc((strlen(ID) +1)*sizeof(char *));
    strcpy(count->name, ID);
    return list;
}

void InsertQueue(ir list, char * name) {
    ir curr = list;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = calloc(1, sizeof(inbound_rels));
    curr = curr->next;
    curr->o_entity = malloc((strlen(name) +1) * sizeof(char));
    strcpy(curr->o_entity, name);
}

void sPurga(ir head) {
    if (head==NULL) return;
    sPurga(head->next);
    free(head->o_entity);
    free(head);
}

rs BuildReport(rs structure, e list, int pos) {
    if (list->rels[pos]!=NULL) {
        if (list->rels[pos]->counter==0) return structure;
        rs count = structure;
        //printf("\nCOUNT COUNTER: %d", count->counter);
        if (count->counter == 0) {
            //free(count->rels->o_entity);
            count->rels = calloc(1, sizeof(inbound_rels));
            count->rels->o_entity = malloc((strlen(list->name) +1)*sizeof(char));
            strcpy(count->rels->o_entity, list->name);
            count->counter = list->rels[pos]->counter;
            //count->rels->next = NULL;
            return  structure;
        }
        if (count->rels!=NULL && list->rels[pos]->counter > count->counter) {
            //printf("\nCOUNTER REPORT 1: %d", list->rels[key]->counter);
            if (count->rels->next!=NULL) sPurga(count->rels->next);
            free(count->rels->o_entity);
            count->rels->o_entity = malloc((strlen(list->name) +1) * sizeof(char));
            strcpy(count->rels->o_entity, list->name);
            count->rels->next = NULL;
            count->counter = list->rels[pos]->counter;
            return structure;
        }
        else if (list->rels[pos]->counter == count->counter) {
            //printf("\nCOUNTER REPORT 2: %d", list->rels[key]->counter);
            InsertQueue(count->rels, list->name);
            return structure;
        }
        else if (list->rels[pos]->counter < count->counter) {
            //printf("\n\nSTRUCTURE: %d", structure[pos]->counter);
            return structure;
        }
        else{
            //printf("\nCOUNTER REPORT: %d", list->rels[key]->counter);
            count->rels = calloc(1, sizeof(inbound_rels));
            count->rels->o_entity = malloc((strlen(list->name) +1) * sizeof(char));
            strcpy(count->rels->o_entity, list->name);
            count->rels->next = NULL;
            count->counter = list->rels[pos]->counter;
            return structure;
        }
    }
    return structure;
}

rs check_report_del (rs reporter, char * ID, e list[A][B], int pos) {
    if (reporter->rels!=NULL) {
        if (reporter->counter==0) return reporter;
        if (strcmp(reporter->rels->o_entity, ID) == 0) {
            if (reporter->rels->next != NULL) {
                ir prev = reporter->rels;
                reporter->rels = reporter->rels->next;
                free(prev->o_entity);
                free(prev);
                return reporter;
            } else if (reporter->rels->next == NULL) {
                free(reporter->rels->o_entity);
                free(reporter->rels);
                reporter->rels = NULL;
                reporter->counter = 0;
                for (int i = 0; i < A; i++) {
                    for (int j = 0; j < B; j++) {
                        if (list[i][j] != NULL) {
                            e count = list[i][j];
                            while (count != NULL) {
                                reporter = BuildReport(reporter, count, pos);
                                count = count->next;
                            }
                        }
                    }
                }
                return reporter;
            }
           }
    }

    ir count = reporter->rels;
    ir prev = count;
    while (count!=NULL) {
        if (strcmp(count->o_entity, ID) == 0) {
            prev->next = count->next;
            free(count->o_entity);
            free(count);
            return reporter;
        }
        prev = count;
        count = count->next;
    }
    return reporter;
}

ir funz (ir list, r rels, char * ID, k key) {
    ir temp = list;
    if (strcmp(temp->o_entity, ID)==0) {
        list = list->next;
        free(temp->o_entity);
        free(temp);
        rels->counter--;
        key->counter--;
        if (key->counter == 0) rep--;
        return list;
    }

    ir previo = list;

    while (temp!=NULL) {
        if (strcmp(temp->o_entity, ID)==0) {
            previo->next = temp->next;
            free(temp->o_entity);
            free(temp);
            rels->counter--;
            key->counter--;
            if (key->counter == 0) rep--;
            return list;
        }
        previo = temp;
        temp = temp->next;
    }
    return list;
}

or del_single_rel(or list, char * dest) {
    or count = list, prev=list;
    if (strcmp(count->d_entity->name, dest)==0) {
        list = list->next;
        free(count);
        return list;
    }
    while (count!=NULL) {
        if (strcmp(count->d_entity->name, dest)==0) {
            prev->next = count->next;
            free(count);
            return list;
        }
        prev = count;
        count = count->next;
    }
    return list;
}

void del2(char * ID, e list[A][B], int i, char * dest) {
    char letter;
    int letter_id1, letter_id2;
    letter = ID[0];
    letter_id1 = search_letters(letter, 1);
    letter = ID[1];
    letter_id2 = search_letters(letter, 2);

    e count = list[letter_id1][letter_id2];
    while (count!=NULL) {
        if (strcmp(count->name, ID)==0) break;
        count = count->next;
    }
    if (count->rels[i]->o_rel!=NULL) count->rels[i]->o_rel = del_single_rel(count->rels[i]->o_rel, dest);

}

e del_single_ent(char *ID, e head, k key[], rs reporter[], e list[A][B]) {
    e cur, prev;
    cur = head;

    if (head!=NULL && strcmp(head->name, ID)==0) {
        for (int i=0; i<E && key[i]!=NULL; i++) {
            if (cur->rels[key[i]->key]!=NULL) {
                or cur2 = cur->rels[key[i]->key]->o_rel, prev2 = cur->rels[key[i]->key]->o_rel;
                while (cur2 != NULL) {
                    cur2->d_entity->rels[key[i]->key]->i_rel = funz(cur2->d_entity->rels[key[i]->key]->i_rel, cur2->d_entity->rels[key[i]->key], ID, key[i]);
                    reporter[key[i]->key] = check_report_del(reporter[key[i]->key], cur2->d_entity->name, list, key[i]->key);
                    cur2 = cur2->next;
                    free(prev2);
                    prev2 = cur2;
                }
                ir cur3 = cur->rels[key[i]->key]->i_rel, prev3 = cur->rels[key[i]->key]->i_rel;
                while (cur3 != NULL) {
                    del2(cur3->o_entity, list, key[i]->key, ID);
                    cur3 = cur3->next;
                    free(prev3->o_entity);
                    free(prev3);
                    prev3 = cur3;
                }
                cur->rels[key[i]->key]->counter = 0;
                free(cur->rels[key[i]->key]->name);
                free(cur->rels[key[i]->key]);
                reporter[key[i]->key] = check_report_del(reporter[key[i]->key], ID, list, key[i]->key);
            }
        }
        head = head->next;
        free(cur->name);
        free(cur);
        return head;
    }

    prev = head;

    while (cur!=NULL) {             //Search for the correct entity in the list
        if (strcmp(cur->name, ID)==0) {
            for (int i=0; i<E && key[i]!=NULL; i++) {
                if (cur->rels[key[i]->key]!=NULL) {
                    or cur2 = cur->rels[key[i]->key]->o_rel, prev2 = cur->rels[key[i]->key]->o_rel;
                    while (cur2 != NULL) {
                        cur2->d_entity->rels[key[i]->key]->i_rel = funz(cur2->d_entity->rels[key[i]->key]->i_rel, cur2->d_entity->rels[key[i]->key], ID, key[i]);
                        reporter[key[i]->key] = check_report_del(reporter[key[i]->key], cur2->d_entity->name, list, key[i]->key);
                        cur2 = cur2->next;
                        free(prev2);
                        prev2 = cur2;
                    }
                    ir cur3 = cur->rels[key[i]->key]->i_rel, prev3 = cur->rels[key[i]->key]->i_rel;
                    while (cur3 != NULL) {
                        del2(cur3->o_entity, list, key[i]->key, ID);
                        cur3 = cur3->next;
                        free(prev3->o_entity);
                        free(prev3);
                        prev3 = cur3;
                    }
                    cur->rels[key[i]->key]->counter = 0;
                    free(cur->rels[key[i]->key]->name);
                    free(cur->rels[key[i]->key]);
                    reporter[key[i]->key] = check_report_del(reporter[key[i]->key], ID, list, key[i]->key);
                }
            }
            prev->next = cur->next;
            free(cur->name);
            free(cur);

            return head;
        }

        prev = cur;
        cur = cur->next;
    }

    return head;
}

or insert_rel_o (or list, e count) {
    if (list == NULL) {
        list = calloc(1, sizeof(outbound_rels));
        list->d_entity = count;
        return list;
    }
    else {
        or head;
        head = calloc(1, sizeof(outbound_rels));
        head->d_entity = count;
        head->next = list;
        list = head;
        return list;
    }
}

int insert_relation(r list, char * ID_orig, r outbound, e dest) {
    ir count = list->i_rel;

    while (count->next != NULL) {
        if (strcmp(ID_orig, count->o_entity) == 0) return 0;
        if (strcmp(ID_orig, count->next->o_entity) < 0) {
            ir temp = calloc(1, sizeof(inbound_rels));
            temp->o_entity = malloc((strlen(ID_orig) +1) * sizeof(char));
            strcpy(temp->o_entity, ID_orig);
            outbound->o_rel = insert_rel_o(outbound->o_rel, dest);
            temp->next = count->next;
            count->next = temp;
            return 1;
        }
        count = count->next;
    }
    if (strcmp(ID_orig, count->o_entity) == 0) return 0;
    count->next = calloc(1, sizeof(inbound_rels));
    count = count->next;
    count->o_entity = malloc((strlen(ID_orig) +1) * sizeof(char));
    strcpy(count->o_entity, ID_orig);
    outbound->o_rel = insert_rel_o(outbound->o_rel, dest);
    return 1;
}

rs check_report (rs reporter, int counter, char * ID, char * ID_rel) {

    if (reporter == NULL) {
        reporter = calloc(1, sizeof(report_structure));
        reporter->name = malloc((strlen(ID_rel) +1) * sizeof(char));
        strcpy(reporter->name, ID_rel);
        reporter->rels = calloc(1, sizeof(inbound_rels));
        reporter->rels->o_entity = malloc((strlen(ID) +1) * sizeof(char));
        strcpy(reporter->rels->o_entity, ID);
        reporter->counter = counter;
        return reporter;
    }

    else if (counter < reporter->counter) return reporter;

    else if (counter == reporter->counter) {
        if (strcmp(ID, reporter->rels->o_entity)<0) {
            ir head;
            head = calloc(1, sizeof(inbound_rels));
            head->o_entity = malloc(strlen(ID) * sizeof(char) + 1);
            strcpy(head->o_entity, ID);
            head->next = reporter->rels;
            reporter->rels = head;
            return reporter;
        }
        ir count = reporter->rels;
        while (count->next!=NULL) {
            if (strcmp(ID, count->next->o_entity)<0) {
                ir temp = calloc(1, sizeof(inbound_rels));
                temp->o_entity = malloc((strlen(ID) +1) * sizeof(char));
                strcpy(temp->o_entity, ID);
                temp->next = count->next;
                count->next = temp;
                return reporter;
            }
            count = count->next;
        }
        count->next = calloc(1, sizeof(inbound_rels));
        count = count->next;
        count->o_entity = malloc((strlen(ID) + 1) * sizeof(char));
        strcpy(count->o_entity, ID);
        return reporter;
    }

    else if (counter > reporter->counter) {
        ir count = reporter->rels;
        if (count==NULL) {
            count = calloc(1, sizeof(inbound_rels));
            count->o_entity = malloc((strlen(ID) + 1)*sizeof(char));
            strcpy(count->o_entity, ID);
            reporter->counter = counter;
            reporter->rels = count;
            return reporter;
        }
        if (count->next == NULL && strcmp(count->o_entity, ID)==0) {
            reporter->counter = counter;
            return reporter;
        }
        count = count->next;
        ir prev = NULL;
        while (count != NULL) {
            prev = count;
            count = count->next;
            free(prev->o_entity);
            free(prev);
        }
        reporter->rels->next = NULL;
        free(reporter->rels->o_entity);
        reporter->rels->o_entity = malloc((strlen(ID) + 1)*sizeof(char));
        strcpy(reporter->rels->o_entity, ID);
        reporter->counter = counter;
        return reporter;
    }
    return reporter;
}

r insert_rel_i(r list, char * ID_orig, k key, r outbound, e dest) {		//Relation insertion function		//FUNZIONA
    ir count = list->i_rel;
    if (list->i_rel==NULL) {
        list->i_rel = calloc(1, sizeof(inbound_rels));
        list->i_rel->o_entity = malloc((strlen(ID_orig) +1) * sizeof(char));
        strcpy(list->i_rel->o_entity, ID_orig);
        outbound->o_rel = insert_rel_o(outbound->o_rel, dest);
        list->counter++;
        if (key->counter == 0) rep++;
        key->counter++;
        glob = 1;
        return list;
    }
    else if (strcmp(ID_orig, count->o_entity) < 0) {
        ir head;
        head = calloc(1,sizeof(inbound_rels));
        head->o_entity = malloc(strlen(ID_orig) * sizeof(char) + 1);
        strcpy(head->o_entity, ID_orig);
        outbound->o_rel = insert_rel_o(outbound->o_rel, dest);
        head->next = list->i_rel;
        list->i_rel = head;
        list->counter++;
        if (key->counter == 0) rep++;
        key->counter++;
        glob = 1;
        return list;
    }
    if (insert_relation(list, ID_orig, outbound, dest)==1) {
        list->counter++;
        if (key->counter == 0) rep++;
        key->counter++;
        glob = 1;
    }
    else {
        glob = 0;
        return list;
    }
    return list;

}

r add_rel_type (r list, char * ID_rel) {
    list = calloc(1, sizeof(relations));
    list->name = malloc((strlen(ID_rel) +1) * sizeof(char));
    strcpy(list->name, ID_rel);
    return list;
}

void add_key (k key[], char * ID_rel, int j, int pos) {
    key[pos] = calloc(1, sizeof(keys));
    key[pos]->ID_rel = malloc((strlen(ID_rel) +1) * sizeof(char));
    strcpy(key[pos]->ID_rel, ID_rel);
    key[pos]->key = j;
    last_el++;
}

void key_swap (k key[], int i) {
    if (key[i + 1]->ID_rel != NULL) {
        key_swap(key, i+1);
    }
    key[i + 1] = key[i];
}

void check_index (e count2, k key[], char * ID_rel, char * ID_orig, char * ID_dest, rs reporter[], e count1) {
    int j;

    if (key[0] == NULL) {
        add_key(key, ID_rel, 0, 0);
        count2->rels[0] = add_rel_type(count2->rels[0], ID_rel);
        count1->rels[0] = add_rel_type(count1->rels[0], ID_rel);
        count2->rels[0] = insert_rel_i(count2->rels[0], ID_orig, key[0], count1->rels[0], count2);
        reporter[0] = check_report(reporter[0], count2->rels[0]->counter, ID_dest, ID_rel);
        return;
    }

    if (strcmp(ID_rel, key[last_el - 1]->ID_rel) > 0) {
        add_key(key, ID_rel, last_el, last_el);
        count2->rels[key[last_el - 1]->key] = add_rel_type(count2->rels[key[last_el - 1]->key], ID_rel);
        count1->rels[key[last_el - 1]->key] = add_rel_type(count1->rels[key[last_el - 1]->key], ID_rel);
        count2->rels[key[last_el - 1]->key] = insert_rel_i(count2->rels[key[last_el - 1]->key], ID_orig, key[last_el - 1], count1->rels[key[last_el - 1]->key], count2);
        if (glob==1) {
            reporter[key[last_el - 1]->key] = check_report(reporter[key[last_el - 1]->key], count2->rels[key[last_el - 1]->key]->counter, ID_dest, ID_rel);
            glob = 0;
        }
        return;
    }

     for (j = last_el - 1; j > -1; j--) {
        if (strcmp(ID_rel, key[j]->ID_rel) == 0) {
            if (count2->rels[key[j]->key] != NULL) {
                if (count1->rels[key[j]->key] == NULL) {
                    count1->rels[key[j]->key] = add_rel_type(count1->rels[key[j]->key], ID_rel);
                    count2->rels[key[j]->key] = insert_rel_i(count2->rels[key[j]->key], ID_orig, key[j], count1->rels[key[j]->key], count2);
                    reporter[key[j]->key] = check_report(reporter[key[j]->key], count2->rels[key[j]->key]->counter, ID_dest, ID_rel);

                    return;
                }
                else {
                    count2->rels[key[j]->key] = insert_rel_i(count2->rels[key[j]->key], ID_orig, key[j], count1->rels[key[j]->key], count2);
                    if (glob==1) {
                        reporter[key[j]->key] = check_report(reporter[key[j]->key], count2->rels[key[j]->key]->counter, ID_dest, ID_rel);
                        glob = 0;
                    }
                        return;
                }
            }
            else {
                count2->rels[key[j]->key] = add_rel_type(count2->rels[key[j]->key], ID_rel);
                if (count1->rels[key[j]->key] == NULL) {
                    count1->rels[key[j]->key] = add_rel_type(count1->rels[key[j]->key], ID_rel);
                    count2->rels[key[j]->key] = insert_rel_i(count2->rels[key[j]->key], ID_orig, key[j], count1->rels[key[j]->key], count2);
                    reporter[key[j]->key] = check_report(reporter[key[j]->key], count2->rels[key[j]->key]->counter, ID_dest, ID_rel);
                    return;
                }
                else {
                    count2->rels[key[j]->key] = insert_rel_i(count2->rels[key[j]->key], ID_orig, key[j], count1->rels[key[j]->key], count2);
                    if (glob==1) {
                        reporter[key[j]->key] = check_report(reporter[key[j]->key], count2->rels[key[j]->key]->counter, ID_dest, ID_rel);
                        glob = 0;
                    }
                    return;
                }
            }
        }
        else if (j==0) break;
        else if (strcmp(ID_rel, key[j-1]->ID_rel) > 0) break;
    }
    key[last_el] = calloc(1, sizeof(keys));
    key_swap(key, j);
    add_key(key, ID_rel, last_el, j);
    count1->rels[key[j]->key] = add_rel_type(count1->rels[key[j]->key], ID_rel);
    count2->rels[key[j]->key] = add_rel_type(count2->rels[key[j]->key], ID_rel);
    count2->rels[key[j]->key] = insert_rel_i(count2->rels[key[j]->key], ID_orig, key[j], count1->rels[key[j]->key], count2);
    reporter[key[j]->key] = check_report(reporter[key[j]->key], count2->rels[key[j]->key]->counter, ID_dest, ID_rel);
}

void add_rel(char * ID_orig, char * ID_dest, char * ID_rel, e list[A][B], k key[], rs reporter[]) {
    //printf("\nID_O: %s", ID_orig);
    //printf("\nID_D: %s", ID_dest);
    //printf("\nREL: %s", ID_rel);
    e count1, count2;         //Count1: origin list ||| count2: destination list
    int check_o = 0, check_d = 0, letter_id11 = 0, letter_id12 = 0, letter_id13 = 0, letter_id21 = 0, letter_id22 = 0, letter_id23 = 0;

    letter_id11 = search_letters(ID_orig[0], 1);
    letter_id12 = search_letters(ID_orig[1], 2);
    //letter_id13 = search_letters(ID_orig[2]);

    letter_id21 = search_letters(ID_dest[0], 1);
    letter_id22 = search_letters(ID_dest[1], 2);
    //letter_id23 = search_letters(ID_dest[2]);


    if (list[letter_id11][letter_id12] != NULL) count1 = list[letter_id11][letter_id12];
    else {
        return;
    }
    if (list[letter_id21][letter_id22] != NULL) count2 = list[letter_id21][letter_id22];
    else {
        return;
    }

    if (count1->next == NULL && count2->next == NULL) {
        if (strcmp(count1->name, ID_orig) != 0 || strcmp(count2->name, ID_dest) != 0) {
            return;
        } else {
            check_index(count2, key, ID_rel, ID_orig, ID_dest, reporter, count1);
            return;
        }
    }
    while(count1!=NULL || count2!=NULL) {        //Start searching inside the entity list
        if (strcmp(count1->name, ID_dest) == 0 && strcmp(count2->name, ID_orig) == 0) {        //If destination & origin is an autoring, then positive
            check_d = 1;
            check_o = 1;
        }
        else {
            if (strcmp(count2->name, ID_dest) == 0) {        //If destination found then positive
                check_d = 1;
            }
            if (strcmp(count1->name, ID_orig) == 0) {        //If origin found then positive
                check_o = 1;
            }
        }

        if (check_o == 1 && check_d == 1) {            //If origin & destination already found then return success
            check_index(count2, key, ID_rel, ID_orig, ID_dest, reporter, count1);
            return;
        }

        if (check_d == 1 && count1->next == NULL) break;
        if (check_o == 1 && count2->next == NULL) break;

        if ((count1->next != NULL && check_d == 1) || (count2 != NULL && check_o) || (count1->next != NULL && count2 != NULL)) {

            if (check_o != 1 && count1->next != NULL) {
                count1 = count1->next;                            //Else continue list scanning														//MIGLIORIA POSSIBILE
            }

            if (check_d != 1 && count2->next != NULL) {
                count2 = count2->next;
            }
        } else return;
    }
    if (check_o==0 || check_d==0) {									//If list = empty or origin/destination not found then return with error
        return;
    }
}

or clear_out (or list, char * ID) {
    or cur = list, prev = list;

    if (strcmp(list->d_entity->name, ID)==0) {
        list = list->next;
        free(cur);
        return list;
    }

    while (cur!=NULL) {
        if (strcmp(cur->d_entity->name, ID)==0) {
            prev->next = cur->next;
            free(cur);
            return list;
        }
        prev = cur;
        cur = cur->next;
    }
}

ir delete_relations (ir list, char * ID, r count, k key) {
    ir temp = list;
    if (strcmp(temp->o_entity, ID)==0) {
        list = list->next;
        free(temp->o_entity);
        free(temp);
        count->counter--;
        key->counter--;
        if (key->counter == 0) rep--;
        glob = 1;
        return list;
    }

    ir previo = list;

    while (temp!=NULL) {
        if (strcmp(temp->o_entity, ID)==0) {
            previo->next = temp->next;
            free(temp->o_entity);
            free(temp);
            count->counter--;
            key->counter--;
            if (key->counter == 0) rep--;
            glob = 1;
            return list;
        }
        previo = temp;
        temp = temp->next;
    }
    return list;
}

void del_rel (char * ID_orig, char * ID_dest, char * ID_rel, e list[A][B], k key[], rs reporter[]) {
    e count1, count2;
    int i, letter_id1, letter_id2, letter_id3, letter_id4;
    char letter;
    letter = ID_dest[0];
    letter_id1 = search_letters(letter, 1);
    letter = ID_dest[1];
    letter_id2 = search_letters(letter, 2);
    letter = ID_orig[0];
    letter_id3 = search_letters(letter, 1);
    letter = ID_orig[1];
    letter_id4 = search_letters(letter, 2);



    count1 = list[letter_id3][letter_id4];
    count2 = list[letter_id1][letter_id2];

    if (count2==NULL) return;
    while(count2->next!=NULL) {
        if (strcmp(count2->name, ID_dest)==0) break;
        else count2 = count2->next;
    }
    if (strcmp(count2->name, ID_dest)!=0) return;

    if (strcmp(count2->name, ID_dest)!=0) return;
    if (count1==NULL) return;
    while (count1->next!=NULL) {
        if (strcmp(count1->name, ID_orig)==0) break;
        else count1 = count1->next;
    }
    if (strcmp(count1->name, ID_orig)!=0) return;
    else {
        for (i = 0; i < E && key[i] != NULL; i++) {
            if (strcmp(key[i]->ID_rel, ID_rel) == 0) break;
        }
        if (key[i]==NULL) return;
        if (strcmp(key[i]->ID_rel, ID_rel) != 0) return;
        if (count1->rels[key[i]->key]==NULL) return;
        if (count2->rels[key[i]->key]==NULL) return;
        else {
            or tmp = count1->rels[key[i]->key]->o_rel;
            if (tmp!=NULL) {
                while (tmp->next != NULL) {
                    if (strcmp(tmp->d_entity->name, ID_dest) == 0) {
                        tmp->d_entity->rels[key[i]->key]->i_rel = delete_relations(
                                tmp->d_entity->rels[key[i]->key]->i_rel, ID_orig, tmp->d_entity->rels[key[i]->key],
                                key[i]);
                        if (glob == 1) {
                            count1->rels[key[i]->key]->o_rel = clear_out(count1->rels[key[i]->key]->o_rel, ID_dest);
                            glob = 0;
                        }
                        reporter[key[i]->key] = check_report_del(reporter[key[i]->key], ID_dest, list, key[i]->key);
                        return;
                    }
                    tmp = tmp->next;
                }
                if (strcmp(tmp->d_entity->name, ID_dest) == 0) {
                    tmp->d_entity->rels[key[i]->key]->i_rel = delete_relations(tmp->d_entity->rels[key[i]->key]->i_rel,
                                                                               ID_orig,
                                                                               tmp->d_entity->rels[key[i]->key],
                                                                               key[i]);
                    if (glob == 1) {
                        count1->rels[key[i]->key]->o_rel = clear_out(count1->rels[key[i]->key]->o_rel, ID_dest);
                        glob = 0;
                    }
                    reporter[key[i]->key] = check_report_del(reporter[key[i]->key], ID_dest, list, key[i]->key);
                    return;
                }

                //count1->rels[key[i]->key]->o_rel = del_outbound(count1->rels[key[i]->key]->o_rel);
                //count2->rels[key[i]->key]->i_rel = delete_relations(count2->rels[key[i]->key]->i_rel, count2->rels[key[i]->key], ID_orig, key[i]);
            }
        }
    }
}

void Print(ir list) {
    ir count = list;
    while (count->next!=NULL) {
        if (count->o_entity!=NULL) {
            fputs(" \"", stdout);
            fputs(count->o_entity, stdout);
            fputs("\"", stdout);
        }
        count = count->next;
    }
    if (count->o_entity!=NULL) {
        fputs(" \"", stdout);
        fputs(count->o_entity, stdout);
        fputs("\"", stdout);
    }
}

void report (rs structure[], k key[]) {
    if (rep==0) {
        fputs("none\n", stdout);
        return;
    }
    e count;
    int f=0;
    for (int i = 0; i < E; i++) {
        if (key[i] != NULL) {
            if (structure[key[i]->key] !=NULL && structure[key[i]->key]->counter!=0) {
                if (f != 0) fputs(" ", stdout);
                fputs("\"", stdout);
                fputs(structure[key[i]->key]->name, stdout);
                fputs("\"", stdout);
                Print(structure[key[i]->key]->rels);
                fputs(" ", stdout);
                f++;
                printf("%d;", structure[key[i]->key]->counter);
            }
        }
    }
    fputs("\n", stdout);
}

void Print2(ir list) {
    ir count = list;
    while (count!=NULL) {
        fputs(" ", stdout);
        fputs(count->o_entity, stdout);
        fputs("; ", stdout);
        count = count->next;
    }
}

void relationss(e list[A][B]) {
    for (int i = 0; i < A; i++) {
        for (int k =0; k<B; k++) {
            e count = list[i][k];
            while (count != NULL) {
                fputs("\n", stdout);
                fputs(count->name, stdout);
                for (int j = 0; j < E; j++) {
                    if (count->rels[j] != NULL) {
                        fputs("   REL:", stdout);
                        fputs(count->rels[j]->name, stdout);
                        printf(" %d", count->rels[j]->counter);
                        printf("  FROM: ");
                        Print2(count->rels[j]->i_rel);
                    }
                }
                count = count->next;
            }
        }
    }
    printf("\n\n --- --- END OF DEBUGGING REPORT --- ---\n");
}

int main() {
    char * token;
    char * id=NULL, * id_d=NULL, * relation=NULL;
    char letter=0;
    int letter_id1=0, letter_id2=0, letter_id3=0;
    rs reporter[E];
    //reporter = calloc(E, sizeof(report_structure));
    e list[A][B];
    k rel_key[E];
    for (int x=0; x<A; x++) {
        for (int y=0; y<B; y++) {
            list[x][y] = NULL;
        }
    }

    for (int i=0; i<E; i++) {
        rel_key[i] = NULL;
        reporter[i] = NULL;
    }


    while (1) {		//execute "menu" till user types "end" (case sensitive)
        //printf("\n\n\"Please insert command, end to quit: \"\n");
        if (scanf("%ms", &token)==1) {		//Scan the command
            //token = strtok(input," ");		//Extract the first no-space-input NOT IN USE
            //printf("Token: %s", token);
            if (strcmp("addent", token) == 0) {		//If input = addent, open the add_ent function
                if (scanf(" \"%m[^\"]\"s", &id)==1) {					//Get the entity's ID
                    //printf("\nID :%s", id);
                    letter = id[0];
                    letter_id1 = search_letters(letter, 1);
                    letter = id[1];
                    letter_id2 = search_letters(letter, 2);
                    /*letter = id[2];
                    letter_id3 = search_letters(letter);*/

                    list[letter_id1][letter_id2] = add_ent(id, list[letter_id1][letter_id2]);
                    free(id);
                }			//else continue execution in main
            }

            else if (strcmp("delent", token) == 0) {		//If input = delent, open the del_ent function
                if (scanf(" \"%m[^\"]\"s", &id)==1) {					//Get the entity's ID
                    letter = id[0];
                    letter_id1 = search_letters(letter, 1);
                    letter = id[1];
                    letter_id2 = search_letters(letter, 2);

                    //del_ent(id, list, rel_key);
                    list[letter_id1][letter_id2] = del_single_ent(id, list[letter_id1][letter_id2], rel_key, reporter, list);
                    free(id);
                }
            }       //else continue execution in main

            else if (strcmp("addrel", token)==0) {
                if (scanf(" \"%m[^\"]\"s", &id)==1 && scanf(" \"%m[^\"]\"s", &id_d)==1 && scanf(" \"%m[^\"]\"s", &relation)==1) {
                    add_rel(id, id_d, relation, list, rel_key, reporter);
                    free(id);
                    free(id_d);
                    free(relation);
                }
            }

            else if (strcmp("delrel", token)==0) {
                if (scanf(" \"%m[^\"]\"", &id)==1 && scanf(" \"%m[^\"]\"", &id_d)==1 && scanf(" \"%m[^\"]\"", &relation)==1) {
                    del_rel(id, id_d, relation, list, rel_key, reporter);
                    free(id);
                    free(id_d);
                    free(relation);
                }
            }

            else if (strcmp("report", token) == 0) {	//If input = report, open the report function
                //printf("\nReporting...");
                report(reporter, rel_key);
            }

            else if (strcmp("end", token) == 0) {		//If input = end, close the program
                //printf("\nQuitting...");
                return 0;
            }			//else continue execution in main
            
            else if (strcmp("relations", token)==0) {
                printf("\n --- --- DEBUGGING DEVELOP REPORT --- ---");
                printf("\n TASK 1: last element value: %d", last_el);
                printf("\n TASK 2: rep bit value; %d", rep);
                printf("\n TASK 3: Rel key values: ");
                for (int i=0; i<E; i++) {
                    if (rel_key[i]!=NULL) printf(" %s, %d", rel_key[i]->ID_rel, rel_key[i]->counter);
                }
                printf("\nTASK 4: Entity/Relations List:\n");
                relationss(list);
            }
            //printf("\n\"END\" not found, continuing execution");
        }
        free(token);
    }
}
