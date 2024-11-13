#include "jeu.hpp"

JeuDevinette::JeuDevinette() {
    initialiserNiveaux();
    initialiserAchievements();
    nombreEssais = 0;
    partieGagnee = false;
    joueurActuel = {"", 1, 0, 0};
    statistiques = {0, 0, 0, {}};
}

void JeuDevinette::initialiserNiveaux() {
    niveaux.push_back(Difficulte(15, 50, "Facile"));
    niveaux.push_back(Difficulte(10, 100, "Moyen"));
    niveaux.push_back(Difficulte(5, 200, "Difficile"));
}

void JeuDevinette::initialiserAchievements() {
    achievements = {
        {"Première Victoire", "Gagnez votre première partie", false, 0, 1},
        {"Devin", "Gagnez 10 parties", false, 0, 10},
        {"Score Parfait", "Trouvez le nombre en 3 essais ou moins", false, 0, 1},
        {"Expert", "Gagnez une partie en mode Difficile", false, 0, 1},
        {"Persévérant", "Jouez 20 parties", false, 0, 20},
        {"Score Elite", "Obtenez un score de 5000 points ou plus", false, 0, 1},
        {"Polyvalent", "Gagnez au moins une fois dans chaque difficulté", false, 0, 3},
        {"Chanceux", "Trouvez le nombre du premier coup", false, 0, 1}
    };
}

void JeuDevinette::afficherMenu() const {
    std::cout << "\nBienvenue dans le jeu Devine le nombre !" << std::endl;
    std::cout << "Choisissez votre niveau de difficulté :" << std::endl;
    for (size_t i = 0; i < niveaux.size(); i++) {
        std::cout << i + 1 << ". " << niveaux[i].nom 
                 << " (1-" << niveaux[i].maxNombre 
                 << ", " << niveaux[i].maxEssais 
                 << " essais)" << std::endl;
    }
}

void JeuDevinette::choisirDifficulte() {
    int choix;
    do {
        std::cout << "Votre choix (1-" << niveaux.size() << ") : ";
        std::cin >> choix;
    } while (choix < 1 || choix > static_cast<int>(niveaux.size()));
    
    difficulteChoisie = &niveaux[choix - 1];
    nombreSecret = rand() % difficulteChoisie->maxNombre + 1;
}

void JeuDevinette::jouer() {
    int tentative;
    nombreEssais = 0;
    partieGagnee = false;
    
    std::cout << "J'ai choisi un nombre entre 1 et " 
              << difficulteChoisie->maxNombre << "." << std::endl;
    
    do {
        std::cout << "\nIl vous reste " 
                  << (difficulteChoisie->maxEssais - nombreEssais) 
                  << " essais." << std::endl;
        std::cout << "Entrez votre proposition : ";
        std::cin >> tentative;
        nombreEssais++;
        
        if (nombreEssais >= difficulteChoisie->maxEssais && tentative != nombreSecret) {
            std::cout << "Game Over ! Le nombre était " << nombreSecret << std::endl;
            break;
        }
        
        if (tentative < nombreSecret) {
            std::cout << "C'est plus grand !" << std::endl;
        } else if (tentative > nombreSecret) {
            std::cout << "C'est plus petit !" << std::endl;
        } else {
            std::cout << "Bravo ! Vous avez trouvé en " << nombreEssais << " essais !" << std::endl;
            partieGagnee = true;
        }
    } while (tentative != nombreSecret && nombreEssais < difficulteChoisie->maxEssais);
}

bool JeuDevinette::rejouer() const {
    char reponse;
    std::cout << "\nVoulez-vous rejouer ? (o/n) : ";
    std::cin >> reponse;
    return (reponse == 'o' || reponse == 'O');
}

void JeuDevinette::reinitialiser() {
    nombreEssais = 0;
    partieGagnee = false;
}

int JeuDevinette::calculerScore() const {
    int score = difficulteChoisie->maxNombre * 100;
    score -= (nombreEssais - 1) * 50;
    score += (difficulteChoisie->maxEssais - nombreEssais) * 100;
    return std::max(0, score);
}

