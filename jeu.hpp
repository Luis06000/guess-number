#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <map>

// Déclaration anticipée des structures
struct Achievement;
struct Score;
struct Joueur;

class Difficulte {
public:
    int maxEssais;
    int maxNombre;
    std::string nom;
    
    Difficulte(int essais, int nombre, std::string n) 
        : maxEssais(essais), maxNombre(nombre), nom(n) {}
};

// Définition des structures
struct Score {
    std::string nomJoueur;
    int score;
    std::string difficulte;
    int nbEssais;
};

struct Achievement {
    std::string nom;
    std::string description;
    bool debloque;
    int progression;
    int objectif;
};

struct Joueur {
    std::string nom;
    int niveau;
    int experience;
    int pieces;
};

class JeuDevinette {
private:
    std::vector<Difficulte> niveaux;
    std::vector<Score> highScores;
    std::vector<Achievement> achievements;
    Joueur joueurActuel;
    int nombreSecret;
    int nombreEssais;
    Difficulte* difficulteChoisie;
    bool partieGagnee;
    void initialiserAchievements();
    void verifierAchievementPartie();
    void verifierAchievementScore();
    void verifierAchievementDifficulte();
    
public:
    JeuDevinette();
    void initialiserNiveaux();
    void afficherMenu() const;
    void choisirDifficulte();
    void jouer();
    void reinitialiser();
    bool rejouer() const;
    
    // Nouvelles méthodes
    int calculerScore() const;
    void sauvegarderScore();
    void afficherHighScores() const;
    void verifierAchievements();
    void afficherAchievements() const;
    void setNomJoueur(const std::string& nom);
    int getNombrePartiesJouees() const { return statistiques.partiesJouees; }
    int getMeilleurScore() const;
    
private:
    struct Statistiques {
        int partiesJouees;
        int partiesGagnees;
        int meilleurScore;
        std::map<std::string, int> victairesParDifficulte;
    };
    Statistiques statistiques;
}; 