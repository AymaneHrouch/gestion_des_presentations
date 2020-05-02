#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "main.h"
#include "fonctions.h"

//////////// ******** DES FONCTIONS GENERALES ******** ////////////


/*
La premiere fonction qui charge les donnees des deux fichiers dans deux tableau dynamique
*/
void charger_donnees(Etudiant* e, Pres* p) {
    int i;
    FILE* users = NULL;
    users = fopen("users.txt", "r");
    /*
    si le fichier n'existe pas, fopen() retourne la valeur NULL
    */
    if(users == NULL) {
        printf("\n\n\nLe fichier \"users.txt\" est introuvable\n\n\tFRAPPER UNE TOUCHE POUR FERMER L\'APPLICATION");
        getch();
        exit(0);
    }
    for(i = 0; i < nbre_etudiant(); i++) {
        fscanf(users, "%d %s %s %d %d", &(e+i)->id, &(e+i)->nom, &(e+i)->prenom, &(e+i)->disp, &(e+i)->presentation);
    }
    fclose(users);

    FILE* sujets = NULL;
    sujets = fopen("sujets.txt", "r");

    if(sujets == NULL) {
        printf("\n\n\n\tLe fichier \"sujets.txt\" est introuvable\n\n\tFRAPPER UNE TOUCHE POUR FERMER L\'APPLICATION");
        getch();
        exit(0);
    }
    for(i = 0; i < nbre_sujets(); i++) {
        fscanf(sujets, "%d %s", &(p+i)->id, &(p+i)->titre);
    }
    fclose(sujets);
}

/*
fonction pour demander le mot de passe avant utiliser le programme
*/
int login(){
    int main_quitter; // cette variable doit etre sois 0 (pour quitter) sois 1 (pour ressayer)
    char pass[5], mdp[5] = "ensa";
    printf("\n\n\tEntrez le mot de passe pour utiliser ce programme : ");
    scanf("%s", &pass);
    if(strcmp(pass, mdp) == 0 ) {
        return 1;
    } else {
        ressayer:
        system("color 4F");
        system("cls");
        printf("\n\n\tMot de passe incorrect!\a");
        printf("\n\t1: Ressayer\n\t0: Quitter\n\tVotre choix : ");
        scanf("%d", &main_quitter);
        if(main_quitter == 1) {
            system("cls");
            main();
        }
        else if(main_quitter == 0) {
            exit(0);
        }
        else {
            printf("\tChoix invalide!");
            goto ressayer;
        }
    }
}

/*
on utilise la fonction afficher_sujets_disp() ou la fonction afficher_pres_afaire() pour afficher une liste
des choix des sujets, mais l'utilisateur peux faire une faute est taper un choix qui n'est pas sur la liste
ça peut faire des problémes pcq par exemple si l'utilisateur fait une faute en ressayons de supprimer un sujet,
peut etre il va supprimer un sujet qui est n'est pas diponible, ça va gacher notre programme

cette fonction va verifier le choix de l'utilisateur, c'est un boolean, si le choix est disponible (verifié) elle va retourner 1
*/

int verifier_choix(int* choices, int choix, int n) {
    int i, cndt = 0;
    for(i = 0; i < n; i++) {
        if(*(choices + i + 1) == choix) cndt++; // cette condition doit verifie une seul fois
    }
    free(choices); // on a plus besoin de ce tableau
    if(cndt == 1) return 1;
    else return 0;
}

/*
Une fonction pour ralentir la boucle for
*/
void fordelay(int j)
{   int i,k;
    for(i=0;i<j;i++)
         k=i;
}

void retour() {
    printf("\n\n\tFrapper une touche pour retourner aux MENU");
    getch();
}


////////// ****  DES FONCTIONS POUR MANIPULER LES ETUDIANTS **** //////////

/*
cette fonction est très importante
elle nous permet de calculer combien d'etudiant est sur notre liste
note bien que le nombre d'etudiant peut etre different de l'identifiant (id) du dernier etudiant
*/
int nbre_etudiant(){
    Etudiant e;
    int n = 0;
    FILE* users = NULL;
    users = fopen("users.txt", "r");
    /*
    en cas de l'echec de la fonction "fscanf" il retourne -1
    */
    while(fscanf(users, "%d %s %s %d %d", &e.id, &e.nom, &e.prenom, &e.disp, &e.presentation)!=-1) {
        n++;
    /*
    chaque fois cette boucle terminer notre position va etre au dernier du ligne
    et donc on peut tester la ligne suivante quand la boucle redemarrer
    */
    }
    fclose(users);
    return n;
}