void JeuDevinette::sauvegarderScore() {
    Score nouveauScore{
        joueurActuel.nom,
        calculerScore(),
        difficulteChoisie->nom,
        nombreEssais
    };
    highScores.push_back(nouveauScore);
    std::sort(highScores.begin(), highScores.end(),
              [](const Score& a, const Score& b) { 
                  return a.score > b.score; 
              });
}

void JeuDevinette::setNomJoueur(const std::string& nom) {
    joueurActuel.nom = nom;
}

void JeuDevinette::afficherHighScores() const {
    std::cout << "\n=== High Scores ===" << std::endl;
    for (const auto& score : highScores) {
        std::cout << score.nomJoueur << " : " 
                  << score.score << " points ("
                  << score.difficulte << ", "
                  << score.nbEssais << " essais)" << std::endl;
    }
}

void JeuDevinette::verifierAchievements() {
    verifierAchievementPartie();
    verifierAchievementScore();
    verifierAchievementDifficulte();
}

void JeuDevinette::verifierAchievementPartie() {
    if (partieGagnee) {
        statistiques.partiesGagnees++;
        
        // Première Victoire
        if (!achievements[0].debloque && statistiques.partiesGagnees == 1) {
            achievements[0].debloque = true;
            std::cout << "\nAchievement débloqué : " << achievements[0].nom << std::endl;
        }
        
        // Maître Devin
        if (!achievements[1].debloque) {
            achievements[1].progression = statistiques.partiesGagnees;
            if (achievements[1].progression >= achievements[1].objectif) {
                achievements[1].debloque = true;
                std::cout << "\nAchievement débloqué : " << achievements[1].nom << std::endl;
            }
        }
        
        // Score Parfait
        if (!achievements[2].debloque && nombreEssais <= 3) {
            achievements[2].debloque = true;
            std::cout << "\nAchievement débloqué : " << achievements[2].nom << std::endl;
        }
        
        // Chanceux
        if (!achievements[7].debloque && nombreEssais == 1) {
            achievements[7].debloque = true;
            std::cout << "\nAchievement débloqué : " << achievements[7].nom << std::endl;
        }
    }
    
    statistiques.partiesJouees++;
    
    // Persévérant
    if (!achievements[4].debloque) {
        achievements[4].progression = statistiques.partiesJouees;
        if (achievements[4].progression >= achievements[4].objectif) {
            achievements[4].debloque = true;
            std::cout << "\nAchievement débloqué : " << achievements[4].nom << std::endl;
        }
    }
}

void JeuDevinette::verifierAchievementScore() {
    int scoreActuel = calculerScore();
    statistiques.meilleurScore = std::max(statistiques.meilleurScore, scoreActuel);
    
    // Score Elite
    if (!achievements[5].debloque && scoreActuel >= 5000) {
        achievements[5].debloque = true;
        std::cout << "\nAchievement débloqué : " << achievements[5].nom << std::endl;
    }
}

void JeuDevinette::verifierAchievementDifficulte() {
    if (partieGagnee) {
        statistiques.victairesParDifficulte[difficulteChoisie->nom]++;
        
        // Expert
        if (!achievements[3].debloque && difficulteChoisie->nom == "Difficile") {
            achievements[3].debloque = true;
            std::cout << "\nAchievement débloqué : " << achievements[3].nom << std::endl;
        }
        
        // Polyvalent
        if (!achievements[6].debloque) {
            int difficultesMaitrisees = 0;
            for (const auto& niveau : niveaux) {
                if (statistiques.victairesParDifficulte[niveau.nom] > 0) {
                    difficultesMaitrisees++;
                }
            }
            achievements[6].progression = difficultesMaitrisees;
            if (difficultesMaitrisees >= 3) {
                achievements[6].debloque = true;
                std::cout << "\nAchievement débloqué : " << achievements[6].nom << std::endl;
            }
        }
    }
}

void JeuDevinette::afficherAchievements() const {
    std::cout << "\n=== Achievements ===" << std::endl;
    for (const auto& achievement : achievements) {
        std::cout << (achievement.debloque ? "[X] " : "[ ] ")
                  << achievement.nom << " - " 
                  << achievement.description;
        
        if (!achievement.debloque && achievement.progression > 0) {
            std::cout << " (" << achievement.progression 
                      << "/" << achievement.objectif << ")";
        }
        std::cout << std::endl;
    }
} 