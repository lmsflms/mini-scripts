#include <iostream>
#include <vector>

// NOTES :
// - On peut vérifier que x et y ont au moins 2 points (pour pas calculer inutilement)
// - Les arguments de la fonction sont dupliqués.

//Procédure qui approxime un jeu de données par une droite (polynôme de degré 1).
void coefficients_droite(std::vector<double> x, std::vector<double> y) {
    double somme_x_i = 0, somme_y_i = 0, somme_x_i_y_i = 0, somme_xx_i = 0;

    if (x.size() != y.size()) { //cas où les deux vecteurs n'ont pas la même taille
        return;
    }

    if (x.size() == 0) { //cas où la taille des deux vecteurs est nulle (donc N ne sera pas nul)
        return;
    }

    int N = x.size(); //N = nombre de points

    for (int i = 0; i < N; i++) { //calcul de toute les sommes nécessaires
        somme_x_i += x[i];
        somme_y_i += y[i];
        somme_x_i_y_i += x[i]* y[i];
        somme_xx_i += x[i] * x[i];
    }

    double denominateur_a = (N * somme_xx_i - (somme_x_i * somme_x_i));

    if (denominateur_a == 0) {
        return;
    }

    double a = (N * somme_x_i_y_i - somme_x_i * somme_y_i) / denominateur_a;
    double b = (1.0 / N) * (somme_y_i - a * somme_x_i); //il faut bien mettre 1.0, sinon on obtient une opération d'entiers.

    std::cout << "y = " << a << "x + " << b;
}

int main() {
    //jeu de données
    std::vector<double> x = { 1,2,4,5,6,8 };
    std::vector<double> y = { 2,3,8,12,10,13 };

    coefficients_droite(x, y); //appel de la procédure
}