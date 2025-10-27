#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#include <string>
#endif // _WIN32
#include <iomanip>

void affichage(std::vector<std::vector<float>> tab) {
    //Affiche les tableaux de valeurs avec des caractères spéciaux.
    for (auto lignes : tab) {
        for (auto col : lignes) {
            if (col == 0)
                std::cout << " ";
            else if (col <= 1)
                std::cout << "#";
            else if (col <= 2)
                std::cout << "▒";
            else if (col < 3)
                std::cout << "▓";
            else
                std::cout << "█";
        }
        std::cout << "\n";
    }
}

int main(){
    #ifdef _WIN32
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
    #endif // _WIN32

    std::ifstream fluxIn("input.csv");
    std::ofstream fluxOut("output.csv");

    std::vector <std::vector<float>> tabCsv;

    if (!fluxIn.is_open())
        std::cerr << "Problème d'ouverture du fichier \"input.csv\".\n";
    else {
        std::string ligne;
        while (std::getline(fluxIn, ligne)) {
            std::stringstream sstr(ligne);
            std::string cellule;
            std::vector <float> ligneVec;
            
            while (std::getline(sstr, cellule, ';')) {
                ligneVec.push_back(std::stof(cellule));
            }
            tabCsv.push_back(ligneVec);
        }
        fluxIn.close();
    }

    std::vector <std::vector<float>> tab = tabCsv;

    for (int i = 0; i < tab.size()-1; i++) {
        for (int j = 0; j < tab[i].size(); j++) {
            if (tab[i][j] > 0) {
                if (tab[i + 1][j] == 0) {
                    tab[i + 1][j] = tab[i][j] - 1;
                }
            }
        }
    }

    //Enregistrement du flux dans output.csv
    for (auto lignes : tab) {
        for (auto col : lignes) {
            fluxOut << col << ';';
        }
        fluxOut << '\n';
    }
    fluxOut.close();

    affichage(tabCsv);
    std::cout << "\n";
    affichage(tab);
}