/*
cette fonction va nous aider sur les autres fonctions pour
demander l'utilisateur a determiner quel etudiant il veut manipuler

elle permet l'utilisateur de chercher l'etudiant par son nom
elle peut verifier l'existence de l'etudiant et egalement afficher tout les etudiant
qui ont le meme nom
*/
Etudiant* selectionner_etudiant(Etudiant* e) {
    int i, choix, n = nbre_etudiant(), possibilite, err = 0, menu_res ;
    char nom[CMAX];
    scanf("%s", &nom);
    err_handl:
    possibilite = 0;
    printf("\n");
    for (i = 0; i < n; i++) {
        if(strcmp(nom, (e+i)->nom)==0) {
            possibilite++;
            printf("\t%d. %s %s\n", possibilite, (e+i)->nom, (e+i)->prenom); // afficher tous les resultats (les etudiant qui ont ce nom)
        }
    }
    if(possibilite) { // possibilite != 0 donc il y a des resultats
        possibilite = 0;
        printf("\n\tVeuillez verifier et confirmer votre choix : ");
        scanf("%d", &choix);
    } else { // possibilite reste egal a 0 alors il y a aucun resultat
        printf("\tAucun resultat, ressayez : ");
        selectionner_etudiant(e);
        return;
    }
    for(i = 0; i < n; i++) {
        if(strcmp(nom, (e+i)->nom)==0) possibilite++; // on va refaire la meme boucle, pour qu'on sait l'etudiant choisi par l'utilisateur
        if(possibilite == choix) return (e+i); // si on trouve une possibilite egal a notre choix on va retourner l'etudiant selectionné
        else err = 1; // sinon, ça veut dire que l'utilisateur a entré un choix invalide, pour ça on va donne a notre boolean (err) la valeur 1
    }
    err_handl_2:
    if(err) { // si err = 1 alors choix est invalide, l'utilisateur a deux choix, ressayer ou quitter
        printf("\n\n\tChoix invalide!\n\n0: Exit\n1: Ressayer\nvotre choix : ");
        scanf("%d", &menu_res);
    }
    if(menu_res == 1) { // si l'utilisateur a choisi un choix invalide on va afficher une autre fois les choix et demander un choix valide
            system("cls");
            goto err_handl;
    }
    else if(menu_res == 0) exit(0); // s'il choisi 0 alors il veut quitter le programme
    else goto err_handl_2; // sinon on va repeter la meme question pour lui :)
}


/*
on a besoin de cette fonction quand on veut ajouter ou modifier un etudiant
elle nous permet de verifier si l'etudiant est deja sur notre liste ou non
si oui elle vas retourner 0, sinon elle va retourner 1
la variable ajouter_ modifier a deux cas, 0 ou 1, 0 pour quand veut ajouter
*/
int verifier_etudiant(Etudiant* e, Etudiant* etudiant_a_verifier, int ajouter_modifier) {
    int n = nbre_etudiant(), i, cndt = 0;
    for(i = 0; i < n; i++) {
        if((strcmp(etudiant_a_verifier->nom, (e+i)->nom) == 0) && (strcmp(etudiant_a_verifier->prenom, (e+i)->prenom) == 0)) cndt++;
        if(cndt > 1 && ajouter_modifier == 1) return 0; // si on modifie un nom, alors la fonctions va trouver 1 nom et donc cndt = 1, cad il y a un autre nom ssi cndt > 1
        if(cndt > 0 && ajouter_modifier == 0) return 0; // si on veut ajouter un etudiant il faut que cndt reste egal a 0, si cndt > 0 alors cet etudiant est deja sur la liste
    }
    return 1;
}

