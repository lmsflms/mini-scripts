#include <vector>
#include <iostream>
#include <iomanip>

using matrice = std::vector<std::vector<double>>; //utilisation d'un alias pour que ça soit plus lisible.

//juste pour afficher la matrice
void afficher_matrice(matrice A) {
	for (auto i : A) {
		for (auto val : i) {
			std::cout << std::setw(4) << val << std::setw(4) << "|";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

//Fonction qui résout le système A * X = B en utilisant le pivot de Gauss.
std::vector<double> pivot_de_gauss(matrice A, matrice B) {
	int n = A.size();

	// vérifier que A_0,0 != 0, sinon on échange avec la ligne suivante.
	for (int i = 0; i < n - 1; i++) { //si le pivot est nul sur la derniere ligne
		if (A[i][i] == 0) { //A[i][i] est forcément une diagonales puisque c'est quand i et j ont la même valeur.

			//on cherche si il y a une ligne en dessous avec pivot non nul
			bool echange = false; //booléen qui vérifie si il y a une échange
			int ligne_a_echanger; //variable avec l'incide de la ligne à échanger

			for (int l = i + 1; l < n; l++) { //i+1 pour ignorer la ligne au dessus
				if (A[l][i] != 0) { //la ligne suivant à un pivot valide
					echange = true;
					ligne_a_echanger = l;
					break; //stop à la 1e ligne valide
				}
			}

			//si on a trouvé une ligne valide, on échange les lignes
			if (echange) {
				std::vector<double> ligne_temporaire = A[i]; //suppr, on pourrait utiliser std::swap(A[i], A[ligne_a_echanger]); mais on l'a pas vraiment vu
				A[i] = A[ligne_a_echanger];
				A[ligne_a_echanger] = ligne_temporaire;

				std::vector<double> ligne_temporaire_B = B[i];
				B[i] = B[ligne_a_echanger];
				B[ligne_a_echanger] = ligne_temporaire_B;
			}
		}

		//mettre zéro sous le pivot
		for (int j = i + 1; j < n; j++) {
			if (A[j][i] != 0) { //si il n'y a pas déjà 0 sous le pivot
				double val_elimination = A[j][i] / A[i][i]; //combien de fois la ligne i doit être multipliée pour que A[i][j] soit nul

				for (int k = i; k < n; k++) {
					A[j][k] -= val_elimination * A[i][k]; //on soustrait A[i][k] fois val_elimination
				}

				//on fait la même chose dans B
				for (int k = 0; k < B[0].size(); k++) {
					B[j][k] -= val_elimination * B[i][k];
				}
			}
		}
	}

	std::vector<double> X(n);

	for (int i = n - 1; i >= 0; i--) {
		double somme = 0.0;
		for (int j = i + 1; j < n; j++) {
			somme += A[i][j] * X[j];
		}
		X[i] = (B[i][0] - somme) / A[i][i];
	}

	return X;
}

int main() {
	matrice A = { {1, -1, 2},{0, 4, 7},{1, 3, -1} };
	matrice B = { {1}, {2}, {3} };

	afficher_matrice(A);
	afficher_matrice(B);
	std::vector<double> X = pivot_de_gauss(A, B);

	std::cout << "x = " << X[0] << "\ny = " << X[1] << "\nz = " << X[2] << "\n";
}