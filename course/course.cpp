#include <iostream>
#include <cmath>
#include <functional>
#include <chrono>

// структура для возврата результата и количества итераций
struct Result {
    double root;
    int iterations;
    double duration;  // в микросекундах
};

// функции и их производные
double f1(double x) { return x * x - x - 2; }
double df1(double x) { return 2 * x - 1; }

double f2(double x) { return exp(x) - 3 * x * x; }
double df2(double x) { return exp(x) - 6 * x; }

// методы для решения уравнений
Result bisection_method(std::function<double(double)> f, double a, double b, double tol = 1e-6) {
    int iter = 0;
    auto start = std::chrono::high_resolution_clock::now();
    if (f(a) * f(b) >= 0) {
        throw std::invalid_argument("Функция должна иметь разные знаки на концах интервала a и b.");
    }
    while ((b - a) / 2.0 > tol) {
        double c = (a + b) / 2.0;
        if (f(c) == 0) {
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::micro> duration = end - start;
            return { c, iter, duration.count() };
        }
        else if (f(a) * f(c) < 0) {
            b = c;
        }
        else {
            a = c;
        }
        iter++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> duration = end - start;
    return { (a + b) / 2.0, iter, duration.count() };
}

Result newton_raphson_method(std::function<double(double)> f, std::function<double(double)> df, double x0, double tol = 1e-6, int max_iter = 100) {
    int iter = 0;
    double x = x0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < max_iter; i++) {
        double x_new = x - f(x) / df(x);
        if (std::abs(x_new - x) < tol) {
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::micro> duration = end - start;
            return { x_new, iter, duration.count() };
        }
        x = x_new;
        iter++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> duration = end - start;
    return { x, iter, duration.count() };
}

Result secant_method(std::function<double(double)> f, double x0, double x1, double tol = 1e-6, int max_iter = 100) {
    int iter = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < max_iter; i++) {
        if (std::abs(f(x1) - f(x0)) < tol) {
            throw std::invalid_argument("Значения функции в x0 и x1 слишком близки.");
        }
        double x_new = x1 - f(x1) * (x1 - x0) / (f(x1) - f(x0));
        if (std::abs(x_new - x1) < tol) {
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::micro> duration = end - start;
            return { x_new, iter, duration.count() };
        }
        x0 = x1;
        x1 = x_new;
        iter++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> duration = end - start;
    return { x1, iter, duration.count() };
}

Result fixed_point_iteration(std::function<double(double)> g, double x0, double tol = 1e-6, int max_iter = 100) {
    int iter = 0;
    double x = x0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < max_iter; i++) {
        double x_new = g(x);
        if (std::abs(x_new - x) < tol) {
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::micro> duration = end - start;
            return { x_new, iter, duration.count() };
        }
        x = x_new;
        iter++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> duration = end - start;
    return { x, iter, duration.count() };
}

void solve_and_compare(std::function<double(double)> f, std::function<double(double)> df, std::function<double(double)> g, double a, double b, double x0, const std::string& function_name) {
    // Метод бисекции
    try {
        Result res_bisection = bisection_method(f, a, b);
        std::cout << "Метод бисекции (" << function_name << "): корень = " << res_bisection.root << ", итерации = " << res_bisection.iterations << ", время = " << res_bisection.duration << " микросекунд\n";
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Метод бисекции (" << function_name << "): ошибка - " << e.what() << "\n";
    }
    // Метод Ньютона-Рафсона
    try {
        Result res_newton = newton_raphson_method(f, df, x0);
        std::cout << "Метод Ньютона-Рафсона (" << function_name << "): корень = " << res_newton.root << ", итерации = " << res_newton.iterations << ", время = " << res_newton.duration << " микросекунд\n";
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Метод Ньютона-Рафсона (" << function_name << "): ошибка - " << e.what() << "\n";
    }
    // Метод секущих
    try {
        Result res_secant = secant_method(f, a, b);
        std::cout << "Метод секущих (" << function_name << "): корень = " << res_secant.root << ", итерации = " << res_secant.iterations << ", время = " << res_secant.duration << " микросекунд\n";
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Метод секущих (" << function_name << "): ошибка - " << e.what() << "\n";
    }
    // Метод простой итерации
    try {
        Result res_fixed_point = fixed_point_iteration(g, x0);
        std::cout << "Метод простой итерации (" << function_name << "): корень = " << res_fixed_point.root << ", итерации = " << res_fixed_point.iterations << ", время = " << res_fixed_point.duration << " микросекунд\n";
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Метод простой итерации (" << function_name << "): ошибка - " << e.what() << "\n";
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    // определение функций, их производных и начальных приближений
    std::function<double(double)> f1_func = f1, df1_func = df1, g1 = [](double x) { return sqrt(x + 2); };
    std::function<double(double)> f2_func = f2, df2_func = df2, g2 = [](double x) { return sqrt(exp(x) / 3); };

    // решение и сравнение
    solve_and_compare(f1_func, df1_func, g1, 0, 3, 1.5, "f1(x) = x^2 - x - 2");
    solve_and_compare(f2_func, df2_func, g2, 0, 3, 1.5, "f2(x) = e^x - 3x^2");

    return 0;
}
