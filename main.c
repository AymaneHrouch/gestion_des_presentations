#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include <conio.h>

int main() {
    system("color 5F");
    int i, x, y, choix, k;
    x = nbre_etudiant();
    y = nbre_sujets();
    Etudiant* etudiants;
    Pres* sujets;
    etudiants = malloc(sizeof(Etudiant) * x);
    sujets = malloc(sizeof(Pres) * y);
    charger_donnees(etudiants, sujets);
    if(login() == 1){ // login() est egal a 1 si le mot de passe est correct
        printf("\n\nMot de passe correct ");
        for(i = 0; i < 5; i++) {
            fordelay(100000000);
            printf(".");
        }
        menu:
        system("cls");
        printf("\n\n\t\t\t\tPRESENTATIONS MANAGER");
        printf("\n\n\n\t\t\t\tBIENVENUE AU MENU PRINCIPALE");
        printf("\n\n\nGestion des etudiants :\t\tnombre total : %d \n\n1 : Ajouter un etudiant\n2 : Ajouter plusieurs etudiants a la fois\n3 : Modifier etudiant nom\n4 : Supprimer etudiant\n5 : Chercher un etudiant\n6 : Afficher la liste des etudiants par ordre alphabetic", nbre_etudiant());
        printf("\n\n\nGestion des presentations : \t\tnombre total : %d\n\n7 : Ajouter une presentation\n8 : Ajouter plusieurs presentations a la fois\n9 : Modifier le titre d\'une presentation\n10 : Supprimer une presentation\n11 : Chercher un sujet\n12 : Afficher les presentations par ordre alphabetic", nbre_sujets());
        printf("\n\n\nGestion des devoirs : \n\n13 : Faire assigner\n14 : Une presentation a ete faite\n15 : Abandonner\n16 : Changer le presenteur\n\n0. Quitter\nvotre choix : ");
        scanf("%d", &choix);
        system("cls");
        switch(choix) {
            case 0:
                printf("Merci pour utiliser notre application\n\n\t\tFrapper une touche pour quitter...");
                getch();
                exit(0);
            break;
            case 1:
                x++; // on va ajouter un etudiant alors il faut qu'on reserve son place
                etudiants = realloc(etudiants, sizeof(Etudiant) * x);
                ajouter_etudiant(etudiants);
                retour();
                goto menu;
            break;
            case 2:
                k = nbre_etudiant_ajouter();
                x += k;
                etudiants = realloc(etudiants, sizeof(Etudiant) * x);
                for(i = 0; i < k; i++) ajouter_etudiant(etudiants);
                retour();
                goto menu;
            break;
            case 3:
                modifier_etudiant(etudiants);
                retour();
                goto menu;
            break;
            case 4:;
                supp_etudiant(etudiants);
                x--; // on a supprimé un etudiant alors on n'a plus besoin d'une place
                etudiants = realloc(etudiants, sizeof(Etudiant) * x);
                retour();
                goto menu;
            break;
            case 5:
                chercher_etudiant(etudiants, sujets);
                retour();
                goto menu;
            case 6:
                afficher_etudiants(etudiants, sujets);
                retour();
                goto menu;
            break;
            case 7:
                y++; // on va ajouter un sujet, il faut qu'on reserve son place
                sujets = realloc(sujets, sizeof(Pres) * y);
                ajouter_sujet(sujets);
                retour();
                goto menu;
            break;
            case 8:
                k = nbre_sujets_ajouter();
                y += k;
                sujets = realloc(sujets, sizeof(Pres) * y);
                for(i = 0; i < k; i++) ajouter_sujet(sujets);
                retour();
                goto menu;
            break;
            case 9:
                modifier_sujet(sujets);
                retour();
                goto menu;
            break;
            case 10:
                supp_sujet(etudiants, sujets);
                y--; // on a supprimé un sujet alors on n'a pas encore besoin d'une place
                sujets = realloc(sujets, sizeof(Pres) * y);
                retour();
                goto menu;
            break;
            case 11:
                chercher_sujet(etudiants, sujets);
                retour();
                goto menu;
            case 12:
                afficher_sujets(etudiants, sujets);
                retour();
                goto menu;
            case 13:
                assigner(etudiants, sujets);
                retour();
                goto menu;
            break;
            case 14:
                fait(etudiants, sujets);
                retour();
                goto menu;
            break;
            case 15:
                abandonner(etudiants, sujets);
                retour();
                goto menu;
            break;
            case 16:
                changer_presenteur(etudiants, sujets);
                retour();
                goto menu;
            break;
            default:
                printf("Choix invalide! frapper une touche pour ressayer\n");
                getch();
                goto menu;
        }
    }

    // on a terminé notre programme alors il faut qu'on libére la memoire
    free(etudiants);
    free(sujets);
    return 0;
}