/*
la fonction suivante nous permet d'ajouter un seul etudiant
*/
void ajouter_etudiant(Etudiant* e) {
    int n = nbre_etudiant();
    int m = (e + n - 1)->id; // C'est le ID du dernier etudiant sur la liste
    int cndt;
    if (n==0) e->id = 1; // si le fichier est vide, le premier etudiant doit avoir un id = 1
    else (e+n)->id = m + 1; // sinon le id du dernier etudiant sur la liste + 1
    (e+n)->disp = 1; // noveau etudiant est disponible
    (e+n)->presentation = 0; // nouveau etudiant n'as pas un assignement
    printf("\n\tNom : ");
    scanf("%s", &(e+n)->nom);
    printf("\tPrenom : ");
    scanf("%s", &(e+n)->prenom);
    cndt = verifier_etudiant(e, (e+n), 0); // on verifie si l'etudiant est deja sur la liste, 3eme parametre est 0 parce que on a en train d'ajouter un etudiant
    if (cndt) {
        sauvegarder_etudiant(e, 1);
        printf("\tEtudiant a ete sauvegarder avec succes");
    } else printf("\tCet etudiant est deja sur la liste");
}

/*
simple fonction pour demander le nombre des etudiants à ajouter
*/
int nbre_etudiant_ajouter() {
    int n;
    printf("Combien d\'etudiants voulez vous ajouter? ");
    scanf("%d", &n);
    return n;
}

/*
la fonction suivante nous permet de
modifier le nom et le prenom d'un etudiant
*/
void modifier_etudiant(Etudiant* e) {
    int cndt;
    char old_nom[CMAX], old_prenom[CMAX];
    Etudiant* etudiant;
    printf("\tSaisir le nom d etudiant : ");
    etudiant = selectionner_etudiant(e);
    strcpy(old_nom, etudiant->nom); // storer le nom et prenom anciens, si l'etudiant est deja sur la liste on va avoir besoin de retourner ceux à l'etudiant
    strcpy(old_prenom, etudiant->prenom);
    printf("\tTu vas changer le nom de : %s %s", etudiant->nom, etudiant->prenom);
    printf("\n\n\tNouveau nom : ");
    scanf("%s", &etudiant->nom);
    printf("\tNouveau prenom : ");
    scanf("%s", &etudiant->prenom);
    cndt = verifier_etudiant(e, etudiant, 1);
    if(cndt) {
        sauvegarder_etudiant(e, 2);
        printf("\tSauvegarde avec succes");
    } else {
        strcpy(etudiant->nom, old_nom);
        strcpy(etudiant->prenom, old_prenom);
        printf("\t%s %s est deja sur la liste", etudiant->nom, etudiant->prenom);
    }
}

/*
La fonction suivante nous permet de supprimer un etudiant de notre liste
a condition que cet etudiant n'a pas fait une presentation et qu'il n'a pas une presentation a faire
*/
void supp_etudiant(Etudiant* e) {
    int n = nbre_etudiant(), i, cndt = 0, j = 0;
    Etudiant* etudiant;
    printf("\tSaisir le nom d etudiant : ");
    etudiant = selectionner_etudiant(e);
    if(etudiant->disp == 0) printf("\tCette personne a deja une presentation a faire\n");
    else if(etudiant->disp == -1) printf("\tCette personne a deja fait une presentation");
    else {
        for(i = 0; i < n; i++) {
            if(etudiant->id == (e+i)->id) {
                j = i;
                break;
            }
        }
        for(i = j; i < n; i++) {
            *(e+i) = *(e+i+1);
        }
        printf("Vous avez supprime l\'etudiant avec succes");
        sauvegarder_etudiant(e, 0);
    }
}

/*
fonction suivante est pour sauvegarder les donnees apres les manipuler en utilisant
nos fonctions
supprimer_ou_ajouter_ou_miseajour va etre 0 ou 1 ou 2, 0 pour supprimer 1 pour ajouter 2 pour misajour
l'utilité de cette variable est qu'on connait si on va ecrire les lignes + 1 ou
les ligne - 1 ou on va ecrire le meme nombre des lignes
*/
void sauvegarder_etudiant(Etudiant* e, int supprimer_ou_ajouter_ou_miseajour) {
    int i, x = nbre_etudiant();;
    FILE* users;
    users = fopen("users.txt","w");
    if(supprimer_ou_ajouter_ou_miseajour == 1) x++;
    else if (supprimer_ou_ajouter_ou_miseajour == 0) x--;
    for(i = 0; i < x; i++) { // ecrire les nouvelles donnes
        fprintf(users, "%d %s %s %d %d\n", (e+i)->id, (e+i)->nom, (e+i)->prenom, (e+i)->disp, (e+i)->presentation);
    }
    fclose(users);
}

