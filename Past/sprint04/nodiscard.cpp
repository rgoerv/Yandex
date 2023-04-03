#include <iostream>
#include <cmath>

using namespace std;

[[nodiscard]] bool SolveQuadraticEquation(double a, double b, double c, 
                                          pair<double, double>& roots) {
    double d = b * b - 4 * a * c;
    if (d < 0) {
        return false;
    }

    double x1 = (-b - sqrt(d)) / (2 * a);
    double x2 = (-b + sqrt(d)) / (2 * a);
    roots = {x1, x2};
    return true;
}

int main() {
    cout << "Введите коэффициенты уравнения a*x^2 + b*x + c = 0"s << endl;
    double a1, b1, c1;
    cin >> a1 >> b1 >> c1;

    bool success;
    /*...*/
    pair<double, double> roots1, roots2;
    success = SolveQuadraticEquation(a1, b1, c1, roots1);

    double a2, b2, c2;
    cin >> a2 >> b2 >> c2;    
    // Информация об ошибке вычисления предыдущего уравнения оказалась утеряна
    success = SolveQuadraticEquation(a2, b2, c2, roots2);
    if (success) {
        /* обрабатываем roots1 и roots2 */
}