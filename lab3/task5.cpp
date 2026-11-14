
#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>

using namespace std;

struct Point
{
    double x, y, z;
};

int main()
{
    setlocale(LC_ALL, "Russian");

    vector<Point> points;
    Point p;

    string filename;

    cout << "Enter input file name: ";
    cin >> filename;

    // Read points from file
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error: cannot open file!" << endl;
        return 1;
    }

    while (file >> p.x >> p.y >> p.z)
    {
        points.push_back(p);
    }

    file.close();

    int N = static_cast<int>(points.size());

    if (N == 0)
    {
        cout << "Error: file contains no points!" << endl;
        return 1;
    }

    int choice;

    cout << "\nChoose calculation method:" << endl;
    cout << "1 - Parallel loop" << endl;
    cout << "2 - Functional decomposition" << endl;
    cout << "Your choice: ";
    cin >> choice;

    double sumX = 0.0;
    double sumY = 0.0;
    double sumZ = 0.0;

    double centerX, centerY, centerZ;

    omp_set_dynamic(0);

    // Method 1: Parallel loop
    if (choice == 1)
    {
        omp_set_num_threads(4);

#pragma omp parallel for reduction(+:sumX, sumY, sumZ)
        for (int i = 0; i < N; i++)
        {
            sumX += points[i].x;
            sumY += points[i].y;
            sumZ += points[i].z;
        }
    }

    // Method 2: Functional decomposition
    else if (choice == 2)
    {
#pragma omp parallel sections num_threads(3)
        {
#pragma omp section
            {
                for (int i = 0; i < N; i++)
                {
                    sumX += points[i].x;
                }
            }

#pragma omp section
            {
                for (int i = 0; i < N; i++)
                {
                    sumY += points[i].y;
                }
            }

#pragma omp section
            {
                for (int i = 0; i < N; i++)
                {
                    sumZ += points[i].z;
                }
            }
        }
    }

    else
    {
        cout << "Invalid choice!" << endl;
        return 1;
    }

    // Calculate geometric center
    centerX = sumX / N;
    centerY = sumY / N;
    centerZ = sumZ / N;

    // Display results
    cout << "\nNumber of points: " << N << endl;

    cout << "\nGeometric center:" << endl;
    cout << "X = " << centerX << endl;
    cout << "Y = " << centerY << endl;
    cout << "Z = " << centerZ << endl;

    cout << "\nCalculation completed!" << endl;

    return 0;
}