/*
Chercher un etudiants
*/
void chercher_etudiant(Etudiant* e, Pres* p) {
    Etudiant* etudiant;
    printf("\tSaisir le nom d\'etudiant que vous voulez chercher : ");
    etudiant = selectionner_etudiant(e);
    printf("\n ID\tnom et prenom\t\tPresentation\t\tFaite?");
    /*
    si l'etudiant n'a pas une presentation a faire en ecrit "pas encore" sur la case du presentation
    si l'etudiant a fait la presentation on ecrit une "X" sur la case "Faite?" sinon on ecrit une "_"
    */
    printf("\n\n %d\t%s %s\t\t%s\t\t%c", etudiant->id, etudiant->nom, etudiant->prenom, fetch_sujet(etudiant, p), (etudiant->disp == -1) ? 'X' : '_');
}

/*
Cette fonction retourner un etudiant qui va faire la presentation avec id égal à x
*/
Etudiant* chercher_e(Etudiant* e, int x) {
    int i, n = nbre_etudiant();
    for(i = 0; i < n; i++) {
        if (x == (e+i)->presentation) return (e+i);
    }
}

/*
Fonction indispensable pour la fonction qsort()
*/
int comparateur_e(const void* p, const void* q)
{
    return strcmp(((Etudiant*) p)->nom, ((Etudiant*) q)->nom);
}

void trier_e(Etudiant* e) {
    int n;
    n = nbre_etudiant();
    qsort(e, n, sizeof(Etudiant), comparateur_e); // fonction populaire du triage, qui utilise l'algorithme "quick sort" pour trier les donnes
}

int comparateur_s(const void* p, const void* q)
{
    return strcmp(((Pres*) p)->titre, ((Pres*) q)->titre);
}

void trier_s(Pres* p) {
    int n;
    n = nbre_sujets();
    qsort(p, n, sizeof(Pres), comparateur_s);
}

/*
Une fonction pour afficher les etudiants en ordre alphabetic
*/
void afficher_etudiants(Etudiant* e, Pres* p) {
    int i, n = nbre_etudiant();
    trier_e(e); // trier le tableau des etudiants
    printf("\nId\t\tNom\t\t\t\tPrenom\t\t\tPresentation\t\t\tFaite?\n");
    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    for (i = 0; i < n; i++) {
        printf("%d\t\t%9s\t\t\t%10s\t\t\t%10s\t\t\t%c\n", (e+i)->id, (e+i)->nom, (e+i)->prenom, fetch_sujet(e+i, p), (((e+i)->disp)==-1) ? 'X' : '_');
    }
    charger_donnees(e, p); /* tres important pcq les autres fonctions ont besoin
     d'un tableau qui contient les donnes des fichiers qui ont trié par l'ordre
     du ID
     */
}

/*la fonction suivante nous permet de trouver l'index i d'un etudiant
en utilisant l'ID de sa presentation,
par exemple si (e+99) va presenter le sujet de id = 5 qui est (p+4),
il suffit qu'on appele la fonction fetch_etudiant(e, p+4) et elle va retourner 99

Cette fonctions va nous aider quand on affiche les sujets
*/
int fetch_etudiant(Etudiant* e, Pres* p) {
    int i, n = nbre_etudiant();
    for(i = 0; i < n; i++) {
        if((e+i)->presentation == p->id) return i;
    }
    return -1;
}



////////// ****  DES FONCTIONS POUR MANIPULER LES SUJETS **** //////////


/*
Meme principe qu'on a utilisé pour calculer le nombre des etudiants
*/
int nbre_sujets(){
    Pres p;
    int n = 0;
    FILE* sujets = NULL;
    sujets = fopen("sujets.txt", "r");
    while(fscanf(sujets, "%d %s", &p.id, &p.titre)!=-1) {
        n++;
    }
    fclose(sujets);
    return n;
}

