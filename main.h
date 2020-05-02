#define CMAX 100

    typedef struct SEtudiant {
        int id;
        char nom[CMAX];
        char prenom[CMAX];
        int disp; /* disponibilité de l'etudiant, il y a 3 cas
        disp = 1 => etudiant est disponible
        disp = 0 => etudiant a une presentation a faire
        disp = -1 => etudiant a deja fait une presentation
        */
        int presentation;
    } Etudiant;

    typedef struct SPresentation {
        int id;
        char titre[CMAX];
    } Pres;

