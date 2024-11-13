#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <limits>
#include "jeu.hpp"

void viderBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void afficherMenuPrincipal() {
    std::cout << "\n=== MENU PRINCIPAL ===" << std::endl;
    std::cout << "1. Jouer une partie" << std::endl;
    std::cout << "2. Voir les high scores" << std::endl;
    std::cout << "3. Voir les achievements" << std::endl;
    std::cout << "4. Changer de joueur" << std::endl;
    std::cout << "5. Quitter" << std::endl;
}

int main() {
    srand(time(0));
    JeuDevinette jeu;
    
    // Configuration initiale du joueur
    std::string nom;
    std::cout << "=== Bienvenue dans le Jeu Devine le Nombre ===" << std::endl;
    std::cout << "Entrez votre nom : ";
    std::getline(std::cin, nom);
    jeu.setNomJoueur(nom);
    
    bool quitter = false;
    int choix;
    
    while (!quitter) {
        afficherMenuPrincipal();
        std::cout << "\nVotre choix : ";
        std::cin >> choix;
        viderBuffer();
        
        switch (choix) {
            case 1: // Jouer une partie
                do {
                    jeu.afficherMenu();
                    jeu.choisirDifficulte();
                    jeu.jouer();
                    jeu.sauvegarderScore();
                    jeu.verifierAchievements();
                    jeu.afficherHighScores();
                    jeu.reinitialiser();
                } while (jeu.rejouer());
                break;
                
            case 2: // Voir les high scores
                jeu.afficherHighScores();
                std::cout << "\nAppuyez sur Entrée pour continuer...";
                std::cin.get();
                break;
                
            case 3: // Voir les achievements
                jeu.afficherAchievements();
                std::cout << "\nAppuyez sur Entrée pour continuer...";
                std::cin.get();
                break;
                
            case 4: // Changer de joueur
                std::cout << "Entrez le nom du nouveau joueur : ";
                std::getline(std::cin, nom);
                jeu.setNomJoueur(nom);
                std::cout << "Joueur changé avec succès !" << std::endl;
                break;
                
            case 5: // Quitter
                quitter = true;
                break;
                
            default:
                std::cout << "Choix invalide. Veuillez réessayer." << std::endl;
                break;
        }
    }
    
    std::cout << "\nMerci d'avoir joué ! À bientôt !" << std::endl;
    return 0;
}