/*
Cette fonction nous permet d'ajouter un seul sujet a notre liste
*/
void ajouter_sujet(Pres* p) {
    int n = nbre_sujets();
    int m = (p + n - 1)->id; // m est le ID du dernier sujet
    int cndt, i;
    if (n == 0) (p+n)->id = 1;
    else (p+n)->id =  m + 1;
    printf("\n\tTitre (pas d\'espaces, utilise \"_\"): ");
    scanf("%s", &(p+n)->titre);
    for(i = 0; i < n; i++) {
        if(strcmp(&(p+n)->titre, (p+i)->titre) == 0) cndt = 0;
    }
    if (cndt) {
        sauvegarder_sujet(p, 1);
        printf("\tPresentation a ete sauvegrader avec succes");
    }
    else printf("\tCette presentation est deja sur la liste");
}

/*
simple fonction pour demander le nombre des etudiants à ajouter
*/
int nbre_sujets_ajouter() {
    int n;
    printf("Combien de sujet voulez vous ajouter? ");
    scanf("%d", &n);
    return n;
}

/*
Cette fonction nous aider a modifier le titre d'un sujet sur notre liste
*/
void modifier_sujet(Pres* p) {
    int n = nbre_sujets(), i, choix;
    char titre[CMAX];
    ressayer:
    system("cls");
    printf("\tSaisir le titre de la presentation : ");
    scanf("%s", titre);
    for(i = 0; i < n; i++) {
        if(strcmp((p+i)->titre, titre) == 0) {
            /*
        la fonction strcmp() a situé dans <string.h>
        elle accepte deux chaine de caracteres comme arguments
        elle retourne 0 si les deux chaine de caracteres sont identiques

        on a trouvé le sujet, alors l'utilisateur peut maintenant modifier
        le titre
            */
            printf("\tSaisir le nouveau titre : ");
            scanf("%s", &(p+i)->titre);
            sauvegarder_sujet(p, 2);
            printf("\tSauvegarde avec succes");
            return; // c'est fini on doit arreter l'execution de la fonction
        }
    }
    /*
    On a pas trouvé la presentation
    */
    printf("\tIl n\'y a aucun presentation avec ce titre");
    choix_invalide:
    printf("\n\n\t0: Menu\n\t1: Ressayer\n\tVotre choix : ");
    scanf("%d", &choix);
    if(choix == 1) goto ressayer;
    else if (choix == 0) return;
    else goto choix_invalide;
}

/*
Cette fonctions est pour supprimer un sujet de notre liste
au debut on affiche une liste des sujets disponibles, (ou supprimable)
càd il y a aucun personne qui va fait ou qui a fait ces sujets
*/
void supp_sujet(Etudiant* e, Pres* p) {
    int i, n = nbre_sujets(), j = 0, choix, nbre_choix, verifier, menu_res;
    int *choices; // se sont les choix disponible
    ressayer:
    system("cls");
    choices = afficher_sujets_disp(e, p); // affichage des sujets supprimable + cette fonction retourne les choix disponibles, mais le premier element et le nombre des elements de ce tableau
    printf("\tEcrire l\'identifiant de votre choix : ");
    nbre_choix = *choices - 1; // le nombre de choix et le nombre des elements du tableau moins un
    scanf("%d", &choix);
    verifier = verifier_choix(choices, choix, nbre_choix); // verifier si le choix est valide ou non
    if(verifier) { // choix est valide, alors on supprime
        for(i = 0; i < n; i++) {
            if((p+i)-> id == choix) {
                j = i;
                break;
            }
        }
        for(i = j; i < n; i++) {
            *(p+i) = *(p+i+1);
        }
        sauvegarder_sujet(p, 0);
        printf("\tSujet a ete supprimme avec succees");
    } else { // choix est invalide, on informe l'utilisateur
        printf("\n\tINVALIDE CHOIX\n\n");
        ressayer_1:
        printf("0 : Menu\n1 : Ressayer\nvotre choix : ");
        scanf("%d", &menu_res);
        if(menu_res == 0) return;
        else if(menu_res == 1) goto ressayer;
        else {
            printf("CHOIX INVALIDE");
            goto ressayer_1;
        }
    }

}

