#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <cmath>

typedef std::vector<int> list;
typedef std::vector<std::vector<int>> tableau;

template<typename t>
void printi(t message, int tabulations = 0, int endlines = 0) {
    // Tabulations
    for (int i = 0; i < tabulations; i++) {
        std::cout << "\t";
    }
    // Message
    std::cout << message;
    // Endlines
    for (int i = 0; i < endlines; i++) {
        std::cout << "\n";
    }
}

void afficher_plateau(list noeud) {
    printi("", 0, 2);
    // Afficher le plateau de jeu
    printi("     ", 3);
    for (int j = 0; j < noeud.size(); j++) {
        printi(j);
        printi("   ");
    }
    printi("\n");
    printi("   ", 3);
    for (int j = 0; j < noeud.size(); j++) {
        printi("----");
    }
    printi("\n");
    for (int i = 0; i < noeud.size(); i++) {
        printi(i, 3);
        printi("  | ");
        for (int j = 0; j < noeud.size(); j++) {
            if (noeud[j] == i) {
                printi("R | ");
            }
            else {
                std::cout << " ";
                std::cout << " | ";
            }

        }
        printi("", 0, 1);
        printi("   ", 3);
        for (int i = 0; i < noeud.size(); i++) {
            printi("----");
        }
        printi("", 0, 1);
    }
    printi("", 0, 3);
    for (int j = 0; j < noeud.size(); j++) {

    }
}

/** Tirage aléatoire de noeud de départ
 *
 *  @param dimension : nombres de reines
 *                   sur le plateau de jeu
 *  @return noeud    : noeud tiré
**/
list tirer_noeud_depart(int dimension) {
    // Declaration
    list noeud(dimension);

    // Initialiser hasard
    srand(time(NULL));

    // Parcourt de noeud
    for (int i = 0; i < dimension; i++) {
        noeud[i] = rand() % (dimension);
    }

    // Retourner noeud courant
    return noeud;
}

/** Calcul des noeuds adjacents au noeud courant
 *
 *  @param  noeud_courant    : noeud_courant dont on souhaite
 *                           calculer les noeuds adjacents
 *  @return noeuds_adjacents : Tableau contenant toutes les 
 *                           configurations adjacentes au noeud
 *                           courant
**/
tableau calculer_noeuds_adjacents(list noeud_courant) {
    // Declaration
    int dim = noeud_courant.size();
    tableau noeuds_ajacents(dim*dim - dim - 1, noeud_courant);
    int k = 0; // Compteur d'initialisation de noeuds_adjacents

    // On parcourt les colonnes du plateau
    for (int i = 0; i < dim; i++) {
        // Pour chaque reine, on parcourt toutes les cases du dessus et du dessous
        for (int j = noeud_courant[i] - 1; j > noeud_courant[i] - (dim - 1); j--) {
            if (j == 0) continue;             // Si le déplacement est nul, on passe l'itération
            else {
                noeuds_ajacents[k][i] -= j;  // Sinon on déplace la reine et on stocke le resultat
                k++;
            }
        }
    }

    // Retourner noeuds
    return noeuds_ajacents;
}

/** Vérifie si la paire de reine est en ligne d'attaque
 *
 *  @param  reine1 et reine 2: Paire de reine à vérifier
 *  @return en_ligne         : true si les reines peuvent s'attaquer
 *                             false sinon
**/
bool en_ligne_de_vue(list reine1, list reine2) {
    // Déclaration
    double angle;
    bool en_ligne = false;

    // Calcul du vecteur de translation du pion 1 au pion 2
    list vecteur = { reine2[0] - reine1[0], reine2[1] - reine1[1] };
    // Conversion des coordonnées cartésiennes en polaires
    angle = std::atan2(vecteur[0], vecteur[1]);

    // Si l'angle est un angle de ligne de vue de la reine
    if (angle == 0            ||
        angle == M_PI         || angle == -M_PI ||
        angle == M_PI / 2     || angle == -M_PI / 2 ||
        angle == M_PI / 4     || angle == -M_PI / 4 ||
        angle == 3 * M_PI / 4 || angle == -3 * M_PI / 4) {
        en_ligne = true;
    }

    // Retourner le résultat du calcul
    return en_ligne;
}

