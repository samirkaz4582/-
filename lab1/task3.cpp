
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <omp.h>

using namespace std;

int main()
{
    int ntr = 0;
    int method = 0;

    cout << "Enter number of threads: ";
    cin >> ntr;

    if (ntr <= 0)
    {
        cout << "Number of threads must be positive.\n";
        return 1;
    }

    cout << "\nChoose a method:\n";
    cout << "1 - critical\n";
    cout << "2 - barrier\n";
    cout << "3 - atomic\n";
    cout << "4 - OpenMP lock\n";
    cout << "5 - condition_variable\n";
    cout << "Your choice: ";
    cin >> method;

    if (method < 1 || method > 5)
    {
        cout << "Invalid method.\n";
        return 1;
    }

    omp_set_dynamic(0);

    // Method 1: critical
    if (method == 1)
    {
        int turn = -1;

        cout << "\nMethod 1 (critical):\n";

#pragma omp parallel num_threads(ntr) shared(turn)
        {
            int id = omp_get_thread_num();

#pragma omp single
            {
                turn = omp_get_num_threads() - 1;
            }

            bool printed = false;

            while (!printed)
            {
#pragma omp critical
                {
                    if (turn == id)
                    {
                        printf("Thread %d: Hello world\n", id);
                        turn--;
                        printed = true;
                    }
                }

                if (!printed)
                    this_thread::yield();
            }
        }
    }

    // Method 2: barrier
    else if (method == 2)
    {
        cout << "\nMethod 2 (barrier):\n";

#pragma omp parallel num_threads(ntr)
        {
            int id = omp_get_thread_num();
            int total = omp_get_num_threads();

            for (int turn = total - 1; turn >= 0; turn--)
            {
#pragma omp barrier

                if (id == turn)
                    printf("Thread %d: Hello world\n", id);

#pragma omp barrier
            }
        }
    }


    // Method 3: critical synchronization
    else if (method == 3)
    {
        int turn = ntr - 1;

        cout << "\nMethod 3 (critical synchronization):\n";

#pragma omp parallel num_threads(ntr) shared(turn)
        {
            int id = omp_get_thread_num();
            bool printed = false;

            while (!printed)
            {
#pragma omp critical
                {
                    if (turn == id)
                    {
                        printf("Thread %d: Hello world\n", id);
                        turn--;
                        printed = true;
                    }
                }

                if (!printed)
                    this_thread::yield();
            }
        }
    }


    // Method 4: OpenMP lock
    else if (method == 4)
    {
        int turn = -1;
        omp_lock_t lock;

        omp_init_lock(&lock);

        cout << "\nMethod 4 (OpenMP lock):\n";

#pragma omp parallel num_threads(ntr) shared(turn, lock)
        {
            int id = omp_get_thread_num();

#pragma omp single
            {
                turn = omp_get_num_threads() - 1;
            }

            bool printed = false;

            while (!printed)
            {
                omp_set_lock(&lock);

                if (turn == id)
                {
                    printf("Thread %d: Hello world\n", id);
                    turn--;
                    printed = true;
                }

                omp_unset_lock(&lock);

                if (!printed)
                    this_thread::yield();
            }
        }

        omp_destroy_lock(&lock);
    }

    // Method 5: condition_variable
    else if (method == 5)
    {
        int turn = -1;
        mutex mtx;
        condition_variable cv;

        cout << "\nMethod 5 (condition_variable):\n";

#pragma omp parallel num_threads(ntr) shared(turn, mtx, cv)
        {
            int id = omp_get_thread_num();

#pragma omp single
            {
                turn = omp_get_num_threads() - 1;
            }

            unique_lock<mutex> guard(mtx);

            cv.wait(guard, [&]()
                {
                    return turn == id;
                });

            printf("Thread %d: Hello world\n", id);

            turn--;

            guard.unlock();
            cv.notify_all();
        }
    }

    return 0;
}