/*
Meme principe qu'on a utilisé pour sauvegarder les etudiants
*/
void sauvegarder_sujet(Pres* p, int supprimer_ou_ajouter_ou_miseajour) {
    int i, x = nbre_sujets();
    FILE* sujets;
    sujets = fopen("sujets.txt", "w");
    if (supprimer_ou_ajouter_ou_miseajour == 0) x--;
    else if(supprimer_ou_ajouter_ou_miseajour == 1) x++;
    for(i = 0; i < x; i++) {
        fprintf(sujets, "%d %s\n", (p+i)->id, (p+i)->titre);
    }
    fclose(sujets);
}

/*
Cette fonctions est utile pour la fonction afficher_etudiants() et chercher_etudiant()
pcq on a besoin d'afficher le sujet de presentation d'etudiant
alors, si cet etudiant a une presentation la fonction va retourner le titre
sinon elle va retouner "Pas encore"
*/
char* fetch_sujet(Etudiant* e, Pres* p){
    int i, n = nbre_sujets();
    for(i = 0; i < n; i++) {
        if((p+i)->id == e->presentation) return (p+i)->titre;
    }
    return "Pas encore";
}


/*
Les sujets disponible est ceux qui n'ont pas de presenteur
*/
int* afficher_sujets_disp(Etudiant* e, Pres* p) {
    int i, n = nbre_sujets(), h, x = 1, j = 0; //
    int *choices; // c'est un tableau qui va contenir les choix disponible, et le nombre de ces elements comme un premier element
    choices = malloc(sizeof(int));
    printf("\t\tLES SUJETS DISPONIBLES\n\n");
    printf("\n Id\t\tSujet\n");
    printf("- - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    for(i = 0; i < n; i ++) {
        h = fetch_etudiant(e, p+i);
/*
    fetch_etudiant() retourne -1 quand elle ne trouve pas un presenteur
    alors le sujet est disponible si fetch_etudiant retourne
*/      if(h == -1) {
            x++;
            choices = realloc(choices, sizeof(int) * x);
            *(choices + j + 1) = (p+i)->id; // mettre les choix disponible sur notre tableau, mais le premier element reste vide car on va affecter x là
            j++;
            printf("%3d\t\t %10s\n", (p+i)->id, (p+i)->titre);
        }
    }

    *choices = x; // premier element du tableau est le nombre de ces elements, c'est à dire le nombre des choix disponible est (*choices - 1)
    return choices;
}

/*
Une fonction pour afficher les presentations qui ont des presenteurs est qui ont
pas encore fait dans la classe
*/
int* afficher_pres_afaire(Etudiant* e, Pres* p) {
    int i, n = nbre_sujets(), h, x = 1, j = 0;
    int* choices;
    choices = malloc(sizeof(int));
    printf("\t\tLES SUJETS A FAIRE\n\n");
    printf("\n Id\t\tSujet\t\tPresenteur\n");
    printf("- - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    for(i = 0; i < n; i ++) {
        h = fetch_etudiant(e, p+i);
        if(h != -1 && (e+h)->disp != -1) {
        /*
            fetch_etudiant() retourne -1 quand elle ne trouve pas un presenteur
            alors il faut que fetch_etudiant ne retourne pas -1

            si h!=-1 alors le sujet a un presenteur, et fetch_etudiant a retourner
            l'index de ce presenteur sur le tableau dynamique
            alors il suffit qu'on s'assure que ce sujet n'a pas été fait dans la classe
            càd disponibilité doit different de -1 (égal a 0 :) )
        */
            x++;
            choices = realloc(choices, sizeof(int) * x);
            *(choices + j + 1) = ((p+i)->id);
            j++;
            printf("%3d\t\t %10s\t\t%s %s\n", (p+i)->id, (p+i)->titre, (e+h)->nom, (e+h)->prenom);
        }
    }
    *choices = x;
    return choices;
}

/*
Une fonction pour qui retourne une presentation, qui a un presenteur avec un id égal à x
*/
Pres* chercher_p(Pres* p, int x) {
    int i, n = nbre_sujets();
    for(i = 0; i < n; i++) {
        if(x == (p+i)->id) return (p+i);
    }
}

