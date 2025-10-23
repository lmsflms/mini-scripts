// La fonction segments_intersection retourne true si les segments [AB] et [CD]

#include <iostream>

bool segments_intersection(double x_a, double y_a, double x_b, double y_b, double x_c, double y_c, double x_d, double y_d)
{
    double a = 0, a2 = 0, b = 0, b2 = 0, c = 0, c2 = 0, Px = 0, Py = 0;

    //ETAPE 0 : génerer les deux droites (AB) et (CD) à partir des points sous la forme ax + by + c = 0

    if (x_a == x_b) { //Si x_a = x_b : la droite (AB) est verticale d'equation x = x_a
        a = 1; //coefficient de x
        b = 0; //il n'y a pas de y dans l'equation (droite verticale)
        c = -x_a; //pour que 1*x + 0*y + c = 0, il faut c = -x_a
    }
    else if (y_a == y_b) { //Si y_a = y_b : la droite (AB) est horizontale d'equation y = y_a
        a = 0;
        b = 1;
        c = -y_a;
    }
    else {//Sinon la droite (AB) est oblique. On met b = 1 et on calcule a et c pour avoir l'équation ax + 1y + c = 0 (pour éviter les divisions par 0)

        a = (y_b - y_a) / (x_a - x_b);
        b = 1;
        c = (y_a * x_b - y_b * x_a) / (x_a - x_b);
    }
    if (x_c == x_d) { //Si x_c = x_d : la droite (CD) est verticale d'equation x = x_c
        a2 = 1;
        b2 = 0;
        c2 = -x_c;
    }
    else if (y_c == y_d) { //Si y_c = y_d : la droite (CD) est horizontale d'equation y = y_c
        a2 = 0;
        b2 = 1;
        c2 = -y_c;
    }
    else {//Sinon la droite (CD) est oblique. On met b = 1 et on calcule a et c pour avoir l'équation ax + 1y + c = 0 (pour éviter les divisions par 0)
        a2 = (y_d - y_c) / (x_c - x_d);
        b2 = 1;
        c2 = (y_c * x_d - y_d * x_c) / (x_c - x_d);
    }

    // ETAPE 1 : déterminer si il y a intersection entre (AB) et (CD)

    double determinant = a * b2 - a2 * b; //pour déterminer s'il y a intersection il faut calculer le déterminant

    if (determinant != 0) { //les droites se croisent en un point d'intersection.
        //Le point d'intersection (Px, Py) est calculé par :
        Px = (b * c2 - b2 * c) / determinant;
        Py = (a2 * c - a * c2) / determinant;
    }
    else { //les droites sont parallèles ou alignées
        if ((a * b2 == a2 * b) && (a * c2 == a2 * c) && (b * c2 == b2 * c)) { //Verifier si les droites sont alignées. J'ai évité ((a / a2) == (b / b2) && (b / b2) == (c / c2)) pour ne pas avoir à faire des tests à cause de la division.
            if ((x_a >= x_c && x_a <= x_d) || (x_b >= x_c && x_b <= x_d) || (x_c >= x_a && x_c <= x_b) || (x_d >= x_a && x_d <= x_b)) { //chevauchement X
                if ((y_a >= y_c && y_a <= y_d) || (y_b >= y_c && y_b <= y_d) || (y_c >= y_a && y_c <= y_b) || (y_d >= y_a && y_d <= y_b)) { //chevauchement Y
                    return true; //Les segments [AB] et [CD] sont cofondus.
                }
            }
        }
        else {
            return false; //il n'y a pas d'intersection.
        }
    }

    // ETAPE 2 : Déterminer si le point d'intersection est situé entre les bornes des deux segments.

    if (((Px >= x_a && Px <= x_b) || (Px >= x_b && Px <= x_a)) //On vérifie que le point d'intersection (Px, Py) est bien sur les deux segments.
        && ((Py >= y_a && Py <= y_b) || (Py >= y_b && Py <= y_a))
        && ((Px >= x_c && Px <= x_d) || (Px >= x_d && Px <= x_c))
        && ((Py >= y_c && Py <= y_d) || (Py >= y_d && Py <= y_c))) {
        return true; //Les segments [AB] et [CD] se croisent (le point  d'intersection est sur les segments)
    }
    return false; //Les segments [AB] et [CD] ne se croisent pas (le point  d'intersection n'est pas sur les segments)
}

int main() {
    //Test 1 : segments obliques qui se croisent : 1 (vrai)
    std::cout << "Segments obliques qui se croisent : " << segments_intersection(1, 1, 6, 4, 4, 1, 3, 4) << "\n";

    //Test 2 : segments horizontaux qui se croisent : 1 (vrai)
    std::cout << "Segments horizontaux qui se croisent : " << segments_intersection(0, 2, 5, 2, 3, 2, 7, 2) << "\n";

    //Test 3 : segments verticaux qui se croisent : 1 (vrai)
    std::cout << "Segments verticaux qui se croisent : " << segments_intersection(2, 1, 2, 5, 2, 3, 2, 6) << "\n";

    //Test 5 : segments alignes avec chevauchement : 1 (vrai)
    std::cout << "Segments alignes avec chevauchement : " << segments_intersection(0, 0, 5, 0, 1, 0, 4, 0) << "\n";

    //Test 6 : segments alignes sans chevauchement : 0 (faux) 
    std::cout << "Segments alignes sans chevauchement : " << segments_intersection(0, 0, 2, 0, 3, 0, 5, 0) << "\n";

    //Test 7 : segments parallèls : 0 (faux)
    std::cout << "Segments obliques paralleles : " << segments_intersection(0, 0, 3, 3, 0, 1, 3, 4) << "\n";
}