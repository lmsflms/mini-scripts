#include <vector>
#include <iostream>
#include <iomanip>

using matrice = std::vector<std::vector<double>>;

void afficher_matrice(matrice A) {
	for (auto i : A) {
		for (auto val : i) {
			std::cout << std::setw(4) << val << std::setw(4) << "|";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

//Fonction qui réalise le produit de 2 matrices. Une matrice peut être représentée avec un tableau.
matrice produit_matrice(matrice A, matrice B) {
	int n = A.size();

	matrice C(n, std::vector<double>(n, 0)); //on créee une matrice C avec chaque valeurs initialisées à 0.

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}

	return C;
}


int main() {
	matrice A = { { {1,2,3},{5,-2,3},{0,2,1} } };
	matrice B = { { {1,0,0},{0,1,0},{0,0,1} } };

	matrice C = produit_matrice(A, B);
	afficher_matrice(C);
}