#include <iostream>
#include <functional>

auto f = [](double x) {
    return x * x;
    };

double valeur_integrale(std::function<double(double)> f, double a, double b, int n) {
    if (n == 0) {
        return 0; //gestion du cas ou n = 0
    }
    if (a == b) {
        return 0; //gestion du cas ou a = b
    }

    //a > b attention

    double largeur_rect = (b - a) / n;
    double integrale = 0;

    //Méthode des rectangles
    for (int i = 0; i < n; i++) {
        double milieu_rect_x = a + largeur_rect * (i + 0.5);
        double hauteur_rect = f(milieu_rect_x);
        integrale += hauteur_rect * largeur_rect;
    }
    return integrale;
}

int main() {
    double a = 0.0;
    double b = 1.0;
    int n = 50;

    double val_integrale = valeur_integrale(f, a, b, n);

    std::cout << val_integrale;
}