/** Fonction de valeur du noeud 
 *
 *  @param  noeud : Noeud dont on souhaite calculer la valeur
 *  @return value : Image de noeud par la fonction de valeur
**/
int valeur(list noeud) {
    // Déclaration
    int k = 0;     // Compteur de colonne
    int r = 1;     // Compteur de la reine à regarder en premier
    int value = 0; // Valeur du noeud

    for (k; k < noeud.size() - 1; k++) {
        for (int i = r; i < noeud.size(); i++) {
            /* Si le segment borné par la paire de reines est une droite
            horizontale, verticale, ou diagonale */
            if (en_ligne_de_vue(list({ k, noeud[k] }), list({i, noeud[i]}))) {
                // On incrémente la valeur du noeud
                value++;
            }
        }
        r++; /* On incrémente le compteur de la première reine pour ne
             pas recalculer une paire de reine précédente */
    }

    return value;
}

list hillclimbingbruteforce() {
    // Déclarations
    int coups = 0;
    list noeud_courant;
    tableau noeuds_adjacents;
    tableau noeuds_potentiels; /* Noeuds n' dont l'image par valeur() est
                               inférieure à l'image par valeur() de n courant */
    int dimensions;

    // Initialisation
    std::cout << "Entrer dimension : ";
    std::cin >> dimensions;
    int valeurNoeud = 1;
    while (valeurNoeud != 0) {
        noeud_courant = tirer_noeud_depart(dimensions);

        // Climbing loop
        while (true) {
            // Affichage
            system("clear"); // Effacer la console
            std::cout << "Coups effectues : " << coups;
            afficher_plateau(noeud_courant);

            // On calcule les noeuds adjacents au noeud courant
            noeuds_adjacents = calculer_noeuds_adjacents(noeud_courant);

            // On parcourt les noeuds adjacents et on stocke les noeuds avec la meilleure valeur
            for (int i = 0; i < noeuds_adjacents.size(); i++) {
                // Si la valeur du noeud est meilleure que la valeur courante
                if (valeur(noeuds_adjacents[i]) < valeur(noeud_courant)) {
                    // S'il existe déjà un noeud potentiel
                    if (noeuds_potentiels.size() > 0) {
                        /* Si le nouveau noeud est meilleur, la liste des noeuds potentiels
                        est vidée et on y stocke le nouveau noeud */
                        if (valeur(noeuds_adjacents[i]) < valeur(noeuds_potentiels[0])) {
                            noeuds_potentiels = { noeuds_adjacents[i] };
                        }
                        /* Sinon si le nouveau noeud est de même valeur que les autres noeuds
                        potentiels, on le rajoute juste à la liste */
                        else if (valeur(noeuds_adjacents[i]) == valeur(noeuds_potentiels[0])) {
                            noeuds_potentiels.push_back(noeuds_adjacents[i]);
                        }
                    }
                    /* Sinon le noeud est le premier potentiel et on initialise juste la liste avec
                    ce noeud */
                    else {
                        noeuds_potentiels.push_back(noeuds_adjacents[i]);
                    }
                }
            }

            // S'il n'y a pas de meilleur noeud adjacent, on retourne le courant
            if (noeuds_potentiels.size() == 0) {
                break;
            }
            // S'il y a plusieurs noeuds potentiels, on en tire un au hasard
            else if (noeuds_potentiels.size() > 1) {
                noeud_courant = noeuds_potentiels[rand() % noeuds_potentiels.size()];
            }
            // Sinon on garde le seul noeud potentiel comme courant
            else {
                noeud_courant = noeuds_potentiels[0];
            }

            coups++; // On incrémente le nombre de coups effectués
            noeuds_potentiels = tableau(); // On réinitialise les noeuds potentiels
        }
        valeurNoeud = valeur(noeud_courant);
    }
    return noeud_courant;
}

int main() {
    list noeud = hillclimbingbruteforce();
    int valeurNoeud = valeur(noeud);

    std::cout << "L'algorithme est tombé sur une solution !";
    std::cout << "\n\n";
    for (int i = 0; i < noeud.size(); i++) {
        std::cout << noeud[i] << " ";
    }

    std::cin.ignore();
    std::cin.get();

    return 0;
}