#include <vector>
#include <iostream>
#include <iomanip>

//On utilise un alias pour que ça soit plus lisible.
using matrice = std::vector<std::vector<double>>;

//afficher_matrice
//afficher la matrice dans la console.
// - paramètres : matrice (std::vector<std::vector<double>>)
void afficher_matrice(matrice M) {
	for (auto i : M) {
		for (auto val : i) {
			std::cout << std::setw(4) << val << std::setw(4) << "|";
		} 
		std::cout << "\n";
	}
}

//afficher_vecteur
//afficher un vecteur dans la console.
// - paramètres : vecteur de réels (std::vector<double>)
// - note : s'affiche en ligne et non en colonnes.
void afficher_vecteur(std::vector<double> V) {
	for (auto val : V) {
		std::cout << std::setw(4) << val << std::setw(4) << "|"; 
	}
	std::cout << "\n";
}

//produit_matrice
//calculer le produit matriciel C = A*B en renvoyant un vecteur de doubles 
// - paramètres : matrice carrée (std::vector<std::vector<double>>), vecteur de réels (std::vector<double>)
// - limites : fonctionne pour des matrices carrées, non vides et avec la taille du vecteur B = au nombre de colonnes de A.
std::vector<double> produit_matrice(matrice A, std::vector<double> B) {

	//cas ou on a une matrice vide
	if (A.empty() || A[0].empty()) {
		std::cerr << "Erreur : la matrice ne peut pas etre vide.";
		exit(EXIT_FAILURE);
	}

	//cas ou on a pas une matrice carree.
	if (A[0].size() != A.size()) {
		std::cerr << "Erreur : la fonction ne prend que en compte les matrices carrees.";
		exit(EXIT_FAILURE);
	}

	//cas ou le nombre de colonnes de A (A[0]) != nombre de lignes de B.
	if (A[0].size() != B.size())  {
		std::cerr << "Erreur : il faut que le nombre de colonnes de la premiere matrice soit egal au nombre de lignes de la deuxieme."; 
		exit(EXIT_FAILURE);
	}

	//n = taille de la matrice
	int n = A.size();

	//on créee un vecteur C avec chaque valeurs initialisées à 0.
	std::vector<double> C(n, 0);

	for (int i = 0; i < n; i++) {
		//on utilise n aussi car matrice carrée
		for (int j = 0; j < n; j++) { 
			//on ajoute la somme des produits scalaires entre A et B a C[i]
			C[i] += A[i][j] * B[j];
		}
	}

	return C;
}

//construction_matrice_A
//créér et renvoyer une matrice A qui permet de simuler la diffusion thermique.
// - paramètres : nombre de mailles N (int) et le coefficient r (double) compris entre -0.5 et 0 
// - limites : ne vérifie pas N ou r
matrice construction_matrice_A(int N, double r) {
	//déclaration de la matrice A de taille N avec ses valeurs initialisées à 0
	matrice A(N, std::vector<double>(N, 0));

	//mise en place des valeurs pour les extrémités (relations differentes)
	//pour la 1ère valeur
	A[0][0] = 1 + 3 * r;
	A[0][1] = -r; 

	//pour N-1
	A[N - 1][N - 1] = 1 + 3 * r;
	A[N - 1][N - 2] = -r;

	//mise en place des valeurs pour les mailles intérieures
	for (int i = 1; i < N - 1; i++) {
		A[i][i - 1] = -r;
		A[i][i] = 1 + 2 * r;
		A[i][i + 1] = -r;
	}

	return A;
}

//construction_vecteur_T
//créér et renvoyer le vecteur T qui représente la température initiale
// - paramètres : nombre de mailles N (int)
// - limites : si N est pair, le centre est N/2 (pas exactement la vraie maille centrale)
std::vector<double> construction_vecteur_T(int N) {
	//initialiser les valeurs de la barre à 0°C 
	std::vector<double> T(N, 0);

	//sauf au centre où elle est à 20°C
	T[N / 2] = 20;
	return T;
}

//simuler_evolution_temps
//simuler l'évolution de la température dans le temps en revoyant le vecteur T.
// - paramètres : nombre de mailles N (int) et le coefficient r (double) compris entre -0.5 et 0 , temps (int) (nombre d'itérations).
std::vector<double> simuler_evolution_temps(int N, double r, int temps) {

	//on vérifie que N n'est pas inférieur à 3 (sinon simulation incorrecte)
	if (N < 3) {
		std::cerr << "Erreur : N doit etre superieur a 3";
		exit(EXIT_FAILURE);
	}

	//on vérifie que r est entre -0.5 et 0
	if (r < -0.5 || r > 0) {
		std::cerr << "Erreur : r doit etre compris entre -0.5 et 0";
		exit(EXIT_FAILURE);
	}

	matrice A = construction_matrice_A(N, r);
	std::vector<double> T = construction_vecteur_T(N);

	//pour chaque itérations, on applique T(t+1) = A * T(t)
	for (int t = 0; t < temps; t++) {
		T = produit_matrice(A, T);

		//les extrémités doivent être maintenues à 0°C
		T[0] = 0;
		T[N-1] = 0;
	}

	return T;
}