/*
Une fonction pour chercher un sujet
*/
void chercher_sujet(Etudiant* e, Pres* p) {
    int i, n, h, choix;
    char titre[CMAX];
    n = nbre_sujets();
    ressayer:
    system("cls");
    printf("\tSaisir le titre du presentation : ");
    scanf("%s", titre);
    for(i = 0; i < n; i++) {
        if(strcmp(titre, (p+i)->titre) == 0) {
            h = fetch_etudiant(e, p+i);
            printf("\n ID\tTITRE\t\t\tPresenteur\t\tFaite?");
            if(h != -1) printf("\n\n%d\t%s\t\t%s %s\t\t%c", (p+i)->id, (p+i)->titre, (e+h)->nom, (e+h)->prenom, ((e+h)->disp == -1) ? 'X' : '_' );
            else printf("\n\n %d\t%s\t\t%10s\t\t%c", (p+i)->id, (p+i)->titre, "Aucun etudiant", '_');
            return;
        }
    }
    printf("\n\n\tSujet introuvable!\n");
    err_handl:
    printf("\n\t0 : Menu\n\t1 : Ressayer\n\tVotre choix : ");
    scanf("%d", &choix);
    if(choix == 1) goto ressayer;
    else if(choix == 0) return;
    else {
        printf("\n\n\tchoix invalide");
        goto err_handl;
    }
}

/*
Une fonctions pour afficher les sujets en ordre alphabetic
*/
void afficher_sujets(Etudiant* e, Pres* p) {
    int i, n = nbre_sujets(), h;
    printf("\n Id\tSujet\t\t\tPresenteur\t\t\tFaite?\n");
    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    trier_s(p);
    for(i = 0; i < n; i++){
        h = fetch_etudiant(e, p+i);
        if(h!=-1) printf("%d\t%10s\t\t%s %s\t\t\t%c\n", (p+i)->id, (p+i)->titre, (e+h)->nom, (e+h)->prenom, (((e+h)->disp)==-1) ? 'X' : '_');
        else printf("%d\t%10s\t\t%s %s\t\t\t%c\n", (p+i)->id, (p+i)->titre, "aucun", "etudiant", '_');
    }
    charger_donnees(e, p); /* tres important pcq les autres fonctions ont besoin
     d'un tableau qui contient les donnes des fichiers qui ont trié par l'ordre
     du ID
     */
}


///////////////////**** OPERATIONS SUR LES PRESENTATIONS ****//////////////////////

/*
Affecter une presentation a un etudiant
*/
void assigner(Etudiant* e, Pres* p) {
    int id_choix, verifier, nbre_choix, menu_res;
    int *choices;
    printf("\tSaisir le nom d\'etudiant : ");
    Etudiant* etudiant;

    etudiant = selectionner_etudiant(e);
    if(etudiant->disp == 0) printf("\tCette personne a deja une presentation a faire\n");
    else if(etudiant->disp == -1) printf("\tCette personne a deja fait une presentation");
    else { // l'etudiant est disponible
        ressayer:
        system("cls");
        choices = afficher_sujets_disp(e, p); // afficher les sujets disponibles
        nbre_choix = *choices - 1;
        printf("\tSaisir le ID do votre choix : ");
        scanf("%d", &id_choix);
        verifier = verifier_choix(choices, id_choix, nbre_choix);
        if(verifier) {
            etudiant->disp = 0; // l'étudiant n'est plus disponible
            etudiant->presentation = (chercher_p(p, id_choix))->id; // affecter la presentation a l'etudiant
            printf("\t%s %s va presenter le sujet \"%s\"\nEnregistre avec succes!", etudiant->nom, etudiant->prenom, (chercher_p(p, id_choix))->titre);
            sauvegarder_etudiant(e, 2);
        } else { // choix est invalide, on informe l'utilisateur
            printf("\n\tINVALIDE CHOIX\n\n");
            ressayer_1:
            printf("0 : Menu\n1 : Ressayer\nvotre choix : ");
            scanf("%d", &menu_res);
            if(menu_res == 0) return;
            else if(menu_res == 1) goto ressayer;
            else {
                printf("CHOIX INVALIDE");
                goto ressayer_1;
            }
        }
    }
}

