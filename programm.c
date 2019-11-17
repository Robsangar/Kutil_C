#include <stdio.h>
#include <stdlib.h> // For exit() function
#include <string.h>

typedef struct teil {
    char typ[30], bez[30], einheit[10];
    double gewicht, preis;
    struct teil * next;
    struct teil * needs;
    
}teil;

int main()
{
    FILE *teilFile, *schrittFile;
    if ((teilFile = fopen("teil.dat", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    if ((schrittFile = fopen("schritt.dat", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    char teil_typ[30], teil_bez[30], teil_einheit[10]; //teil.dat
    double teil_gewicht = 0, teil_preis = 0;

    teil *HEAD = NULL;
    teil *TAIL = NULL;
    int size = 0;

    while( fscanf(teilFile,"%s %s %s %lf %lf", &teil_typ, &teil_bez, &teil_einheit, &teil_gewicht, &teil_preis) != EOF )
    {
        size++;
        teil *temp = (teil*)malloc(sizeof(teil));
        strcpy(temp->typ, teil_typ);
        strcpy(temp->bez, teil_bez);
        strcpy(temp->einheit, teil_einheit);
        temp->gewicht = teil_gewicht;
        temp->preis = teil_preis;
        temp->needs = NULL;
        temp->next = NULL;

        if(HEAD == NULL) {
            HEAD = temp;
            TAIL = temp;
        } else {
            TAIL->next = temp;
            TAIL = temp;
        }

    }
    fclose(teilFile);

    char ziel_typ[30], ziel_bez[30], quel_typ[30], quel_bez[30]; //schritt.dat
    int nr = 0;

    teil *TAIL_temp = NULL;

    while( fscanf(schrittFile,"%s %s %d %s %s %*lf %*s %*lf", &ziel_typ, &ziel_bez, &nr, &quel_typ, &quel_bez) != EOF )
    {
        teil *temp = (teil*)malloc(sizeof(teil));
        strcpy(temp->typ, quel_typ);
        strcpy(temp->bez, quel_bez);
        temp->next = NULL;
        temp->needs = NULL;

        if(nr == 1) {
            teil *p = HEAD;
            while (p != NULL) {
                if((strcmp(p->typ, ziel_typ) == 0) && (strcmp(p->bez, ziel_bez)== 0)) {
                    //printf("if   %s %s \n", p->typ, p->bez);
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


    teil *HEAD_sorted = NULL;
    teil *TAIL_sorted = NULL;


    while (HEAD != NULL) { // Liste boÅŸalana kadar
        teil *p = HEAD;
        teil *p_prev = NULL;
        while (p != NULL) { // TÃ¼m karÄ±ÅŸÄ±k liste iÃ§in
            teil *n = p->needs;
            short bool = 1;
            while(n != NULL) { // TÃ¼m ihtiyaÃ§lar iÃ§in
                teil *p2 = HEAD_sorted;
                while (p2 != NULL) { // TÃ¼m sÄ±ralanmÄ±ÅŸ liste iÃ§in
                    if((strcmp(n->typ, p2->typ) == 0) && (strcmp(n->bez, p2->bez)== 0)) {
                        bool = 1;
                        break;
                    }else{
                        bool = 0;
                    }
                    p2 = p2->next;
                }
                if(bool == 0)
                    break;
                n = n->needs;
            }
            if(bool == 1) {
                //OK
                if (HEAD_sorted == NULL) {
                    HEAD_sorted = p;
                    TAIL_sorted = p;
                } else {
                    TAIL_sorted->next = p;
                    TAIL_sorted = p;
                }

                if (p_prev == NULL) {
                    HEAD = p->next;
                } else {
                    p_prev->next = p->next;
                }
                p->next = NULL;
            }
            p_prev = p;
            p = p->next;
            teil *d = HEAD_sorted;
            while (d != NULL) {
                d = d->next;
            }
            teil *du = HEAD;
            while (du != NULL) {
                du = du->next;
            }
        }
    }

    FILE * fOutput = fopen("geordnet.dat", "w");
    teil *p = HEAD_sorted;
    while (p != NULL) {
        fprintf(fOutput,"%s %s %s %lf %lf\n", p->typ, p->bez, p->einheit, p->gewicht, p->preis);
        p = p->next;
    }
    fclose(fOutput);



    return 0;
}