int main() {

	std::cout << "Les cas speciaux qui generent des erreurs ont etes mis en commentaires.\n\n";

	std::cout << "---------------------------------------------------------------------\n\n";

	//1. Produit entre une matrice de taille 3x3 et un vecteur de taille 3 :
	std::cout << "1. Produit entre une matrice A de taille 3x3 et vecteur B de taille 3 :\n\n";
	matrice A = { {1, -2, 4},{2, 9, 1},{3, -6, 0} };
	std::vector<double> B = {2, 0, 1};

	std::vector<double> C = produit_matrice(A, B);

	afficher_matrice(A);
	std::cout << "*\n";
	afficher_vecteur(B);
	std::cout << "=\n";
	afficher_vecteur(C);
	std::cout << "\n";

	//3. Produit avec une matrice remplie de 0 :
	std::cout << "3. Produit avec une matrice remplie de 0s :\n\n";
	matrice A3 = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
	std::vector<double> B3 = {5, 5, 5};


	std::vector<double> C3 = produit_matrice(A3, B3);

	afficher_matrice(A3);
	std::cout << "*\n";
	afficher_vecteur(B3);
	std::cout << "=\n";
	afficher_vecteur(C3);
	std::cout << "\n";

	//4. (erreur) Matrice non carree :
	//std::cout << "4. (erreur) Matrice non carree :\n";
	//matrice A4 = { {1, 2, 3},{4, 5, 6},{7, 8, 9},{1, 1, 1} };
	//std::vector<double> B4 = {1, 2, 3};

	//std::vector<double> C4 = produit_matrice(A4, B4);
	//std::cout << "\n";

	//5. (erreur) matrice 3x3 et vecteur de taille 2) :
	//std::cout << "5. (erreur) matrice 3x3 et vecteur de taille 2) :\n";
	//matrice A5 = { {1, 0, 0},{0, 1, 0},{0, 0, 1} };
	//std::vector<double> B5 = {4, 5};


	//std::vector<double> C5 = produit_matrice(A5, B5);
	//std::cout << "\n";

	std::cout << "---------------------------------------------------------------------\n\n";

	//1. Etat d'une barre de 5 mailles avec r = -0.001 apres 2000 iterations :
	std::cout << "1. Etat d'une barre de 5 mailles avec r = -0.001 apres 2000 iterations :\n\n";
	std::vector<double> T = simuler_evolution_temps(5, -0.001,  2000);
	afficher_vecteur(T);
	std::cout << "\n";

	//2. Etat d'une barre de 10 mailles avec r = -0.5 (valeur minimale) apres 50 iterations :
	std::cout << "2. Etat d'une barre de 10 mailles avec r = -0.5 (valeur minimale) apres 50 iterations :\n\n";
	std::vector<double> T2 = simuler_evolution_temps(10, -0.5, 50);
	afficher_vecteur(T2);
	std::cout << "\n";

	//3. Etat d'une barre de 7 mailles avec r = -0.2 apres 0 iterations (renvoie le vecteur initial) :
	std::cout << "3. Etat d'une barre de 7 mailles avec r = -0.2 apres 0 iterations (renvoie le vecteur initial) :\n\n";
	std::vector<double> T3 = simuler_evolution_temps(7, -0.2, 0);
	afficher_vecteur(T3);
	std::cout << "\n";

	//5. (erreur) N < 3 :
	//std::cout << "5. (erreur) N < 3";
	//std::vector<double> T5 = simuler_evolution_temps(2, -0.1, 10);
	//afficher_vecteur(T5);
	//std::cout << "\n";

	//6. (erreur) r > 0 :
	//std::cout << "6. (erreur) r > 0 :\n";
	//std::vector<double> T6 = simuler_evolution_temps(5, 0.1, 10);
	//afficher_vecteur(T6);
	//std::cout << "\n";

	//7. (erreur)  r < -0.5 :
	//std::cout << "7. (erreur)  r < -0.5 :\n";
	//std::vector<double> T7 = simuler_evolution_temps(5,-0.8,10);
	//afficher_vecteur(T7);
	//std::cout << "\n";
}