
#include <omp.h>
#include <iostream>
#include <iomanip>
#include <clocale>

int main()
{
    const int N = 16000;
    const int NUM_THREADS = 8;

    int method;

    std::cout << "Choose scheduling method:\n";
    std::cout << "1 - static\n";
    std::cout << "2 - dynamic\n";
    std::cout << "3 - guided\n";
    std::cout << "Your choice: ";
    std::cin >> method;

    if (method < 1 || method > 3)
    {
        std::cout << "Invalid method selection.\n";
        return 1;
    }

    double* a = new double[N];
    double* b = new double[N];

    // Initialize array a
    for (int i = 0; i < N; ++i)
    {
        a[i] = static_cast<double>(i);
    }

    // Initialize array b
    for (int i = 0; i < N; ++i)
    {
        b[i] = -1.0;
    }

    omp_set_dynamic(0);
    omp_set_num_threads(NUM_THREADS);

    // Parallel loop with selected scheduling method
    if (method == 1)
    {
        std::cout << "\nSelected method: static\n";

#pragma omp parallel for schedule(static)
        for (int i = 1; i < N - 1; ++i)
        {
            b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
        }
    }
    else if (method == 2)
    {
        std::cout << "\nSelected method: dynamic\n";

#pragma omp parallel for schedule(dynamic)
        for (int i = 1; i < N - 1; ++i)
        {
            b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
        }
    }
    else if (method == 3)
    {
        std::cout << "\nSelected method: guided\n";

#pragma omp parallel for schedule(guided)
        for (int i = 1; i < N - 1; ++i)
        {
            b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
        }
    }

    // Set boundary elements
    b[0] = a[0];
    b[N - 1] = a[N - 1];

    // Output results
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "First 5 elements of b: ";
    for (int i = 0; i < 5; ++i)
    {
        std::cout << b[i] << " ";
    }

    std::cout << "\nLast 5 elements of b: ";
    for (int i = N - 5; i < N; ++i)
    {
        std::cout << b[i] << " ";
    }

    std::cout << std::endl;

    delete[] a;
    delete[] b;

    return 0;
}
