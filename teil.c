#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "teil.h"
#include "utility.h"

// spast
teil* newTeil(char typ[], char bez[], char einheit[], char gewicht[], char preis[]) {
    teil *temp = (teil*)malloc(sizeof(teil));
    if(typ != NULL)
	    strcpy(temp->typ, typ);
    if(bez != NULL)
	    strcpy(temp->bez, bez);
    if(einheit != NULL)
	    strcpy(temp->einheit, einheit);
    if(gewicht != NULL)
	    strcpy(temp->gewicht, gewicht);
    if(preis != NULL)
	    strcpy(temp->preis, preis);
	    temp->needs = NULL;
	    temp->next = NULL;
    return temp;
}

void addTeilToList(teil **HEAD, teil **TAIL, teil *p) {
    if(*HEAD == NULL) {
        *HEAD = p;
        *TAIL = p;
    } else {
        (*TAIL)->next = p;
        *TAIL = p;
    }
}

void transferTeil(teil **HEAD_FROM, teil **HEAD_TO, teil **TAIL_TO, teil *p, teil *p_prev) {

    //OK
    if (*HEAD_TO == NULL) {
        *HEAD_TO = p;
        *TAIL_TO = p;
    } else {
        (*TAIL_TO)->next = p;
        *TAIL_TO = p;
    }

    if (p_prev == NULL) {
        *HEAD_FROM = p->next;
    } else {
        p_prev->next = p->next;
    }
    p->next = NULL;
}

short canTeilBuild (teil *HEAD_sorted, teil *p) {
	teil *n = p->needs;
    short check = 1; //bool check = true;
    while(n != NULL) {
        teil *p2 = HEAD_sorted;
        while (p2 != NULL) {
            if((strcmp(n->typ, p2->typ) == 0) && (strcmp(n->bez, p2->bez)== 0)) {
                check = 1;
                break;
            }else{
                check = 0;
            }
            p2 = p2->next;
        }
        if(check == 0)
            break;
        n = n->needs;
    }
    return check;
}

void readTeils(teil** HEAD, char path[]) {
    FILE *teilFile = fopen(path, "r");
    existFile(teilFile);

    char teil_typ[30], teil_bez[30], teil_einheit[10], teil_gewicht[30], teil_preis[30];  //teil.dat

	teil *TAIL = NULL;

    while( fscanf(teilFile,"%s %s %s %s %s", teil_typ, teil_bez, teil_einheit, teil_gewicht, teil_preis) != EOF )
    {
        teil *temp = newTeil(teil_typ, teil_bez, teil_einheit, teil_gewicht, teil_preis);
        addTeilToList (HEAD, &TAIL, temp);
	}
    fclose(teilFile);
}


void readSchritts(teil *HEAD, char path[]) {

    FILE *schrittFile = fopen(path, "r");
    existFile(schrittFile);

	teil *TAIL_temp = NULL;
    char ziel_typ[30], ziel_bez[30], quel_typ[30], quel_bez[30]; //schritt.dat
    int nr = 0;

    while( fscanf(schrittFile,"%s %s %d %s %s %*s %*s %*s", ziel_typ, ziel_bez, &nr, quel_typ, quel_bez) != EOF )
    {
        teil *temp = newTeil(quel_typ, quel_bez, NULL, NULL, NULL);

        if(nr == 1) {
            teil *p = HEAD;
            while (p != NULL) {
                if((strcmp(p->typ, ziel_typ) == 0) && (strcmp(p->bez, ziel_bez)== 0)) {
                    p->needs = temp;
                    TAIL_temp = p->needs;
                    break;
                }
                p = p->next;
            }
        } else {
            TAIL_temp->needs = temp;
            TAIL_temp = temp;
        }
    }

    fclose(schrittFile);
}

void sortingTeils(teil *HEAD, teil **HEAD_sorted) {

    teil *TAIL_sorted = NULL;

    while (HEAD != NULL) {
        teil *p = HEAD;
        teil *p_prev = NULL;
        while (p != NULL) {
            if(canTeilBuild(*HEAD_sorted, p) == 1) { //ok
            	transferTeil(&HEAD, HEAD_sorted, &TAIL_sorted, p, p_prev);
            }
            p_prev = p;
            p = p->next;
        }
    }
    freeSpace(HEAD);
}

void printTeils(teil *HEAD, char path[]) {
    FILE * fOutput = fopen(path, "w");
    teil *p = HEAD;
    while (p != NULL) {
        fprintf(fOutput,"%s %s %s %s %s\n", p->typ, p->bez, p->einheit, p->gewicht, p->preis);
        p = p->next;
    }
    fclose(fOutput);
    freeSpace(HEAD);
}