/*
Une fonction pour marqué une presentation comme faite
*/
void fait(Etudiant* e, Pres* p) {
    int choix, verifier, menu_res, nbre_choix;
    int *choices;
    Pres* prsnt; // c'est la presentation qui a été faite
    Etudiant* etdnt; // c'est le presenteur
    ressayer:
    system("cls");
    choices = afficher_pres_afaire(e, p);
    nbre_choix = *choices - 1;
    printf("\tSaisir le ID de la presentation qui a ete faite : ");
    scanf("%d", &choix);
    verifier = verifier_choix(choices, choix, nbre_choix);
    if(verifier) {
        prsnt = chercher_p(p, choix);
        etdnt = chercher_e(e, (prsnt->id));
        etdnt->disp = -1;
        printf("\t%s %s a fait la presentation \" %s \"", etdnt->nom, etdnt->prenom, prsnt->titre);
        sauvegarder_etudiant(e, 2);
    } else { // choix est invalide, on informe l'utilisateur
        printf("\n\tINVALIDE CHOIX\n\n");
        ressayer_1:
        printf("0 : Menu\n1 : Ressayer\nvotre choix : ");
        scanf("%d", &menu_res);
        if(menu_res == 0) return;
        else if(menu_res == 1) goto ressayer;
        else {
            printf("CHOIX INVALIDE");
            goto ressayer_1;
        }
    }

}

/*
Une fonctions pour changer le presenteur
*/
void changer_presenteur(Etudiant* e, Pres* p) {
    int choix, nbre_choix, menu_res, verifier;
    int *choices;
    Pres* prsnt; // presentation
    Etudiant* e_ancn; // Presenteur ancien
    Etudiant* nv_e; // nouveau presenteur
    ressayer:
    system("cls");
    choices = afficher_pres_afaire(e, p); // On affiche les presentations à faire
    nbre_choix = *choices - 1;
    printf("\tSaisir le ID de presentations : ");
    scanf("%d", &choix);
    verifier = verifier_choix(choices, choix, nbre_choix);
    if(verifier) {
        prsnt = chercher_p(p, choix); // cherchons la presentation
        e_ancn = chercher_e(e, (prsnt->id)); // cherchons l'etudiant ancien
        printf("\t%s %s ne presenterai plus le sujet \"%s\"\n", e_ancn->nom, e_ancn->prenom, prsnt->titre);
        printf("\tSaisir le nom du nouveau presenteur : ");
        nv_e = selectionner_etudiant(e); // on selectionne le nouveau etudiant
        if(nv_e->disp == 0) printf("\tCette personne a deja une presentation a faire\n");
        else if(nv_e->disp == -1) printf("\tCette personne a deja fait une presentation");
        else {
            printf("\tMaintenant %s %s est le presenteur du sujet\"%s\"", nv_e->nom, nv_e->prenom, prsnt->titre);
            e_ancn->disp = 1; // l'ancien etudiant est maintenant disponible
            e_ancn->presentation = 0; // l'ancient etudiant n'a pas une presentation a faire maintenant
            nv_e->disp = 0; // Le nouveau presenteur est pas disponible maintenant
            nv_e->presentation = prsnt->id; // On affect le id de la presentation a le nouveau presenteur
            sauvegarder_etudiant(e, 2); // Sauvegardement des changement
            printf("\n\tChangements ont ete sauvegarde avec succes");
        }
    } else { // choix est invalide, on informe l'utilisateur
        printf("\n\tINVALIDE CHOIX\n\n");
        ressayer_1:
        printf("0 : Menu\n1 : Ressayer\nvotre choix : ");
        scanf("%d", &menu_res);
        if(menu_res == 0) return;
        else if(menu_res == 1) goto ressayer;
        else {
            printf("CHOIX INVALIDE");
            goto ressayer_1;
        }
    }
}

/*
Une fonction qui nous serve si un etudiant ne veut plus presenter sa presentation
*/
void abandonner(Etudiant* e) {
    Etudiant* e_abndn;
    printf("\tSaisir le nom d\'etudiant qui veut abandonner : ");
    e_abndn = selectionner_etudiant(e);
    if(e_abndn->disp != 0) printf("\tCet etudiant n\'a pas une presentation a faire");
    else {
        e_abndn->disp = 1; // l'etudiant est maiantenant disponible
        e_abndn->presentation = 0;
        printf("\t%s %s a abandonne sa presentation avec succes\n", e_abndn->nom, e_abndn->prenom);
        sauvegarder_etudiant(e, 2);
    }
}
