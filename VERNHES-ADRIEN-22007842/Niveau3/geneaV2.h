#ifndef GENEAV2_H
#define GENEAV2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LG_DATE 10
#define LG_NOM 10
#define LG_PRENOM 10

struct sArbre {
    struct sFiche *pPremiere;
    struct sFiche *pDerniere;
};

struct tIdentite {
    int Identifiant;
    char *Nom;
    char *Prenom;
    char Sexe;
    char DateNaissance[LG_DATE + 1];
};




struct sFiche {
    struct tIdentite Identite; 
    struct sFiche *pPere;
    struct sFiche *pMere;
    struct sFiche *pSuivante;
};

typedef struct sArbre *tArbre;
typedef struct sFiche *tFiche;
tArbre ArbreCreer(void);
void ArbreAfficher(tArbre Arbre);
void ArbreAjouterPersonne(tArbre Arbre, struct tIdentite Identite);
void ArbreLiberer(tArbre Arbre);
tArbre ArbreLirePersonnesFichier(char Fichier[]);
void ArbreAjouterLienParente(tArbre Arbre, int IdEnfant, int IdParent, char Parente);
int ArbreLireLienParentef(FILE *f, int *pIdEnfant, int *pIdParent, char *pParente);

tArbre ArbreLireLienParenteFichier(tArbre Arbre, char Fichier[]);

void ArbreEcrireGV(tArbre Arbre, char Fichier[]);
void ArbreAfficherAscendants(tArbre Arbre, int Identifiant);
void ArbreEcrireAscendantsGV(tArbre Arbre, int Identifiant, char Fichier[]);

#endif 
