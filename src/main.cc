#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <cmath>
#include <mutex>

double f(double x) {
    return log(sin(x)+cos(x)); // Cambia esta función según lo necesites
}

void simpson_parallel(double a, double b, int n, int start, int end, double &partial_result) {
    double h = (b - a) / n;
    partial_result = 0.0;

    for (int i = start; i <= end; ++i) {
        double x = a + i * h;
        if (i == 0 || i == n) {
            partial_result += f(x); // Extremos
        } else if (i % 2 == 0) {
            partial_result += 2 * f(x); // Coeficiente 2
        } else {
            partial_result += 4 * f(x); // Coeficiente 4
        }
    }
}

double simpson(double a, double b, int n, int num_threads) {
    double h = (b - a) / n;
    double result = 0.0;
    std::vector<std::thread> threads;
    std::vector<double> partial_results(num_threads,0.0f);

    int subintervals_per_thread = n / num_threads;
    for (int i = 0; i < num_threads; ++i) {
        int start = i * subintervals_per_thread;
        int end = (i + 1) * subintervals_per_thread - 1;
        if (i == num_threads - 1) {
            end = n; // Asegurar que el último hilo tome todos los subintervalos restantes
        }
        threads.emplace_back(simpson_parallel, a, b, n, start, end, std::ref(partial_results[i]));
    }

    for (auto &thread : threads) {
        thread.join();
    }

    for (const auto& res : partial_results)
        result+=res;

    return (h / 3) * result;
}

int main(int argc, char** argv) {
    double a = 0, b = M_PI / 2; // Intervalo de integración
    unsigned long long n = 100000000000;        
    int num_threads = atoi(argv[1]);    // Número de hilos

    double result = simpson(a, b, n, num_threads);
    std::cout << "Resultado de la integral: " << result << std::endl;

    return 0;
}
